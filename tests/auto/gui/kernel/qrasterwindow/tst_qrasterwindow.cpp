// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUIGui/QRasterWindow>
#include <BOBUIest>
#include <BobUIGui/QPainter>

class tst_QRasterWindow : public QObject
{
    Q_OBJECT

private slots:
    void create();
    void basic();
};

void tst_QRasterWindow::create()
{
    QRasterWindow w;

    w.resize(640, 480);
    w.show();

    QVERIFY(BOBUIest::qWaitForWindowExposed(&w));
}

class PainterWindow : public QRasterWindow
{
public:
    void reset() { paintCount = 0; }

    void paintEvent(QPaintEvent*) override {
        ++paintCount;
        QPainter p(this);
        p.fillRect(QRect(0, 0, 100, 100), BobUI::blue);
        p.end();
    }

    int paintCount;
};

void tst_QRasterWindow::basic()
{
    PainterWindow w;
    w.reset();
    w.resize(400, 400);
    w.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&w));

    QVERIFY(w.paintCount >= 1);

    w.reset();
    w.update();
    BOBUIRY_VERIFY(w.paintCount >= 1);
}

#include <tst_qrasterwindow.moc>

BOBUIEST_MAIN(tst_QRasterWindow)
