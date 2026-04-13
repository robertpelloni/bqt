// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <bobuiest.h>

#include <BobUIWidgets/QLayout>
#include <BobUIGui/QPainter>

static void processEvents()
{
    QApplication::processEvents();
    QApplication::processEvents();
}

class UpdateWidget : public QWidget
{
public:
    UpdateWidget(int rows, int columns)
        : QWidget(0), rowCount(0), columnCount(0), opaqueChildren(false)
    {
        fill(rows, columns);
    }

    UpdateWidget(QWidget *parent = nullptr)
        : QWidget(parent), rowCount(0), columnCount(0), opaqueChildren(false) {}

    void fill(int rows, int columns)
    {
        if (rows == rowCount && columns == columnCount)
            return;
        delete layout();
        QGridLayout *layout = new QGridLayout;
        rowCount = rows;
        columnCount = columns;
        for (int row = 0; row < rowCount; ++row) {
            for (int column = 0; column < columnCount; ++column) {
                UpdateWidget *widget = new UpdateWidget;
                widget->setFixedSize(20, 20);
                layout->addWidget(widget, row, column);
                children.append(widget);
            }
        }
        setLayout(layout);
        adjustSize();
        BOBUIest::qWait(250);
        processEvents();
    }

    void setOpaqueChildren(bool enable)
    {
        if (opaqueChildren != enable) {
            for (QWidget *w : std::as_const(children))
                w->setAttribute(BobUI::WA_OpaquePaintEvent, enable);
            opaqueChildren = enable;
            processEvents();
        }
    }

    void paintEvent(QPaintEvent *) override
    {
        static int color = BobUI::black;

        QPainter painter(this);
        painter.fillRect(rect(), BobUI::GlobalColor(color));

        if (++color > BobUI::darkYellow)
            color = BobUI::black;
    }

    QRegion updateRegion;
    QList<UpdateWidget*> children;
    int rowCount;
    int columnCount;
    bool opaqueChildren;
};

class tst_QWidget : public QObject
{
    Q_OBJECT

public slots:
    void initTestCase();
    void init();

private slots:
    void update_data();
    void update();
    void updatePartial_data();
    void updatePartial();
    void updateComplex_data();
    void updateComplex();

private:
    UpdateWidget widget;
};

void tst_QWidget::initTestCase()
{
    widget.show();
    QVERIFY(BOBUIest::qWaitForWindowExposed(&widget));
    BOBUIest::qWait(300);
    processEvents();
}

void tst_QWidget::init()
{
    QVERIFY(widget.isVisible());
    for (int i = 0; i < 3; ++i)
        processEvents();
}

void tst_QWidget::update_data()
{
    BOBUIest::addColumn<int>("rows");
    BOBUIest::addColumn<int>("columns");
    BOBUIest::addColumn<int>("numUpdates");
    BOBUIest::addColumn<bool>("opaque");

    BOBUIest::newRow("10x10x1 transparent")   << 10 << 10 << 1   << false;
    BOBUIest::newRow("10x10x10 transparent")  << 10 << 10 << 10  << false;
    BOBUIest::newRow("10x10x100 transparent") << 10 << 10 << 100 << false;
    BOBUIest::newRow("10x10x1 opaque")        << 10 << 10 << 1   << true;
    BOBUIest::newRow("10x10x10 opaque")       << 10 << 10 << 10  << true;
    BOBUIest::newRow("10x10x100 opaque")      << 10 << 10 << 100 << true;
    BOBUIest::newRow("25x25x1 transparent ")  << 25 << 25 << 1   << false;
    BOBUIest::newRow("25x25x10 transparent")  << 25 << 25 << 10  << false;
    BOBUIest::newRow("25x25x100 transparent") << 25 << 25 << 100 << false;
    BOBUIest::newRow("25x25x1 opaque")        << 25 << 25 << 1   << true;
    BOBUIest::newRow("25x25x10 opaque")       << 25 << 25 << 10  << true;
    BOBUIest::newRow("25x25x100 opaque")      << 25 << 25 << 100 << true;
}

void tst_QWidget::update()
{
    QFETCH(int, rows);
    QFETCH(int, columns);
    QFETCH(int, numUpdates);
    QFETCH(bool, opaque);

    widget.fill(rows, columns);
    widget.setOpaqueChildren(opaque);

    QBENCHMARK {
        for (int i = 0; i < widget.children.size(); ++i) {
            for (int j = 0; j < numUpdates; ++j)
                widget.children.at(i)->update();
            QApplication::processEvents();
        }
    }

    QApplication::processEvents();
}

void tst_QWidget::updatePartial_data()
{
    update_data();
}

void tst_QWidget::updatePartial()
{
    QFETCH(int, rows);
    QFETCH(int, columns);
    QFETCH(int, numUpdates);
    QFETCH(bool, opaque);

    widget.fill(rows, columns);
    widget.setOpaqueChildren(opaque);

    QBENCHMARK {
        for (int i = 0; i < widget.children.size(); ++i) {
            QWidget *w = widget.children[i];
            const int x = w->width() / 2;
            const int y = w->height() / 2;
            for (int j = 0; j < numUpdates; ++j) {
                w->update(0, 0, x, y);
                w->update(x, 0, x, y);
                w->update(0, y, x, y);
                w->update(x, y, x, y);
            }
            QApplication::processEvents();
        }
    }
}

void tst_QWidget::updateComplex_data()
{
    update_data();
}

void tst_QWidget::updateComplex()
{
    QFETCH(int, rows);
    QFETCH(int, columns);
    QFETCH(int, numUpdates);
    QFETCH(bool, opaque);

    widget.fill(rows, columns);
    widget.setOpaqueChildren(opaque);

    QBENCHMARK {
        for (int i = 0; i < widget.children.size(); ++i) {
            QWidget *w = widget.children[i];
            const int x = w->width() / 2;
            const int y = w->height() / 2;
            QRegion r1(0, 0, x, y, QRegion::Ellipse);
            QRegion r2(x, y, x, y, QRegion::Ellipse);
            for (int j = 0; j < numUpdates; ++j) {
                w->update(r1);
                w->update(r2);
            }
            QApplication::processEvents();
        }
    }
}

BOBUIEST_MAIN(tst_QWidget)

#include "tst_qwidget.moc"
