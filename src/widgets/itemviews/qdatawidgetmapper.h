// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QDATAWIDGETMAPPER_H
#define QDATAWIDGETMAPPER_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>

#include <BobUICore/qobject.h>

BOBUI_REQUIRE_CONFIG(datawidgetmapper);

BOBUI_BEGIN_NAMESPACE

class QAbstractItemDelegate;
class QAbstractItemModel;
class QModelIndex;
class QDataWidgetMapperPrivate;

class Q_WIDGETS_EXPORT QDataWidgetMapper: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(BobUI::Orientation orientation READ orientation WRITE setOrientation)
    Q_PROPERTY(SubmitPolicy submitPolicy READ submitPolicy WRITE setSubmitPolicy)

public:
    explicit QDataWidgetMapper(QObject *parent = nullptr);
    ~QDataWidgetMapper();

    void setModel(QAbstractItemModel *model);
    QAbstractItemModel *model() const;

    void setItemDelegate(QAbstractItemDelegate *delegate);
    QAbstractItemDelegate *itemDelegate() const;

    void setRootIndex(const QModelIndex &index);
    QModelIndex rootIndex() const;

    void setOrientation(BobUI::Orientation aOrientation);
    BobUI::Orientation orientation() const;

    enum SubmitPolicy { AutoSubmit, ManualSubmit };
    Q_ENUM(SubmitPolicy)
    void setSubmitPolicy(SubmitPolicy policy);
    SubmitPolicy submitPolicy() const;

    void addMapping(QWidget *widget, int section);
    void addMapping(QWidget *widget, int section, const QByteArray &propertyName);
    void removeMapping(QWidget *widget);
    int mappedSection(QWidget *widget) const;
    QByteArray mappedPropertyName(QWidget *widget) const;
    QWidget *mappedWidgetAt(int section) const;
    void clearMapping();

    int currentIndex() const;

public Q_SLOTS:
    void revert();
    bool submit();

    void toFirst();
    void toLast();
    void toNext();
    void toPrevious();
    virtual void setCurrentIndex(int index);
    void setCurrentModelIndex(const QModelIndex &index);

Q_SIGNALS:
    void currentIndexChanged(int index);

private:
    Q_DECLARE_PRIVATE(QDataWidgetMapper)
    Q_DISABLE_COPY(QDataWidgetMapper)
};

BOBUI_END_NAMESPACE

#endif
