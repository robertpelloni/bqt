// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <QFileIconProvider>
#include <QFileInfo>
#include <BOBUIest>

class tst_QFileIconProvider : public QObject
{
    Q_OBJECT

private slots:
    void qfileiconprovider();

    void iconType_data();
    void iconType();

    void iconInfo_data();
    void iconInfo();

    void type_data();
    void type();

    void iconPixmaps();
};

void tst_QFileIconProvider::qfileiconprovider()
{
    // don't crash
    QFileIconProvider provider;
}

Q_DECLARE_METATYPE(QFileIconProvider::IconType)
void tst_QFileIconProvider::iconType_data()
{
    BOBUIest::addColumn<QFileIconProvider::IconType>("type");

    BOBUIest::newRow("computer") << QFileIconProvider::Computer;
    BOBUIest::newRow("desktop") << QFileIconProvider::Desktop;
    BOBUIest::newRow("trashcan") << QFileIconProvider::Trashcan;
    BOBUIest::newRow("network") << QFileIconProvider::Network;
    BOBUIest::newRow("drive") << QFileIconProvider::Drive;
    BOBUIest::newRow("folder") << QFileIconProvider::Folder;
    BOBUIest::newRow("file") << QFileIconProvider::File;
}

// public QIcon icon(QFileIconProvider::IconType const& type) const
void tst_QFileIconProvider::iconType()
{
    QGuiApplication::setDesktopSettingsAware(false);
    QFETCH(QFileIconProvider::IconType, type);
    QFileIconProvider provider;
    QVERIFY(!provider.icon(type).isNull());
}

void tst_QFileIconProvider::iconInfo_data()
{
    BOBUIest::addColumn<QFileInfo>("info");
    BOBUIest::addColumn<bool>("setPath");

    BOBUIest::newRow("null") << QFileInfo() << false;
    BOBUIest::newRow("drive") << QFileInfo(QDir::rootPath()) << true;
    BOBUIest::newRow("home") << QFileInfo(QDir::homePath()) << true;
    BOBUIest::newRow("current") << QFileInfo(QDir::currentPath()) << true;
}

// public QIcon icon(QFileInfo const& info) const
void tst_QFileIconProvider::iconInfo()
{
    QFETCH(QFileInfo, info);
    QFETCH(bool, setPath);

    if (setPath)
        QVERIFY(info.exists());
    QFileIconProvider provider;
    // we should always get an icon
    QVERIFY(!provider.icon(info).isNull());
}

void tst_QFileIconProvider::type_data()
{
    BOBUIest::addColumn<QFileInfo>("info");
    // Return value is _very_ system dependent, hard to test
    // BOBUIest::addColumn<QString>("type");

    BOBUIest::newRow("null") << QFileInfo();
    BOBUIest::newRow("drive") << QFileInfo(QDir::rootPath());
    BOBUIest::newRow("home") << QFileInfo(QDir::homePath());
    BOBUIest::newRow("current") << QFileInfo(QDir::currentPath());
    BOBUIest::newRow("exe") << QFileInfo(QCoreApplication::applicationFilePath());
}

// public QString type(QFileInfo const& info) const
void tst_QFileIconProvider::type()
{
    QFETCH(QFileInfo, info);
    QFileIconProvider provider;
    QVERIFY(!provider.type(info).isEmpty());
}

static QIcon getIcon()
{
    QFileIconProvider fip;
    return fip.icon(QFileInfo(QDir::currentPath()));
}

void tst_QFileIconProvider::iconPixmaps()
{
    const QIcon &icon = getIcon();
    const auto sizes = icon.availableSizes();
    for (const QSize &size : sizes) {
        QPixmap pixmap = icon.pixmap(size);
        QVERIFY(!pixmap.isNull());
    }
}

BOBUIEST_MAIN(tst_QFileIconProvider)
#include "tst_qfileiconprovider.moc"

