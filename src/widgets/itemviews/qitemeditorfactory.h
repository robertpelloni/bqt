// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QITEMEDITORFACTORY_H
#define QITEMEDITORFACTORY_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUICore/qmetaobject.h>
#include <BobUICore/qbytearray.h>
#include <BobUICore/qhash.h>
#include <BobUICore/qvariant.h>

BOBUI_REQUIRE_CONFIG(itemviews);

BOBUI_BEGIN_NAMESPACE

class QWidget;

class Q_WIDGETS_EXPORT QItemEditorCreatorBase
{
public:
    virtual ~QItemEditorCreatorBase();

    virtual QWidget *createWidget(QWidget *parent) const = 0;
    virtual QByteArray valuePropertyName() const = 0;
};

template <class T>
class QItemEditorCreator : public QItemEditorCreatorBase
{
public:
    inline explicit QItemEditorCreator(const QByteArray &valuePropertyName);
    inline QWidget *createWidget(QWidget *parent) const override { return new T(parent); }
    inline QByteArray valuePropertyName() const override { return propertyName; }

private:
    QByteArray propertyName;
};

template <class T>
class QStandardItemEditorCreator: public QItemEditorCreatorBase
{
public:
    inline QStandardItemEditorCreator()
        : propertyName(T::staticMetaObject.userProperty().name())
    {}
    inline QWidget *createWidget(QWidget *parent) const override { return new T(parent); }
    inline QByteArray valuePropertyName() const override { return propertyName; }

private:
    QByteArray propertyName;
};


template <class T>
Q_INLINE_TEMPLATE QItemEditorCreator<T>::QItemEditorCreator(const QByteArray &avaluePropertyName)
    : propertyName(avaluePropertyName) {}

class Q_WIDGETS_EXPORT QItemEditorFactory
{
public:
    inline QItemEditorFactory() {}
    virtual ~QItemEditorFactory();

    virtual QWidget *createEditor(int userType, QWidget *parent) const;
    virtual QByteArray valuePropertyName(int userType) const;

    void registerEditor(int userType, QItemEditorCreatorBase *creator);

    static const QItemEditorFactory *defaultFactory();
    static void setDefaultFactory(QItemEditorFactory *factory);

private:
    QHash<int, QItemEditorCreatorBase *> creatorMap;
};

BOBUI_END_NAMESPACE

#endif // QITEMEDITORFACTORY_H
