// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:critical reason:data-parser

#include "BobUIGui/qimage.h"
#include "bobuiexturefiledata_p.h"
#include <BobUICore/qsize.h>
#include <BobUICore/qvarlengtharray.h>
#include <BobUICore/qmap.h>

BOBUI_BEGIN_NAMESPACE

Q_LOGGING_CATEGORY(lcBobUIGuiTextureIO, "bobui.gui.textureio");

constexpr size_t MAX_FACES = 6;

class BOBUIextureFileDataPrivate : public QSharedData
{
public:
    BOBUIextureFileDataPrivate()
    {
    }

    BOBUIextureFileDataPrivate(const BOBUIextureFileDataPrivate &other)
        : QSharedData(other),
          mode(other.mode),
          logName(other.logName),
          data(other.data),
          offsets(other.offsets),
          lengths(other.lengths),
          images(other.images),
          size(other.size),
          format(other.format),
          numFaces(other.numFaces),
          numLevels(other.numLevels),
          keyValues(other.keyValues)
    {
    }

    ~BOBUIextureFileDataPrivate()
    {
    }

    void ensureSize(int levels, int faces, bool force = false)
    {
        numLevels = force ? levels : qMax(numLevels, levels);
        numFaces = force ? faces : qMax(numFaces, faces);
        if (mode == BOBUIextureFileData::ByteArrayMode) {
            offsets.resize(numFaces);
            lengths.resize(numFaces);

            for (auto faceList : { &offsets, &lengths })
                for (auto &levelList : *faceList)
                    levelList.resize(numLevels);
        } else {
            images.resize(numFaces);
            for (auto &levelList : images)
                levelList.resize(numLevels);
        }
    }

    bool isValid(int level, int face) const { return level < numLevels && face < numFaces; }

    int getOffset(int level, int face) const { return offsets[face][level]; }
    void setOffset(int value, int level, int face) { offsets[face][level] = value; }
    int getLength(int level, int face) const { return lengths[face][level]; }
    void setLength(int value, int level, int face) { lengths[face][level] = value; }

    BOBUIextureFileData::Mode mode = BOBUIextureFileData::ByteArrayMode;
    QByteArray logName;
    QByteArray data;
    QVarLengthArray<QList<int>, MAX_FACES> offsets; // [Face][Level] = offset
    QVarLengthArray<QList<int>, MAX_FACES> lengths; // [Face][Level] = length
    QVarLengthArray<QList<QImage>, MAX_FACES> images; // [Face][Level] = length
    QSize size;
    quint32 format = 0;
    quint32 internalFormat = 0;
    quint32 baseInternalFormat = 0;
    int numFaces = 0;
    int numLevels = 0;
    QMap<QByteArray, QByteArray> keyValues;
};

BOBUIextureFileData::BOBUIextureFileData(Mode mode)
{
    d = new BOBUIextureFileDataPrivate;
    d->mode = mode;
}

BOBUIextureFileData::BOBUIextureFileData(const BOBUIextureFileData &other)
    : d(other.d)
{
}

BOBUIextureFileData &BOBUIextureFileData::operator=(const BOBUIextureFileData &other)
{
    d = other.d;
    return *this;
}

BOBUIextureFileData::~BOBUIextureFileData()
{
}

bool BOBUIextureFileData::isNull() const
{
    return !d;
}

bool BOBUIextureFileData::isValid() const
{
    if (!d)
        return false;

    if (d->mode == ImageMode)
        return true; // Manually populated: the caller needs to do verification at that time.

    if (d->data.isEmpty() || d->size.isEmpty() || (!d->format && !d->internalFormat))
        return false;

    const int numFacesOffset = d->offsets.size();
    const int numFacesLength = d->lengths.size();
    if (numFacesOffset == 0 || numFacesLength == 0 || d->numFaces != numFacesOffset
        || d->numFaces != numFacesLength)
        return false;

    const qint64 dataSize = d->data.size();

    // Go through all faces and levels and check that the range is inside the data size.
    for (int face = 0; face < d->numFaces; face++) {
        const int numLevelsOffset = d->offsets.at(face).size();
        const int numLevelsLength = d->lengths.at(face).size();
        if (numLevelsOffset == 0 || numLevelsLength == 0 || d->numLevels != numLevelsOffset
            || d->numLevels != numLevelsLength)
            return false;

        for (int level = 0; level < d->numLevels; level++) {
            const qint64 offset = d->getOffset(level, face);
            const qint64 length = d->getLength(level, face);
            if (offset < 0 || offset >= dataSize || length <= 0 || (offset + length > dataSize))
                return false;
        }
    }
    return true;
}

void BOBUIextureFileData::clear()
{
    d = nullptr;
}

QByteArray BOBUIextureFileData::data() const
{
    return d ? d->data : QByteArray();
}

void BOBUIextureFileData::setData(const QByteArray &data)
{
    Q_ASSERT(d->mode == ByteArrayMode);
    d->data = data;
}

void BOBUIextureFileData::setData(const QImage &image, int level, int face)
{
    Q_ASSERT(d->mode == ImageMode);
    d->ensureSize(level + 1, face + 1);
    d->images[face][level] = image;
}

int BOBUIextureFileData::dataOffset(int level, int face) const
{
    Q_ASSERT(d->mode == ByteArrayMode);
    return (d && d->isValid(level, face)) ? d->getOffset(level, face) : 0;
}

void BOBUIextureFileData::setDataOffset(int offset, int level, int face)
{
    Q_ASSERT(d->mode == ByteArrayMode);
    if (d.constData() && level >= 0) {
        d->ensureSize(level + 1, face + 1);
        d->setOffset(offset, level, face);
    }
}

int BOBUIextureFileData::dataLength(int level, int face) const
{
    Q_ASSERT(d->mode == ByteArrayMode);
    return (d && d->isValid(level, face)) ? d->getLength(level, face) : 0;
}

QByteArrayView BOBUIextureFileData::getDataView(int level, int face) const
{
    if (d->mode == ByteArrayMode) {
        const int dataLength = this->dataLength(level, face);
        const int dataOffset = this->dataOffset(level, face);

        if (d == nullptr || dataLength == 0)
            return QByteArrayView();

        return QByteArrayView(d->data.constData() + dataOffset, dataLength);
    } else {
        if (!d->isValid(level, face))
            return QByteArrayView();
        const QImage &img = d->images[face][level];
        return img.isNull() ? QByteArrayView() : QByteArrayView(img.constBits(), img.sizeInBytes());
    }
}

void BOBUIextureFileData::setDataLength(int length, int level, int face)
{
    Q_ASSERT(d->mode == ByteArrayMode);
    if (d.constData() && level >= 0) {
        d->ensureSize(level + 1, face + 1);
        d->setLength(length, level, face);
    }
}

int BOBUIextureFileData::numLevels() const
{
    return d ? d->numLevels : 0;
}

void BOBUIextureFileData::setNumLevels(int numLevels)
{
    if (d && numLevels >= 0)
        d->ensureSize(numLevels, d->numFaces, true);
}

int BOBUIextureFileData::numFaces() const
{
    return d ? d->numFaces : 0;
}

void BOBUIextureFileData::setNumFaces(int numFaces)
{
    if (d && numFaces >= 0)
        d->ensureSize(d->numLevels, numFaces, true);
}

QSize BOBUIextureFileData::size() const
{
    return d ? d->size : QSize();
}

void BOBUIextureFileData::setSize(const QSize &size)
{
    if (d.constData())
        d->size = size;
}

quint32 BOBUIextureFileData::glFormat() const
{
    return d ? d->format : 0;
}

void BOBUIextureFileData::setGLFormat(quint32 format)
{
    if (d.constData())
        d->format = format;
}

quint32 BOBUIextureFileData::glInternalFormat() const
{
    return d ? d->internalFormat : 0;
}

void BOBUIextureFileData::setGLInternalFormat(quint32 format)
{
    if (d.constData())
        d->internalFormat = format;
}

quint32 BOBUIextureFileData::glBaseInternalFormat() const
{
    return d ? d->baseInternalFormat : 0;
}

void BOBUIextureFileData::setGLBaseInternalFormat(quint32 format)
{
    if (d.constData())
        d->baseInternalFormat = format;
}

QByteArray BOBUIextureFileData::logName() const
{
    return d ? d->logName : QByteArray();
}

void BOBUIextureFileData::setLogName(const QByteArray &name)
{
    if (d.constData())
        d->logName = name;
}

QMap<QByteArray, QByteArray> BOBUIextureFileData::keyValueMetadata() const
{
    return d ? d->keyValues : QMap<QByteArray, QByteArray>();
}

void BOBUIextureFileData::setKeyValueMetadata(const QMap<QByteArray, QByteArray> &keyValues)
{
    if (d)
        d->keyValues = keyValues;
}

static QByteArray glFormatName(quint32 fmt)
{
    return QByteArray("0x" + QByteArray::number(fmt, 16).rightJustified(4, '0'));
}

QDebug operator<<(QDebug dbg, const BOBUIextureFileData &d)
{
    QDebugStateSaver saver(dbg);

    dbg.nospace() << "BOBUIextureFileData(";
    if (!d.isNull()) {
        dbg.space() << d.logName() << d.size();
        dbg << "glFormat:" << glFormatName(d.glFormat());
        dbg << "glInternalFormat:" << glFormatName(d.glInternalFormat());
        dbg << "glBaseInternalFormat:" << glFormatName(d.glBaseInternalFormat());
        dbg.nospace() << "Levels: " << d.numLevels();
        dbg.nospace() << "Faces: " << d.numFaces();
        if (!d.isValid())
            dbg << " {Invalid}";
        dbg << ")";
        dbg << (d.d->mode ? "[bytearray-based]" : "[image-based]");
    } else {
        dbg << "null)";
    }

    return dbg;
}

BOBUI_END_NAMESPACE
