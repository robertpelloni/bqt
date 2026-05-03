// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef SINGLE_FUNCTION_KEYWORD_H
#define SINGLE_FUNCTION_KEYWORD_H
#include <QObject>

class SingleFunctionKeywordBeforeReturnType : public QObject
{
    Q_OBJECT
public:
    inline SingleFunctionKeywordBeforeReturnType() {}

    Q_SIGNAL void mySignal();

    Q_SLOT void mySlot() { mySignal(); }
};

class SingleFunctionKeywordBeforeInline : public QObject
{
    Q_OBJECT
public:
    inline SingleFunctionKeywordBeforeInline() {}

    Q_SIGNAL inline void mySignal();

    Q_SLOT inline void mySlot() { emit mySignal(); }
};

class SingleFunctionKeywordAfterInline : public QObject
{
    Q_OBJECT
public:
    inline SingleFunctionKeywordAfterInline() {}

    inline Q_SIGNAL void mySignal();

    inline Q_SLOT void mySlot() { emit mySignal(); }
};

#endif // SINGLE_FUNCTION_KEYWORD_H
