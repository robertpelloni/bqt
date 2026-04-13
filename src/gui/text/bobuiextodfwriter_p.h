// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIEXTODFWRITER_H
#define BOBUIEXTODFWRITER_H

#include <BobUIGui/private/bobuiguiglobal_p.h>

#ifndef BOBUI_NO_TEXTODFWRITER

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API. It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUICore/qhash.h>
#include <BobUICore/qlist.h>
#include <BobUICore/qset.h>
#include <BobUICore/qstack.h>
#include <BobUICore/QXmlStreamWriter>

#include "bobuiextdocument_p.h"
#include "bobuiextdocumentwriter.h"

BOBUI_BEGIN_NAMESPACE

class BOBUIextDocumentPrivate;
class BOBUIextCursor;
class BOBUIextBlock;
class QIODevice;
class QXmlStreamWriter;
class BOBUIextOdfWriterPrivate;
class BOBUIextBlockFormat;
class BOBUIextCharFormat;
class BOBUIextListFormat;
class BOBUIextFrameFormat;
class BOBUIextTableCellFormat;
class BOBUIextFrame;
class BOBUIextFragment;
class QOutputStrategy;

class Q_AUTOTEST_EXPORT BOBUIextOdfWriter {
public:
    BOBUIextOdfWriter(const BOBUIextDocument &document, QIODevice *device);
    bool writeAll();

    void setCreateArchive(bool on) { m_createArchive = on; }
    bool createArchive() const { return m_createArchive; }

    void writeBlock(QXmlStreamWriter &writer, const BOBUIextBlock &block);
    void writeFormats(QXmlStreamWriter &writer, const QSet<int> &formatIds) const;
    void writeBlockFormat(QXmlStreamWriter &writer, BOBUIextBlockFormat format, int formatIndex) const;
    void writeCharacterFormat(QXmlStreamWriter &writer, BOBUIextCharFormat format, int formatIndex) const;
    void writeListFormat(QXmlStreamWriter &writer, BOBUIextListFormat format, int formatIndex) const;
    void writeFrameFormat(QXmlStreamWriter &writer, BOBUIextFrameFormat format, int formatIndex) const;
    void writeTableFormat(QXmlStreamWriter &writer, BOBUIextTableFormat format, int formatIndex) const;
    void writeTableCellFormat(QXmlStreamWriter &writer, BOBUIextTableCellFormat format,
                              int formatIndex, QList<BOBUIextFormat> &styles) const;
    void writeFrame(QXmlStreamWriter &writer, const BOBUIextFrame *frame);
    void writeInlineCharacter(QXmlStreamWriter &writer, const BOBUIextFragment &fragment) const;

    const QString officeNS, textNS, styleNS, foNS, tableNS, drawNS, xlinkNS, svgNS;
    const int defaultImageResolution = 11811; // 11811 dots per meter = (about) 300 dpi

protected:
    void tableCellStyleElement(QXmlStreamWriter &writer, const int &formatIndex,
                               const BOBUIextTableCellFormat &format,
                               bool hasBorder, int tableId = 0,
                               const BOBUIextTableFormat tableFormatTmp = BOBUIextTableFormat()) const;

private:
    const BOBUIextDocument *m_document;
    QIODevice *m_device;

    QOutputStrategy *m_strategy;
    bool m_createArchive;

    QStack<BOBUIextList *> m_listStack;

    QHash<int, QList<int>> m_cellFormatsInTablesWithBorders;
    QSet<int> m_tableFormatsWithBorders;
    mutable QSet<int> m_tableFormatsWithColWidthConstraints;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_TEXTODFWRITER
#endif // BOBUIEXTODFWRITER_H
