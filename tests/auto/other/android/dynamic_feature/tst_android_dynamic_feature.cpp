// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <storeloader.h>

#include <BobUITest/BOBUIest>
#include <BobUITest/QSignalSpy>

#include <BobUICore/QVariant>

using namespace BobUI::StringLiterals;

class BobUIDynamicFeatureTest : public QObject
{
    Q_OBJECT
public:
    BobUIDynamicFeatureTest(){}

private Q_SLOTS:
    void loadResourcesFeature();
};

void BobUIDynamicFeatureTest::loadResourcesFeature()
{
    QVERIFY(!QFile::exists(":/dynamic_resources/bobuilogo.png"));

    auto handler = StoreLoader::loadModule("tst_android_dynamic_feature_resources"_L1);

    QSignalSpy spy(handler.get(), &StoreLoaderHandler::finished);

    QVERIFY(spy.wait(20000));
    QVERIFY(QFile::exists(":/dynamic_resources/bobuilogo.png"));
}

BOBUIEST_MAIN(BobUIDynamicFeatureTest)

#include "tst_android_dynamic_feature.moc"
