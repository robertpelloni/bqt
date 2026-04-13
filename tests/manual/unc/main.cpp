// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QApplication>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>

class Dialog : public QDialog
{
public:
    Dialog()
    {
        QString localFile("test.html");
        // server/shared/test.html should be replaced to point to a real file
        QString UNCPath("file://server/shared/test.html");

        QVBoxLayout* vBox = new QVBoxLayout();
        vBox->addWidget(new QLabel("Clicking on the links should open their"
                                   " contents in the default browser !"));
        vBox->addWidget(createLink(localFile));
        vBox->addWidget(new QLabel("The following link must point to "
                                   "a file in a shared folder on a network !"));
        vBox->addWidget(createLink(UNCPath));
        setLayout(vBox);
    }

protected:
    QLabel* createLink(QString path)
    {
        QLabel *label = new QLabel();
        label->setTextFormat(BobUI::RichText);
        label->setTextInteractionFlags(BobUI::TextBrowserInteraction);
        label->setOpenExternalLinks(true);

        QString link("<a href=" + path + QLatin1Char('>') + path + "</a>");

        label->setText(link);
        return label;
    }
};

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    Dialog dlg;
    dlg.show();

    return app.exec();
}
