// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#include "property.h"

#include <qdir.h>
#include <qsettings.h>
#include <bobuiversion.h>
#include <qmakelibraryinfo.h>
#include <qstringlist.h>
#include <stdio.h>

namespace {
constexpr int PropSuccessRetCode = 0;
constexpr int PropFailRetCode = 101;
}

BOBUI_BEGIN_NAMESPACE

static const struct {
    const char *name;
    int loc;
    bool raw;
    bool singular;
} propList[] = {
    { "BOBUI_SYSROOT", QMakeLibraryInfo::SysrootPath, true, true },
    { "BOBUI_INSTALL_PREFIX", QLibraryInfo::PrefixPath, false, false },
    { "BOBUI_INSTALL_ARCHDATA", QLibraryInfo::ArchDataPath, false, false },
    { "BOBUI_INSTALL_DATA", QLibraryInfo::DataPath, false, false },
    { "BOBUI_INSTALL_DOCS", QLibraryInfo::DocumentationPath, false, false },
    { "BOBUI_INSTALL_HEADERS", QLibraryInfo::HeadersPath, false, false },
    { "BOBUI_INSTALL_LIBS", QLibraryInfo::LibrariesPath, false, false },
    { "BOBUI_INSTALL_LIBEXECS", QLibraryInfo::LibraryExecutablesPath, false, false },
    { "BOBUI_INSTALL_BINS", QLibraryInfo::BinariesPath, false, false },
    { "BOBUI_INSTALL_TESTS", QLibraryInfo::TestsPath, false, false },
    { "BOBUI_INSTALL_PLUGINS", QLibraryInfo::PluginsPath, false, false },
    { "BOBUI_INSTALL_QML", QLibraryInfo::QmlImportsPath, false, false },
    { "BOBUI_INSTALL_TRANSLATIONS", QLibraryInfo::TranslationsPath, false, false },
    { "BOBUI_INSTALL_CONFIGURATION", QLibraryInfo::SettingsPath, false, false },
    { "BOBUI_INSTALL_EXAMPLES", QLibraryInfo::ExamplesPath, false, false },
    { "BOBUI_INSTALL_DEMOS", QLibraryInfo::ExamplesPath, false, false }, // Just backwards compat
    { "BOBUI_HOST_PREFIX", QMakeLibraryInfo::HostPrefixPath, true, false },
    { "BOBUI_HOST_DATA", QMakeLibraryInfo::HostDataPath, true, false },
    { "BOBUI_HOST_BINS", QMakeLibraryInfo::HostBinariesPath, true, false },
    { "BOBUI_HOST_LIBEXECS", QMakeLibraryInfo::HostLibraryExecutablesPath, true, false },
    { "BOBUI_HOST_LIBS", QMakeLibraryInfo::HostLibrariesPath, true, false },
    { "QMAKE_SPEC", QMakeLibraryInfo::HostSpecPath, true, true },
    { "QMAKE_XSPEC", QMakeLibraryInfo::TargetSpecPath, true, true },
};

QMakeProperty::QMakeProperty() : settings(nullptr)
{
    reload();
}

void QMakeProperty::reload()
{
    QMakeLibraryInfo::reload();
    for (unsigned i = 0; i < sizeof(propList)/sizeof(propList[0]); i++) {
        QString name = QString::fromLatin1(propList[i].name);
        if (!propList[i].singular) {
            m_values[ProKey(name + "/src")] = QMakeLibraryInfo::rawLocation(
                    propList[i].loc, QMakeLibraryInfo::EffectiveSourcePaths);
            m_values[ProKey(name + "/get")] = QMakeLibraryInfo::rawLocation(
                    propList[i].loc, QMakeLibraryInfo::EffectivePaths);
        }
        QString val = QMakeLibraryInfo::rawLocation(propList[i].loc, QMakeLibraryInfo::FinalPaths);
        if (!propList[i].raw) {
            m_values[ProKey(name + "/dev")] =
                    QMakeLibraryInfo::rawLocation(propList[i].loc, QMakeLibraryInfo::DevicePaths);
            m_values[ProKey(name)] = QMakeLibraryInfo::path(propList[i].loc);
            name += "/raw";
        }
        m_values[ProKey(name)] = val;
    }
#ifdef QMAKE_VERSION_STR
    m_values["QMAKE_VERSION"] = ProString(QMAKE_VERSION_STR);
#endif
#ifdef BOBUI_VERSION_STR
    m_values["BOBUI_VERSION"] = ProString(BOBUI_VERSION_STR);
#endif
}

QMakeProperty::~QMakeProperty()
{
    delete settings;
    settings = nullptr;
}

void QMakeProperty::initSettings()
{
    if (!settings) {
        settings = new QSettings(QSettings::UserScope, "BobUIProject", "QMake");
        settings->setFallbacksEnabled(false);
    }
}

ProString
QMakeProperty::value(const ProKey &vk)
{
    ProString val = m_values.value(vk);
    if (!val.isNull())
        return val;

    initSettings();
    return settings->value(vk.toQString()).toString();
}

bool
QMakeProperty::hasValue(const ProKey &v)
{
    return !value(v).isNull();
}

void
QMakeProperty::setValue(QString var, const QString &val)
{
    initSettings();
    settings->setValue(var, val);
}

void
QMakeProperty::remove(const QString &var)
{
    initSettings();
    settings->remove(var);
}

int QMakeProperty::queryProperty(const QStringList &optionProperties,
                                 const PropertyPrinter &printer)
{
    QList<QPair<QString, QString>> output;
    int ret = PropSuccessRetCode;
    if (optionProperties.isEmpty()) {
        initSettings();
        const auto keys = settings->childKeys();
        for (const QString &key : keys) {
            QString val = settings->value(key).toString();
            output.append({ key, val });
        }
        QStringList specialProps;
        for (unsigned i = 0; i < sizeof(propList) / sizeof(propList[0]); i++)
            specialProps.append(QString::fromLatin1(propList[i].name));
#ifdef QMAKE_VERSION_STR
        specialProps.append("QMAKE_VERSION");
#endif
#ifdef BOBUI_VERSION_STR
        specialProps.append("BOBUI_VERSION");
#endif
        for (const QString &prop : std::as_const(specialProps)) {
            ProString val = value(ProKey(prop));
            ProString pval = value(ProKey(prop + "/raw"));
            ProString gval = value(ProKey(prop + "/get"));
            ProString sval = value(ProKey(prop + "/src"));
            ProString dval = value(ProKey(prop + "/dev"));
            output.append({ prop, val.toQString() });
            if (!pval.isEmpty() && pval != val)
                output.append({ prop + "/raw", pval.toQString() });
            if (!gval.isEmpty() && gval != (pval.isEmpty() ? val : pval))
                output.append({ prop + "/get", gval.toQString() });
            if (!sval.isEmpty() && sval != gval)
                output.append({ prop + "/src", sval.toQString() });
            if (!dval.isEmpty() && dval != pval)
                output.append({ prop + "/dev", dval.toQString() });
        }
    } else {
        for (const auto &prop : optionProperties) {
            const ProKey pkey(prop);
            if (!hasValue(pkey)) {
                ret = PropFailRetCode;
                output.append({ prop, QString("**Unknown**") });
            } else {
                output.append({ prop, value(pkey).toQString() });
            }
        }
    }
    printer(output);
    return ret;
}

int QMakeProperty::setProperty(const QStringList &optionProperties)
{
    for (auto it = optionProperties.cbegin(); it != optionProperties.cend(); ++it) {
        QString var = (*it);
        ++it;
        if (it == optionProperties.cend()) {
            return PropFailRetCode;
        }
        if (!var.startsWith("."))
            setValue(var, (*it));
    }
    return PropSuccessRetCode;
}

void QMakeProperty::unsetProperty(const QStringList &optionProperties)
{
    for (auto it = optionProperties.cbegin(); it != optionProperties.cend(); ++it) {
        QString var = (*it);
        if (!var.startsWith("."))
            remove(var);
    }
}

BOBUI_END_NAMESPACE
