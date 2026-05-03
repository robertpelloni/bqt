// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <bobuiest.h>
#include <BobUICore/qvariant.h>

#define ITERATION_COUNT 1e5

class tst_QGuiVariant : public QObject
{
    Q_OBJECT

public:
    tst_QGuiVariant();
    virtual ~tst_QGuiVariant();

private slots:
    void createGuiType_data();
    void createGuiType();
    void createGuiTypeCopy_data();
    void createGuiTypeCopy();
};

tst_QGuiVariant::tst_QGuiVariant()
{
}

tst_QGuiVariant::~tst_QGuiVariant()
{
}

void tst_QGuiVariant::createGuiType_data()
{
    BOBUIest::addColumn<int>("typeId");
    for (int i = QMetaType::FirstGuiType; i <= QMetaType::LastGuiType; ++i) {
        if (QMetaType metaType(i); metaType.isValid())
            BOBUIest::newRow(metaType.name()) << i;
    }
}

// Tests how fast a BobUI GUI type can be default-constructed by a
// QVariant. The purpose of this benchmark is to measure the overhead
// of creating (and destroying) a QVariant compared to creating the
// type directly.
void tst_QGuiVariant::createGuiType()
{
    QFETCH(int, typeId);
    QBENCHMARK {
        for (int i = 0; i < ITERATION_COUNT; ++i)
            QVariant(QMetaType(typeId));
    }
}

void tst_QGuiVariant::createGuiTypeCopy_data()
{
    createGuiType_data();
}

// Tests how fast a BobUI GUI type can be copy-constructed by a
// QVariant. The purpose of this benchmark is to measure the overhead
// of creating (and destroying) a QVariant compared to creating the
// type directly.
void tst_QGuiVariant::createGuiTypeCopy()
{
    QFETCH(int, typeId);
    QVariant other((QMetaType(typeId)));
    const void *copy = other.constData();
    QBENCHMARK {
        for (int i = 0; i < ITERATION_COUNT; ++i)
            QVariant(QMetaType(typeId), copy);
    }
}

BOBUIEST_MAIN(tst_QGuiVariant)
#include "tst_qguivariant.moc"
