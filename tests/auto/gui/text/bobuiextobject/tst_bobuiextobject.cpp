// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>

#include <qcoreapplication.h>
#include <qdebug.h>
#include <bobuiextobject.h>
#include <bobuiextdocument.h>
#ifndef BOBUI_NO_WIDGETS
#include <bobuiextedit.h>
#endif
#include <bobuiextcursor.h>

class tst_BOBUIextObject : public QObject
{
Q_OBJECT

public:
    tst_BOBUIextObject();
    virtual ~tst_BOBUIextObject();

private slots:
#ifndef BOBUI_NO_WIDGETS
    void getSetCheck();
#endif
    void testStandAloneTextObject();
};

tst_BOBUIextObject::tst_BOBUIextObject()
{
}

tst_BOBUIextObject::~tst_BOBUIextObject()
{
}

#ifndef BOBUI_NO_WIDGETS
// Testing get/set functions
void tst_BOBUIextObject::getSetCheck()
{
    BOBUIextEdit edit;
    BOBUIextFrame obj1(edit.document());
    // BOBUIextFrameLayoutData * BOBUIextFrame::layoutData()
    // void BOBUIextFrame::setLayoutData(BOBUIextFrameLayoutData *)
    BOBUIextFrameLayoutData *var1 = new BOBUIextFrameLayoutData;
    obj1.setLayoutData(var1);
    QCOMPARE(var1, obj1.layoutData());
    obj1.setLayoutData((BOBUIextFrameLayoutData *)0);
    QCOMPARE((BOBUIextFrameLayoutData *)0, obj1.layoutData());
    // delete var1; // No delete, since BOBUIextFrame takes ownership

    BOBUIextBlock obj2 = edit.textCursor().block();
    // BOBUIextBlockUserData * BOBUIextBlock::userData()
    // void BOBUIextBlock::setUserData(BOBUIextBlockUserData *)
    BOBUIextBlockUserData *var2 = new BOBUIextBlockUserData;
    obj2.setUserData(var2);
    QCOMPARE(var2, obj2.userData());
    obj2.setUserData((BOBUIextBlockUserData *)0);
    QCOMPARE((BOBUIextBlockUserData *)0, obj2.userData());

    // int BOBUIextBlock::userState()
    // void BOBUIextBlock::setUserState(int)
    obj2.setUserState(0);
    QCOMPARE(0, obj2.userState());
    obj2.setUserState(INT_MIN);
    QCOMPARE(INT_MIN, obj2.userState());
    obj2.setUserState(INT_MAX);
    QCOMPARE(INT_MAX, obj2.userState());
}
#endif

class TestTextObject : public BOBUIextObject
{
public:
    TestTextObject(BOBUIextDocument *document) : BOBUIextObject(document) {}
};

void tst_BOBUIextObject::testStandAloneTextObject()
{
    BOBUIextDocument document;
    TestTextObject textObject(&document);

    QCOMPARE(textObject.document(), &document);
    // don't crash
    textObject.format();
    textObject.formatIndex();
    QCOMPARE(textObject.objectIndex(), -1);
}

BOBUIEST_MAIN(tst_BOBUIextObject)
#include "tst_bobuiextobject.moc"
