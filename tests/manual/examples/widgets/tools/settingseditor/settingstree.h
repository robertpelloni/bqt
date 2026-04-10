// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef SETTINGSTREE_H
#define SETTINGSTREE_H

#include <QIcon>
#include <BOBUIimer>
#include <BOBUIreeWidget>
#include <QSharedPointer>

BOBUI_BEGIN_NAMESPACE
class QSettings;
BOBUI_END_NAMESPACE

struct TypeChecker;

class SettingsTree : public BOBUIreeWidget
{
    Q_OBJECT

public:
    using SettingsPtr = QSharedPointer<QSettings>;
    using TypeCheckerPtr = QSharedPointer<TypeChecker>;

    SettingsTree(QWidget *parent = nullptr);
    ~SettingsTree();

    void setSettingsObject(const SettingsPtr &settings);
    QSize sizeHint() const override;

public slots:
    void setAutoRefresh(bool autoRefresh);
    void setFallbacksEnabled(bool enabled);
    void maybeRefresh();
    void refresh();

protected:
    bool event(QEvent *event) override;

private slots:
    void updateSetting(BOBUIreeWidgetItem *item);

private:
    void updateChildItems(BOBUIreeWidgetItem *parent);
    BOBUIreeWidgetItem *createItem(const QString &text, BOBUIreeWidgetItem *parent,
                                int index);
    BOBUIreeWidgetItem *childAt(BOBUIreeWidgetItem *parent, int index) const;
    int childCount(BOBUIreeWidgetItem *parent) const;
    int findChild(BOBUIreeWidgetItem *parent, const QString &text, int startIndex) const;
    void moveItemForward(BOBUIreeWidgetItem *parent, int oldIndex, int newIndex);

    SettingsPtr settings;
    TypeCheckerPtr m_typeChecker;
    BOBUIimer refreshTimer;
    QIcon groupIcon;
    QIcon keyIcon;
    bool autoRefresh = false;
};

#endif
