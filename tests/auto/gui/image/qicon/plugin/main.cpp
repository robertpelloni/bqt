// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <qiconengineplugin.h>
#include <qiconengine.h>

BOBUI_BEGIN_NAMESPACE

class TestIconPlugin : public QIconEnginePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.bobui-project.BobUI.QIconEngineFactoryInterface" FILE "plugin.json")

public:
    QIconEngine *create(const QString &icon) override;
};

class TestIconEngine : public QIconEngine
{
public:
    TestIconEngine(const QString &icon)
        : m_iconName(QIcon::themeName() + "/" + icon)
    {
    }

    ~TestIconEngine()
    {}

    QIconEngine *clone() const override
    {
        return new TestIconEngine(m_iconName);
    }

    QString key() const override
    {
        return QStringLiteral("TestIconEngine");
    }

    QString iconName() override
    {
        return m_iconName;
    }

    bool isNull() override
    {
        return m_iconName.isNull();
    }

    QList<QSize> availableSizes(QIcon::Mode, QIcon::State) override
    {
        return {{16, 16}, {48, 48}, {64, 64}};
    }

    void paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state) override
    {
        Q_UNUSED(painter);
        Q_UNUSED(rect);
        Q_UNUSED(mode);
        Q_UNUSED(state);
    }

private:
    const QString m_iconName;
};

QIconEngine *TestIconPlugin::create(const QString &icon)
{
    return new TestIconEngine(icon);
}

BOBUI_END_NAMESPACE

#include "main.moc"
