// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#include <BobUIGui>
#include <BobUIWidgets>

// This example show how calling QDialog::exec() shows the dialog,
// but does not return.

class ClickWindow: public QRasterWindow
{
public:
    ClickWindow() {
        qDebug() << "ClickWindow constructor";
    }

    ~ClickWindow() {
        qDebug() << "ClickWindow destructor";
    }

    void paintEvent(QPaintEvent *ev) override {
        QPainter p(this);
        p.fillRect(ev->rect(), QColorConstants::Svg::deepskyblue);
        p.drawText(50, 100, "Application has started. See the developer tools console for debug output");
    }

    void mousePressEvent(QMouseEvent *) override {
        qDebug() << "mousePressEvent(): calling QMessageBox::exec()";

        QMessageBox messageBox;
        messageBox.setText("Hello! This is a message box.");
        connect(&messageBox, &QMessageBox::buttonClicked, [](QAbstractButton *button) {
           qDebug() << "Button Clicked" << button;
        });
        messageBox.exec(); // <-- does not return

        qDebug() << "mousePressEvent(): done"; // <---  will not be printed
    }
};

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    ClickWindow window;
    window.show();

    return app.exec();
}
