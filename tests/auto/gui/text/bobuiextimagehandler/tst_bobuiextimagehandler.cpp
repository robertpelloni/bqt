// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>

#include <QPainter>
#include <private/bobuiextimagehandler_p.h>

using namespace BobUI::StringLiterals;

// #define DEBUG_WRITE_HTML

class tst_BOBUIextImageHandler : public QObject
{
    Q_OBJECT

public:
    tst_BOBUIextImageHandler();

private slots:
    void init();
    void cleanup();
    void cleanupTestCase();
    void loadAtNImages_data();
#ifndef BOBUI_NO_TEXTHTMLPARSER
    void loadAtNImages();
    void customResourceSchema_data();
    void customResourceSchema();
    void maxWidth_data();
    void maxWidth();
#endif
};

struct MyTextDocument : public BOBUIextDocument
{
    QVariant loadResource(int, const QUrl &name) override
    {
        retrievedScheme = name.scheme();
        return {};
    }
    QString retrievedScheme;
};

tst_BOBUIextImageHandler::tst_BOBUIextImageHandler()
{
}

void tst_BOBUIextImageHandler::init()
{
}

void tst_BOBUIextImageHandler::cleanup()
{
}

void tst_BOBUIextImageHandler::cleanupTestCase()
{
}

void tst_BOBUIextImageHandler::loadAtNImages_data()
{
    BOBUIest::addColumn<QString>("imageFile");

    BOBUIest::addRow("file") << QFINDTESTDATA("data/image.png");
    BOBUIest::addRow("file_url") << QUrl::fromLocalFile(QFINDTESTDATA("data/image.png")).toString();
    BOBUIest::addRow("resource") << ":/data/image.png";
    BOBUIest::addRow("qrc_url") << "qrc:/data/image.png";
}

#ifndef BOBUI_NO_TEXTHTMLPARSER
void tst_BOBUIextImageHandler::loadAtNImages()
{
    QFETCH(QString, imageFile);

    BOBUIextDocument doc;
    BOBUIextCursor c(&doc);
    c.insertHtml("<img src=\"" + imageFile + "\">");
    const auto formats = doc.allFormats();
    const auto it = std::find_if(formats.begin(), formats.end(), [](const auto &format){
        return format.objectType() == BOBUIextFormat::ImageObject;
    });
    QCOMPARE_NE(it, formats.end());
    const BOBUIextImageFormat format = (*it).toImageFormat();
    BOBUIextImageHandler handler;

    for (const auto &dpr : {1, 2}) {
        QImage img(20, 20, QImage::Format_ARGB32_Premultiplied);
        img.fill(BobUI::white);
        img.setDevicePixelRatio(dpr);
        QPainter p(&img);
        handler.drawObject(&p, QRect(0, 0, 20, 20), &doc, 0, format);
        p.end();
        QVERIFY(!img.isNull());
        const auto expectedColor = dpr == 1 ? BobUI::red : BobUI::green;
        QCOMPARE(img.pixelColor(0, 0), expectedColor);
    }
}

void tst_BOBUIextImageHandler::customResourceSchema_data()
{
    BOBUIest::addColumn<QString>("imageFile");
    BOBUIest::addColumn<QString>("expectedScheme");

    BOBUIest::addRow("file_url") << QUrl::fromLocalFile(QFINDTESTDATA("data/image.png")).toString() << "file";
    BOBUIest::addRow("resource") << ":/data/image.png" << "qrc";
    BOBUIest::addRow("qrc_url") << "qrc:/data/image.png" << "qrc";
    BOBUIest::addRow("custom_url") << "custom:/data/image.png" << "custom";
}

void tst_BOBUIextImageHandler::customResourceSchema()
{
    QFETCH(QString, imageFile);
    QFETCH(QString, expectedScheme);

    MyTextDocument doc;

    BOBUIextCursor c(&doc);
    c.insertHtml("<img src=\"" + imageFile + "\">");
    const auto formats = doc.allFormats();
    const auto it = std::find_if(formats.begin(), formats.end(), [](const auto &format){
        return format.objectType() == BOBUIextFormat::ImageObject;
    });
    QCOMPARE_NE(it, formats.end());
    QImage img(20, 20, QImage::Format_ARGB32_Premultiplied);
    img.fill(BobUI::white);
    QPainter p(&img);
    BOBUIextImageHandler handler;
    handler.drawObject(&p, QRect(0, 0, 20, 20), &doc, 0, *it);
    p.end();
    QCOMPARE(expectedScheme, doc.retrievedScheme);
}

void tst_BOBUIextImageHandler::maxWidth_data()
{
    BOBUIest::addColumn<QString>("imageFile");
    BOBUIest::addColumn<QSizeF>("pageSize");
    BOBUIest::addColumn<BOBUIextLength>("maxWidth");
    BOBUIest::addColumn<QSizeF>("expectedSize");

    BOBUIest::addRow("constrained-percentage") << QFINDTESTDATA("data/image.png") << QSizeF(16, 16) << BOBUIextLength(BOBUIextLength::PercentageLength, 100) << QSizeF(12, 12);
    BOBUIest::addRow("not-constrained-percentage") << QFINDTESTDATA("data/image.png") << QSizeF(200, 200) << BOBUIextLength(BOBUIextLength::PercentageLength, 100) << QSizeF(16, 16);
    BOBUIest::addRow("constrained-fixed") << QFINDTESTDATA("data/image.png") << QSizeF(16, 16) << BOBUIextLength(BOBUIextLength::FixedLength, 5) << QSizeF(5, 5);
    BOBUIest::addRow("not-constrained-fixed") << QFINDTESTDATA("data/image.png") << QSizeF(200, 200) << BOBUIextLength(BOBUIextLength::FixedLength, 5) << QSizeF(5, 5);
    BOBUIest::addRow("not-constrained-default") << QFINDTESTDATA("data/image.png") << QSizeF(200, 200) << BOBUIextLength(BOBUIextLength::VariableLength, 5) << QSizeF(16, 16);
}

void tst_BOBUIextImageHandler::maxWidth()
{
    QFETCH(QString, imageFile);
    QFETCH(QSizeF, pageSize);
    QFETCH(BOBUIextLength, maxWidth);
    QFETCH(QSizeF, expectedSize);

    BOBUIextDocument doc;
    doc.setPageSize(pageSize);
    doc.setDocumentMargin(2);
    BOBUIextCursor c(&doc);
    QString style;
    if (maxWidth.type() == BOBUIextLength::PercentageLength)
        style = " style=\"max-width:"_L1 + QString::number(maxWidth.rawValue()) + "%;\""_L1;
    else if (maxWidth.type() == BOBUIextLength::FixedLength)
        style = " style=\"max-width:"_L1 + QString::number(maxWidth.rawValue()) + "px;\""_L1;
    const QString html = "<img src=\"" + imageFile + u'\"' + style + "\">";
    c.insertHtml(html);

#ifdef DEBUG_WRITE_HTML
    {
        QFile out("/tmp/" + QLatin1String(BOBUIest::currentDataTag()) + ".html");
        out.open(QFile::WriteOnly);
        out.write(html.toLatin1());
        out.close();
    }
    {
        QFile out("/tmp/" + QLatin1String(BOBUIest::currentDataTag()) + "_rewrite.html");
        out.open(QFile::WriteOnly);
        out.write(doc.toHtml().toLatin1());
        out.close();
    }
#endif
    const auto formats = doc.allFormats();
    const auto it = std::find_if(formats.begin(), formats.end(), [](const auto &format){
        return format.objectType() == BOBUIextFormat::ImageObject;
    });
    QCOMPARE_NE(it, formats.end());
    const BOBUIextImageFormat format = (*it).toImageFormat();
    BOBUIextImageHandler handler;

    QCOMPARE(handler.intrinsicSize(&doc, 0, format), expectedSize);
}
#endif

BOBUIEST_MAIN(tst_BOBUIextImageHandler)
#include "tst_bobuiextimagehandler.moc"
