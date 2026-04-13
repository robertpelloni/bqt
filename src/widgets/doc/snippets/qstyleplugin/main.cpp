// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <BobUIWidgets>
#include <BobUIGui>

//! [0]
class MyStylePlugin : public QStylePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.bobui-project.BobUI.QStyleFactoryInterface" FILE "mystyleplugin.json")
public:
    MyStylePlugin(QObject *parent = nullptr);

    QStyle *create(const QString &key) override;
    QStringList keys() const;
};
//! [0]

class RocketStyle : public QCommonStyle
{
public:
    RocketStyle() {};

};

class StarBusterStyle : public QCommonStyle
{
public:
    StarBusterStyle() {};
};

MyStylePlugin::MyStylePlugin(QObject *parent)
    : QStylePlugin(parent)
{
}

QStringList MyStylePlugin::keys() const
{
    return QStringList() << "Rocket" << "StarBuster";
}

//! [1]
QStyle *MyStylePlugin::create(const QString &key)
{
    QString lcKey = key.toLower();
    if (lcKey == "rocket") {
        return new RocketStyle;
    } else if (lcKey == "starbuster") {
        return new StarBusterStyle;
    }
    return nullptr;
}
//! [1]

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MyStylePlugin plugin;
    return app.exec();
}
