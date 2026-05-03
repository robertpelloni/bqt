// Copyright (C) 2016 Robin Burchell <robin.burchell@viroteck.net>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QDebug>
#include <BOBUIextDocument>
#include <bobuiest.h>

class tst_BOBUIextDocument : public QObject
{
    Q_OBJECT
private slots:
    void mightBeRichText_data();
    void mightBeRichText();
};

void tst_BOBUIextDocument::mightBeRichText_data()
{
    BOBUIest::addColumn<QString>("source");
    BOBUIest::addColumn<bool>("isMaybeRichText");
    BOBUIest::newRow("empty") << QString() << false;
    BOBUIest::newRow("simple") << QString::fromLatin1("<html><b>Foo</b></html>") << true;
    BOBUIest::newRow("simple2") << QString::fromLatin1("<b>Foo</b>") << true;
    BOBUIest::newRow("documentation-header") << QString("<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\n"
                                                     "<!DOCTYPE html\n"
                                                     "    PUBLIC ""-//W3C//DTD XHTML 1.0 Strict//EN\" \"DTD/xhtml1-strict.dtd\">\n"
                                                     "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\" lang=\"en\">")
                                          << true;
    BOBUIest::newRow("br-nospace") << QString("Test <br/> new line") << true;
    BOBUIest::newRow("br-space") << QString("Test <br /> new line") << true;
    BOBUIest::newRow("br-invalidspace") << QString("Test <br/ > new line") << false;
    BOBUIest::newRow("invalid closing tag") << QString("Test <br/ line") << false;
    BOBUIest::newRow("no tags") << QString("Test line") << false;
}

void tst_BOBUIextDocument::mightBeRichText()
{
    QFETCH(QString, source);
    QFETCH(bool, isMaybeRichText);
    QBENCHMARK {
        QCOMPARE(isMaybeRichText, BobUI::mightBeRichText(source));
    }
}

BOBUIEST_MAIN(tst_BOBUIextDocument)

#include "main.moc"
