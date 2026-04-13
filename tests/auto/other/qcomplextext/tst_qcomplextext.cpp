// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <BobUIGui/BobUIGui>
#include <private/bobuiextengine_p.h>

#include "bidireorderstring.h"

class tst_QComplexText : public QObject
{
Q_OBJECT

private slots:
    void bidiReorderString_data();
    void bidiReorderString();
    void bidiCursor_bobuibug2795();
    void bidiCursor_PDF();
    void bidiCursorMovement_data();
    void bidiCursorMovement();
    void bidiCursorLogicalMovement_data();
    void bidiCursorLogicalMovement();
    void bidiInvalidCursorNoMovement_data();
    void bidiInvalidCursorNoMovement();

    void bidiCharacterTest();
    void bidiTest();

};

void tst_QComplexText::bidiReorderString_data()
{
    BOBUIest::addColumn<QString>("logical");
    BOBUIest::addColumn<QString>("VISUAL");
    BOBUIest::addColumn<int>("basicDir");

    const LV *data = logical_visual;
    while ( data->name ) {
        //next we fill it with data
        BOBUIest::newRow( data->name )
            << QString::fromUtf8( data->logical )
            << QString::fromUtf8( data->visual )
            << (int) data->basicDir;

        BOBUIest::newRow( QByteArray(data->name) + " (doubled)" )
            << (QString::fromUtf8( data->logical ) + QChar(QChar::ParagraphSeparator) + QString::fromUtf8( data->logical ))
            << (QString::fromUtf8( data->visual ) + QChar(QChar::ParagraphSeparator) + QString::fromUtf8( data->visual ))
            << (int) data->basicDir;
        data++;
    }

    QString isolateAndBoundary =  QString(QChar(0x2068 /* DirFSI */)) + QChar(0x1c /* DirB */) + QChar(0x2069 /* DirPDI */);
    BOBUIest::newRow( "isolateAndBoundary" )
        << QString::fromUtf8( data->logical )
        << QString::fromUtf8( data->visual )
        << (int) QChar::DirL;
}

void tst_QComplexText::bidiReorderString()
{
    QFETCH( QString, logical );
    QFETCH( int,  basicDir );

    // replace \n with Unicode newline. The new algorithm ignores \n
    logical.replace(QChar('\n'), QChar(0x2028));

    BOBUIextEngine e(logical, QFont());
    e.option.setTextDirection((QChar::Direction)basicDir == QChar::DirL ? BobUI::LeftToRight : BobUI::RightToLeft);
    e.itemize();
    quint8 levels[256];
    int visualOrder[256];
    int nitems = e.layoutData->items.size();
    int i;
    for (i = 0; i < nitems; ++i) {
        //qDebug("item %d bidiLevel=%d", i,  e.items[i].analysis.bidiLevel);
        levels[i] = e.layoutData->items[i].analysis.bidiLevel;
    }
    e.bidiReorder(nitems, levels, visualOrder);

    QString visual;
    for (i = 0; i < nitems; ++i) {
        QScriptItem &si = e.layoutData->items[visualOrder[i]];
        QString sub = logical.mid(si.position, e.length(visualOrder[i]));
        if (si.analysis.bidiLevel % 2) {
            // reverse sub
            QChar *a = sub.data();
            QChar *b = a + sub.size() - 1;
            while (a < b) {
                QChar tmp = *a;
                *a = *b;
                *b = tmp;
                ++a;
                --b;
            }
            a = (QChar *)sub.unicode();
            b = a + sub.size();
            while (a<b) {
                *a = a->mirroredChar();
                ++a;
            }
        }
        visual += sub;
    }
    // replace Unicode newline back with  \n to compare.
    visual.replace(QChar(0x2028), QChar('\n'));

    BOBUIEST(visual, "VISUAL");
}

void tst_QComplexText::bidiCursor_bobuibug2795()
{
    QString str = QString::fromUtf8("الجزيرة نت");
    BOBUIextLayout l1(str);

    l1.beginLayout();
    l1.setCacheEnabled(true);
    BOBUIextLine line1 = l1.createLine();
    l1.endLayout();

    qreal x1 = line1.cursorToX(0) - line1.cursorToX(str.size());

    str.append("1");
    BOBUIextLayout l2(str);
    l2.setCacheEnabled(true);
    l2.beginLayout();
    BOBUIextLine line2 = l2.createLine();
    l2.endLayout();

    qreal x2 = line2.cursorToX(0) - line2.cursorToX(str.size());

    // The cursor should remain at the same position after a digit is appended
    QCOMPARE(x1, x2);
}

void tst_QComplexText::bidiCursorMovement_data()
{
    BOBUIest::addColumn<QString>("logical");
    BOBUIest::addColumn<int>("basicDir");

    const LV *data = logical_visual;
    while ( data->name ) {
        //next we fill it with data
        BOBUIest::newRow( data->name )
            << QString::fromUtf8( data->logical )
            << (int) data->basicDir;
        data++;
    }
}

void tst_QComplexText::bidiCursorMovement()
{
    QFETCH(QString, logical);
    QFETCH(int,  basicDir);

    BOBUIextLayout layout(logical);
    layout.setCacheEnabled(true);

    BOBUIextOption option = layout.textOption();
    option.setTextDirection(basicDir == QChar::DirL ? BobUI::LeftToRight : BobUI::RightToLeft);
    layout.setTextOption(option);
    layout.setCursorMoveStyle(BobUI::VisualMoveStyle);
    bool moved;
    int oldPos, newPos = 0;
    qreal x, newX;

    layout.beginLayout();
    BOBUIextLine line = layout.createLine();
    layout.endLayout();

    newX = line.cursorToX(0);
    do {
        oldPos = newPos;
        x = newX;
        newX = line.cursorToX(oldPos);
        if (basicDir == QChar::DirL) {
            QVERIFY(newX >= x);
            newPos = layout.rightCursorPosition(oldPos);
        } else
        {
            QVERIFY(newX <= x);
            newPos = layout.leftCursorPosition(oldPos);
        }
        moved = (oldPos != newPos);
    } while (moved);
}

void tst_QComplexText::bidiCursorLogicalMovement_data()
{
    bidiCursorMovement_data();
}

void tst_QComplexText::bidiCursorLogicalMovement()
{
    QFETCH(QString, logical);
    QFETCH(int,  basicDir);

    BOBUIextLayout layout(logical);

    BOBUIextOption option = layout.textOption();
    option.setTextDirection(basicDir == QChar::DirL ? BobUI::LeftToRight : BobUI::RightToLeft);
    layout.setTextOption(option);
    bool moved;
    int oldPos, newPos = 0;

    do {
        oldPos = newPos;
        newPos = layout.nextCursorPosition(oldPos);
        QVERIFY(newPos >= oldPos);
        moved = (oldPos != newPos);
    } while (moved);

    do {
        oldPos = newPos;
        newPos = layout.previousCursorPosition(oldPos);
        QVERIFY(newPos <= oldPos);
        moved = (oldPos != newPos);
    } while (moved);
}

void tst_QComplexText::bidiInvalidCursorNoMovement_data()
{
    bidiCursorMovement_data();
}

void tst_QComplexText::bidiInvalidCursorNoMovement()
{
    QFETCH(QString, logical);
    QFETCH(int,  basicDir);

    BOBUIextLayout layout(logical);

    BOBUIextOption option = layout.textOption();
    option.setTextDirection(basicDir == QChar::DirL ? BobUI::LeftToRight : BobUI::RightToLeft);
    layout.setTextOption(option);

    // visual
    QCOMPARE(layout.rightCursorPosition(-1000), -1000);
    QCOMPARE(layout.rightCursorPosition(1000), 1000);

    QCOMPARE(layout.leftCursorPosition(-1000), -1000);
    QCOMPARE(layout.leftCursorPosition(1000), 1000);

    // logical
    QCOMPARE(layout.nextCursorPosition(-1000), -1000);
    QCOMPARE(layout.nextCursorPosition(1000), 1000);

    QCOMPARE(layout.previousCursorPosition(-1000), -1000);
    QCOMPARE(layout.previousCursorPosition(1000), 1000);
}

void tst_QComplexText::bidiCursor_PDF()
{
    QString str = QString::fromUtf8("\342\200\252hello\342\200\254");
    BOBUIextLayout layout(str);
    layout.setCacheEnabled(true);

    layout.beginLayout();
    BOBUIextLine line = layout.createLine();
    layout.endLayout();

    int size = str.size();

    QVERIFY(line.cursorToX(size) == line.cursorToX(size - 1));
}

static void testBidiString(const QString &data, int paragraphDirection,
                           const QList<int> &resolvedLevels, const QList<int> &visualOrder)
{
    Q_UNUSED(resolvedLevels);

    BOBUIextEngine e(data, QFont());
    BobUI::LayoutDirection pDir = BobUI::LeftToRight;
    if (paragraphDirection == 1)
        pDir = BobUI::RightToLeft;
    else if (paragraphDirection == 2)
        pDir = BobUI::LayoutDirectionAuto;

    e.option.setTextDirection(pDir);
    e.itemize();
    quint8 levels[1024];
    int visual[1024];
    int nitems = e.layoutData->items.size();
    int i;
    for (i = 0; i < nitems; ++i) {
        //qDebug("item %d bidiLevel=%d", i,  e.items[i].analysis.bidiLevel);
        levels[i] = e.layoutData->items[i].analysis.bidiLevel;
    }
    e.bidiReorder(nitems, levels, visual);

    QString visualString;
    for (i = 0; i < nitems; ++i) {
        QScriptItem &si = e.layoutData->items[visual[i]];
        QString sub;
        for (int j = si.position; j < si.position + e.length(visual[i]); ++j) {
            switch (data.at(j).direction()) {
                case QChar::DirLRE:
                case QChar::DirRLE:
                case QChar::DirLRO:
                case QChar::DirRLO:
                case QChar::DirPDF:
                case QChar::DirBN:
                    continue;
                default:
                    break;
            }
            sub += data.at(j);
        }

        // remove explicit embedding characters, as the test data has them removed as well
        sub.remove(QChar(0x202a));
        sub.remove(QChar(0x202b));
        sub.remove(QChar(0x202c));
        sub.remove(QChar(0x202d));
        sub.remove(QChar(0x202e));
        if (si.analysis.bidiLevel % 2) {
            // reverse sub
            QChar *a = sub.data();
            QChar *b = a + sub.size() - 1;
            while (a < b) {
                QChar tmp = *a;
                *a = *b;
                *b = tmp;
                ++a;
                --b;
            }
            a = (QChar *)sub.unicode();
            b = a + sub.size();
//            while (a<b) {
//                *a = a->mirroredChar();
//                ++a;
//            }
        }
        visualString += sub;
    }
    QString expected;
//    qDebug() << "expected visual order";
    for (int i : visualOrder) {
//        qDebug() << "    " << i << hex << data[i].unicode();
        expected.append(data[i]);
    }

    QCOMPARE(visualString, expected);

}

void tst_QComplexText::bidiCharacterTest()
{
    QString testFile = QFINDTESTDATA("data/BidiCharacterTest.txt");
    QFile f(testFile);
    QVERIFY(f.open(QIODevice::ReadOnly));

    int linenum = 0;
    while (!f.atEnd()) {
        linenum++;

        QByteArray line = f.readLine().simplified();
        if (line.startsWith('#') || line.isEmpty())
            continue;
        QVERIFY(!line.contains('#'));

        QList<QByteArray> parts = line.split(';');
        QVERIFY(parts.size() == 5);

        QString data;
        QList<QByteArray> dataParts = parts.at(0).split(' ');
        for (const auto &p : dataParts) {
            bool ok;
            data += QChar((ushort)p.toInt(&ok, 16));
            QVERIFY(ok);
        }

        int paragraphDirection = parts.at(1).toInt();
//        int resolvedParagraphLevel = parts.at(2).toInt();

        QList<int> resolvedLevels;
        QList<QByteArray> levelParts = parts.at(3).split(' ');
        for (const auto &p : levelParts) {
            if (p == "x") {
                resolvedLevels += -1;
            } else {
                bool ok;
                resolvedLevels += p.toInt(&ok);
                QVERIFY(ok);
            }
        }

        QList<int> visualOrder;
        QList<QByteArray> orderParts = parts.at(4).split(' ');
        for (const auto &p : orderParts) {
            bool ok;
            visualOrder += p.toInt(&ok);
            QVERIFY(ok);
        }

        const QByteArray nm = "line #" + QByteArray::number(linenum);

        testBidiString(data, paragraphDirection, resolvedLevels, visualOrder);
    }
}

ushort unicodeForDirection(const QByteArray &direction)
{
    struct {
        const char *string;
        ushort unicode;
    } dirToUnicode[] = {
        { "L", 0x41 },
        { "R", 0x5d0 },
        { "EN", 0x30 },
        { "ES", 0x2b },
        { "ET", 0x24 },
        { "AN", 0x660 },
        { "CS", 0x2c },
        { "B", '\n' },
        { "S", 0x9 },
        { "WS", 0x20 },
        { "ON", 0x2a },
        { "LRE", 0x202a },
        { "LRO", 0x202d },
        { "AL", 0x627 },
        { "RLE", 0x202b },
        { "RLO", 0x202e },
        { "PDF", 0x202c },
        { "NSM", 0x300 },
        { "BN", 0xad },
        { "LRI", 0x2066 },
        { "RLI", 0x2067 },
        { "FSI", 0x2068 },
        { "PDI", 0x2069 }
    };
    for (const auto &e : dirToUnicode) {
        if (e.string == direction)
            return e.unicode;
    }
    Q_UNREACHABLE();
}

void tst_QComplexText::bidiTest()
{
    QString testFile = QFINDTESTDATA("data/BidiTest.txt");
    QFile f(testFile);
    QVERIFY(f.open(QIODevice::ReadOnly));

    int linenum = 0;
    QList<int> resolvedLevels;
    QList<int> visualOrder;
    while (!f.atEnd()) {
        linenum++;

        QByteArray line = f.readLine().simplified();
        if (line.startsWith('#') || line.isEmpty())
            continue;
        QVERIFY(!line.contains('#'));

        if (line.startsWith("@Levels:")) {
            line = line.mid(strlen("@Levels:")).simplified();

            resolvedLevels.clear();
            QList<QByteArray> levelParts = line.split(' ');
            for (const auto &p : levelParts) {
                if (p == "x") {
                    resolvedLevels += -1;
                } else {
                    bool ok;
                    resolvedLevels += p.toInt(&ok);
                    QVERIFY(ok);
                }
            }
            continue;
        } else if (line.startsWith("@Reorder:")) {
            line = line.mid(strlen("@Reorder:")).simplified();

            visualOrder.clear();
            QList<QByteArray> orderParts = line.split(' ');
            for (const auto &p : orderParts) {
                if (p.isEmpty())
                    continue;
                bool ok;
                visualOrder += p.toInt(&ok);
                QVERIFY(ok);
            }
            continue;
        }

        QList<QByteArray> parts = line.split(';');
        Q_ASSERT(parts.size() == 2);

        QString data;
        QList<QByteArray> dataParts = parts.at(0).split(' ');
        for (const auto &p : dataParts) {
            ushort uc = unicodeForDirection(p);
            data += QChar(uc);
        }

        int paragraphDirections = parts.at(1).toInt();

        const QByteArray nm = "line #" + QByteArray::number(linenum);
        if (paragraphDirections & 1)
            testBidiString(data, 2, resolvedLevels, visualOrder);
        if (paragraphDirections & 2)
            testBidiString(data, 0, resolvedLevels, visualOrder);
        if (paragraphDirections & 4)
            testBidiString(data, 1, resolvedLevels, visualOrder);

    }
}



BOBUIEST_MAIN(tst_QComplexText)
#include "tst_qcomplextext.moc"
