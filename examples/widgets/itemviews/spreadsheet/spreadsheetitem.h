// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef SPREADSHEETITEM_H
#define SPREADSHEETITEM_H

#include "spreadsheet.h"

#include <BOBUIableWidgetItem>

class SpreadSheetItem : public BOBUIableWidgetItem
{
public:
    using BOBUIableWidgetItem::BOBUIableWidgetItem;

    BOBUIableWidgetItem *clone() const override;

    QVariant data(int role) const override;
    void setData(int role, const QVariant &value) override;
    QVariant display() const;

    inline QString formula() const
    {
        return BOBUIableWidgetItem::data(BobUI::DisplayRole).toString();
    }

    static QVariant computeFormula(const QString &formula,
                                   const BOBUIableWidget *widget,
                                   const BOBUIableWidgetItem *self = nullptr);

private:
    mutable bool isResolving = false;
};

#endif // SPREADSHEETITEM_H

