// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>

class TopWidget : public QWidget
{
    Q_OBJECT
public:
    TopWidget(QWidget *parent = nullptr) : QWidget(parent)
    {
        QVBoxLayout *layout = new QVBoxLayout;
        QPushButton *button = new QPushButton("Change app icon");
        connect(button, SIGNAL(clicked()), this, SLOT(changeIcon()));
        layout->addWidget(button);
        setLayout(layout);
    }
public slots:
    void changeIcon()
    {
        QPixmap pix(32, 32);
        pix.fill(BobUI::red);
        QIcon i(pix);
        qApp->setWindowIcon(i);
    }
};

#include "main.moc"

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    TopWidget w;
    w.show();
    return a.exec();
}
