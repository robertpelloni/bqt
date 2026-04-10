// Copyright (C) 2016 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QString>
#include <BOBUIest>
#include <QCoreApplication>
#include <QDesktopServices>

class tst_QDesktopServices : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void openUrl();
    void openUrl_data();
};

void tst_QDesktopServices::openUrl_data()
{
    BOBUIest::addColumn<QUrl>("data");
    BOBUIest::addColumn<QString>("message");

    QUrl localFile = QUrl::fromLocalFile(QFINDTESTDATA("test.txt"));

    BOBUIest::newRow("text-file")
            << localFile
            << "This should open test.txt in a text editor";

    localFile.setQuery("x=y");
    BOBUIest::newRow("text-file-with-query")
            << localFile
            << "This should open test.txt in a text editor. Queries do not usually show up.";

    localFile.setQuery(QString());
    localFile.setFragment("top");
    BOBUIest::newRow("text-file-with-fragment")
            << localFile
            << "This should open test.txt in a text editor. Fragments do not usually show up.";

    BOBUIest::newRow("browser-plain")
            << QUrl("http://bobui-project.org")
            << "This should open http://bobui-project.org in the default web browser";

    BOBUIest::newRow("search-url")
            << QUrl("http://google.com/search?q=BobUI+Project")
            << "This should search \"BobUI Project\" on Google";

    BOBUIest::newRow("search-url-with-space")
            << QUrl("http://google.com/search?q=BobUI Project")
            << "This should search \"BobUI Project\" on Google";

    BOBUIest::newRow("search-url-with-quotes")
            << QUrl("http://google.com/search?q=\"BobUI+Project\"")
            << "This should search '\"BobUI Project\"' on Google (including the quotes)";

    BOBUIest::newRow("search-url-with-hashtag")
            << QUrl("http://google.com/search?q=%23bobuiproject")
            << "This should search \"#bobuiproject\" on Google. The # should appear in the Google search field";

    BOBUIest::newRow("search-url-with-fragment")
            << QUrl("http://google.com/search?q=BobUI+Project#top")
            << "This should search \"BobUI Project\" on Google. There should be no # in the Google search field";

    // see BOBUIBUG-32311
    BOBUIest::newRow("search-url-with-slashes")
            << QUrl("http://google.com/search?q=/profile/5")
            << "This should search \"/profile/5\" on Google.";

    // see BOBUIBUG-31945
    BOBUIest::newRow("two-fragments")
            << QUrl("http://developer.apple.com/library/ios/#documentation/uikit/reference/UIViewController_Class/Reference/Reference.html#//apple_ref/doc/uid/TP40006926-CH3-SW81")
            << "This should open \"Implementing a Container View Controller\" in the UIViewController docs";

    BOBUIest::newRow("mail")
            << QUrl("mailto:development@bobui-project.org")
            << "This should open an email composer with the destination set to development@bobui-project.org";

    BOBUIest::newRow("mail-subject")
            << QUrl("mailto:development@bobui-project.org?subject=[Development]%20Test%20Mail")
            << "This should open an email composer and tries to set the subject";
}

void tst_QDesktopServices::openUrl()
{
    QFETCH(QUrl, data);
    QFETCH(QString, message);
    qWarning("\n\nOpening \"%s\": %s", qPrintable(data.toString()), qPrintable(message));
    QVERIFY(QDesktopServices::openUrl(data));
}

BOBUIEST_MAIN(tst_QDesktopServices)

#include "tst_qdesktopservices.moc"
