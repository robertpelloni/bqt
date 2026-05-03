// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUIEXTUREFILEDATA_P_H
#define BOBUIEXTUREFILEDATA_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIGui/bobuiguiglobal.h>
#include <QSharedDataPointer>
#include <QLoggingCategory>
#include <private/qglobal_p.h>

BOBUI_BEGIN_NAMESPACE

class QDebug;

Q_DECLARE_LOGGING_CATEGORY(lcBobUIGuiTextureIO)

class BOBUIextureFileDataPrivate;

class Q_GUI_EXPORT BOBUIextureFileData
{
public:
    enum Mode { ByteArrayMode, ImageMode };

    BOBUIextureFileData(Mode mode = ByteArrayMode);
    BOBUIextureFileData(const BOBUIextureFileData &other);
    BOBUIextureFileData &operator=(const BOBUIextureFileData &other);
    ~BOBUIextureFileData();

    bool isNull() const;
    bool isValid() const;

    void clear();

    QByteArray data() const;
    void setData(const QByteArray &data);
    void setData(const QImage &image, int level = 0, int face = 0);

    int dataOffset(int level = 0, int face = 0) const;
    void setDataOffset(int offset, int level = 0, int face = 0);

    int dataLength(int level = 0, int face = 0) const;
    void setDataLength(int length, int level = 0, int face = 0);

    QByteArrayView getDataView(int level = 0, int face = 0) const;

    int numLevels() const;
    void setNumLevels(int num);

    int numFaces() const;
    void setNumFaces(int num);

    QSize size() const;
    void setSize(const QSize &size);

    quint32 glFormat() const;
    void setGLFormat(quint32 format);

    quint32 glInternalFormat() const;
    void setGLInternalFormat(quint32 format);

    quint32 glBaseInternalFormat() const;
    void setGLBaseInternalFormat(quint32 format);

    QByteArray logName() const;
    void setLogName(const QByteArray &name);

    QMap<QByteArray, QByteArray> keyValueMetadata() const;
    void setKeyValueMetadata(const QMap<QByteArray, QByteArray> &keyValues);

private:
    QSharedDataPointer<BOBUIextureFileDataPrivate> d;
    friend Q_GUI_EXPORT QDebug operator<<(QDebug dbg, const BOBUIextureFileData &d);
};

Q_DECLARE_TYPEINFO(BOBUIextureFileData, Q_RELOCATABLE_TYPE);

Q_GUI_EXPORT QDebug operator<<(QDebug dbg, const BOBUIextureFileData &d);

BOBUI_END_NAMESPACE

#endif // QABSTRACTLAYOUTSTYLEINFO_P_H
