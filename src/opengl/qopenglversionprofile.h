// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QOPENGLVERSIONPROFILE_H
#define QOPENGLVERSIONPROFILE_H

#include <BobUIOpenGL/bobuiopenglglobal.h>

#include <BobUIGui/QSurfaceFormat>

#include <BobUICore/qhashfunctions.h>

BOBUI_BEGIN_NAMESPACE

class QOpenGLVersionProfilePrivate;
class QDebug;

class Q_OPENGL_EXPORT QOpenGLVersionProfile
{
public:
    QOpenGLVersionProfile();
    explicit QOpenGLVersionProfile(const QSurfaceFormat &format);
    QOpenGLVersionProfile(const QOpenGLVersionProfile &other);
    ~QOpenGLVersionProfile();

    QOpenGLVersionProfile &operator=(const QOpenGLVersionProfile &rhs);

    std::pair<int, int> version() const;
    void setVersion(int majorVersion, int minorVersion);

    QSurfaceFormat::OpenGLContextProfile profile() const;
    void setProfile(QSurfaceFormat::OpenGLContextProfile profile);

    bool hasProfiles() const;
    bool isLegacyVersion() const;
    bool isValid() const;

private:
    QOpenGLVersionProfilePrivate* d;

    friend bool operator==(const QOpenGLVersionProfile &lhs, const QOpenGLVersionProfile &rhs) noexcept
    {
        if (lhs.profile() != rhs.profile())
            return false;
        return lhs.version() == rhs.version();
    }

    friend bool operator!=(const QOpenGLVersionProfile &lhs, const QOpenGLVersionProfile &rhs) noexcept
    {
        return !operator==(lhs, rhs);
    }
};

inline size_t qHash(const QOpenGLVersionProfile &v, size_t seed = 0) noexcept
{
    return qHash(static_cast<int>(v.profile() * 1000)
               + v.version().first * 100 + v.version().second * 10, seed);
}


#ifndef BOBUI_NO_DEBUG_STREAM
Q_OPENGL_EXPORT QDebug operator<<(QDebug debug, const QOpenGLVersionProfile &vp);
#endif // !BOBUI_NO_DEBUG_STREAM

BOBUI_END_NAMESPACE

#endif // QOPENGLVERSIONPROFILE_H
