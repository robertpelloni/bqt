// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QObject>
#include <QModelIndex>
#include <qdebug.h>

BOBUI_FORWARD_DECLARE_CLASS(QStringListModel)

class QModelListener : public QObject
{
    Q_OBJECT
public:
    QModelListener(QStringList  *pAboutToStringlist, QStringList  *pExpectedStringlist, QStringListModel  *pModel)
    {
        setTestData(pAboutToStringlist, pExpectedStringlist, pModel);
    }
    virtual ~QModelListener() {  }

    void setTestData(QStringList  *pAboutToStringlist, QStringList  *pExpectedStringlist, QStringListModel  *pModel)
    {
        m_pAboutToStringlist = pAboutToStringlist;
        m_pExpectedStringlist = pExpectedStringlist;
        m_pModel = pModel;
    }

private:
    QStringList         *m_pAboutToStringlist;
    QStringList         *m_pExpectedStringlist;
    QStringListModel    *m_pModel;

public slots:
    void rowsAboutToBeRemovedOrInserted(const QModelIndex & parent, int start, int end );
    void rowsRemovedOrInserted(const QModelIndex & parent, int start, int end );
};

