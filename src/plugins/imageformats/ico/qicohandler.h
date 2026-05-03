// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
#ifndef BOBUIICOHANDLER_H
#define BOBUIICOHANDLER_H

#include <BobUIGui/QImageIOHandler>

BOBUI_BEGIN_NAMESPACE

class ICOReader;
class BobUIIcoHandler: public QImageIOHandler
{
public:
    BobUIIcoHandler(QIODevice *device);
    virtual ~BobUIIcoHandler();

    bool canRead() const override;
    bool read(QImage *image) override;
    bool write(const QImage &image) override;

    int imageCount() const override;
    bool jumpToImage(int imageNumber) override;
    bool jumpToNextImage() override;

    static bool canRead(QIODevice *device);

    bool supportsOption(ImageOption option) const override;
    QVariant option(ImageOption option) const override;

private:
    int m_currentIconIndex;
    ICOReader *m_pICOReader;
    mutable bool knownCanRead = false;
};

BOBUI_END_NAMESPACE

#endif /* BOBUIICOHANDLER_H */

