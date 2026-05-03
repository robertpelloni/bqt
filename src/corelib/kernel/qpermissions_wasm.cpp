// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <private/qpermissions_p.h>
#include <private/qstdweb_p.h>

#include <qglobalstatic.h>
#include <qpermissions.h>
#include <qmetaobject.h>
#include <qnamespace.h>
#include <qmetatype.h>
#include <qstring.h>
#include <bobuiimer.h>
#include <qhash.h>

#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>

#include <utility>
#include <string>
#include <queue>

BOBUI_BEGIN_NAMESPACE

using namespace QPermissions::Private;
using namespace emscripten;

namespace
{
    constexpr const char *wapiGranted = "granted";
    constexpr const char *wapiDenied = "denied";
    constexpr const char *wapiPrompt = "prompt";
    constexpr const char *wapiCamera = "camera";
    constexpr const char *wapiVideoCapture = "video_capture"; // Alternative to "camera".
    constexpr const char *wapiMicrophone = "microphone";
    constexpr const char *wapiAudioCapture = "audio_capture"; // Alternative to "microphone".
    constexpr const char *wapiGeolocation = "geolocation";

    void updatePermission(const std::string &name, const std::string &state,
                          PermissionCallback callback);

    void checkPermission(const std::string &permissionName)
    {
        val permissions = val::global("navigator")["permissions"];
        if (permissions.isUndefined() || permissions.isNull())
            return;

        qstdweb::PromiseCallbacks callbacks;
        callbacks.thenFunc = [permissionName](val permissionState)
        {
            updatePermission(permissionName, permissionState["state"].as<std::string>(), {});
        };
        callbacks.catchFunc = [permissionName](val err)
        {
            if (err["name"].as<std::string>() == "NotAllowedError")
                return updatePermission(permissionName, wapiDenied, {});

            qCInfo(lcPermissions, "'%s' '%s'", err["name"].as<std::string>().c_str(),
                err["message"].as<std::string>().c_str());
        };

        val query = val::object();
        query.set("name", val(permissionName));

        qstdweb::Promise::make(permissions, QStringLiteral("query"), callbacks, query);
    }

    void checkPermissions()
    {
        checkPermission(wapiCamera);
        checkPermission(wapiMicrophone);
        checkPermission(wapiGeolocation);
    }

    void bootstrapCheckPermissions()
    {
        BOBUIimer::singleShot(0, []{checkPermissions();});
    }

    Q_CONSTRUCTOR_FUNCTION(bootstrapCheckPermissions);

    int permissionTypeIdFromString(const std::string &permission)
    {
        if (permission == wapiCamera || permission == wapiVideoCapture)
            return qMetaTypeId<QCameraPermission>();
        if (permission == wapiMicrophone || permission == wapiAudioCapture)
            return qMetaTypeId<QMicrophonePermission>();
        if (permission == wapiGeolocation)
            return qMetaTypeId<QLocationPermission>();

        qCWarning(lcPermissions, "Unknown permission type '%s'", permission.c_str());

        return -1;
    }

    BobUI::PermissionStatus permissionStatusFromString(const std::string &state)
    {
        if (state == wapiGranted)
            return BobUI::PermissionStatus::Granted;
        if (state == wapiDenied)
            return BobUI::PermissionStatus::Denied;
        if (state == wapiPrompt)
            return BobUI::PermissionStatus::Undetermined;

        qCWarning(lcPermissions, "Unknown permission state '%s'", state.c_str());

        return BobUI::PermissionStatus::Denied;
    }

    using PermissionHash = QHash<int, BobUI::PermissionStatus>;
    Q_GLOBAL_STATIC(PermissionHash, permissionStatuses);

    void updatePermission(const std::string &name, const std::string &state, PermissionCallback callback)
    {
        qCDebug(lcPermissions) << "Updating" << name << "permission to" << state;

        const int type = permissionTypeIdFromString(name);
        if (type == -1)
            return; // Unknown permission type

        const auto status = permissionStatusFromString(state);
        (*permissionStatuses)[type] = status;

        if (callback)
            callback(status);
    }

    void requestMediaDevicePermission(const std::string &device, const PermissionCallback &cb)
    {
        Q_ASSERT(cb);

        val mediaDevices = val::global("navigator")["mediaDevices"];
        if (mediaDevices.isUndefined())
            return cb(BobUI::PermissionStatus::Denied);

        qstdweb::PromiseCallbacks queryCallbacks;
        queryCallbacks.thenFunc = [device, cb](val)
        {
            updatePermission(device, wapiGranted, cb);
        };
        queryCallbacks.catchFunc = [device, cb](val error)
        {
            if (error["name"].as<std::string>() == "NotAllowedError")
                return updatePermission(device, wapiDenied, cb);
            updatePermission(device, wapiPrompt, cb);
        };

        val constraint = val::object();
        if (device == wapiCamera)
            constraint.set("video", true);
        else
            constraint.set("audio", true);

        qstdweb::Promise::make(mediaDevices, QStringLiteral("getUserMedia"), queryCallbacks, constraint);
    }

    using GeoRequest = std::pair<QPermission, PermissionCallback>;
    Q_GLOBAL_STATIC(std::deque<GeoRequest>, geolocationRequestQueue);

    bool processingLocationRequest = false;

    void processNextGeolocationRequest();

    void geolocationSuccess(val position)
    {
        Q_UNUSED(position);
        Q_ASSERT(geolocationRequestQueue->size());

        processingLocationRequest = false;

        auto cb = geolocationRequestQueue->front().second;
        geolocationRequestQueue->pop_front();
        updatePermission(wapiGeolocation, wapiGranted, cb);
        processNextGeolocationRequest();
    }

    void geolocationError(val error)
    {
        Q_ASSERT(geolocationRequestQueue->size());

        static int deniedError = []
        {
            val posErr = val::global("GeolocationPositionError");
            if (posErr.isUndefined() || posErr.isNull())
                return 1;
            return posErr["PERMISSION_DENIED"].as<int>();
        }();

        processingLocationRequest = false;

        auto cb = geolocationRequestQueue->front().second;
        geolocationRequestQueue->pop_front();

        const int errorCode = error["code"].as<int>();
        updatePermission(wapiGeolocation, errorCode == deniedError ? wapiDenied : wapiPrompt, cb);
        processNextGeolocationRequest();
    }

    EMSCRIPTEN_BINDINGS(bobui_permissions) {
        function("bobuiLocationSuccess", &geolocationSuccess);
        function("bobuiLocationError", &geolocationError);
    }

    void processNextGeolocationRequest()
    {
        if (processingLocationRequest)
            return;

        if (geolocationRequestQueue->empty())
            return;

        processingLocationRequest = true;

        val geolocation = val::global("navigator")["geolocation"];
        Q_ASSERT(!geolocation.isUndefined());
        Q_ASSERT(!geolocation.isNull());

        const auto &permission = geolocationRequestQueue->front().first;
        const auto locationPermission = *permission.value<QLocationPermission>();
        const bool highAccuracy = locationPermission.accuracy() == QLocationPermission::Precise;

        val options = val::object();
        options.set("enableHighAccuracy", highAccuracy ? true : false);
        geolocation.call<void>("getCurrentPosition", val::module_property("bobuiLocationSuccess"),
                               val::module_property("bobuiLocationError"), options);
    }

    void requestGeolocationPermission(const QPermission &permission, const PermissionCallback &cb)
    {
        Q_ASSERT(cb);
        Q_UNUSED(permission);
        Q_UNUSED(cb);

        val geolocation = val::global("navigator")["geolocation"];
        if (geolocation.isUndefined() || geolocation.isNull())
            return cb(BobUI::PermissionStatus::Denied);

        if (processingLocationRequest)
            qCWarning(lcPermissions, "Permission to access location requested, while another request is in progress");

        geolocationRequestQueue->push_back(std::make_pair(permission, cb));
        processNextGeolocationRequest();
    }
} // Unnamed namespace

namespace QPermissions::Private
{
    BobUI::PermissionStatus checkPermission(const QPermission &permission)
    {
        const auto it = permissionStatuses->find(permission.type().id());
        return it != permissionStatuses->end() ? it.value() : BobUI::PermissionStatus::Undetermined;
    }

    void requestPermission(const QPermission &permission, const PermissionCallback &callback)
    {
        Q_ASSERT(permission.type().isValid());
        Q_ASSERT(callback);

        const auto status = checkPermission(permission);
        if (status != BobUI::PermissionStatus::Undetermined)
            return callback(status);

        const int requestedTypeId = permission.type().id();
        if (requestedTypeId == qMetaTypeId<QCameraPermission>())
            return requestMediaDevicePermission(wapiCamera, callback);

        if (requestedTypeId == qMetaTypeId<QMicrophonePermission>())
            return requestMediaDevicePermission(wapiMicrophone, callback);

        if (requestedTypeId == qMetaTypeId<QLocationPermission>())
            return requestGeolocationPermission(permission, callback);

        (*permissionStatuses)[requestedTypeId] = BobUI::PermissionStatus::Denied;
        callback(BobUI::PermissionStatus::Denied);
    }
}

BOBUI_END_NAMESPACE
