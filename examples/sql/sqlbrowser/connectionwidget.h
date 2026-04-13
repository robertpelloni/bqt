// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef CONNECTIONWIDGET_H
#define CONNECTIONWIDGET_H

#include <QWidget>

BOBUI_FORWARD_DECLARE_CLASS(BOBUIreeWidget)
BOBUI_FORWARD_DECLARE_CLASS(BOBUIreeWidgetItem)
BOBUI_FORWARD_DECLARE_CLASS(QSqlDatabase)

class ConnectionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConnectionWidget(QWidget *parent = nullptr);
    ~ConnectionWidget() override;

    QSqlDatabase currentDatabase() const;

signals:
    void tableActivated(const QString &table);
    void metaDataRequested(const QString &tableName);

public slots:
    void refresh();
    void showMetaData();
    void onItemActivated(BOBUIreeWidgetItem *item);
    void onCurrentItemChanged(BOBUIreeWidgetItem *current);

private:
    void setActive(BOBUIreeWidgetItem *);

    BOBUIreeWidget *tree;
    QAction *metaDataAction;
    QString activeDb;
};

#endif
