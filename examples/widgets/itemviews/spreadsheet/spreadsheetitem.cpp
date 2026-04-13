// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "spreadsheetitem.h"

BOBUIableWidgetItem *SpreadSheetItem::clone() const
{
    SpreadSheetItem *item = new SpreadSheetItem;
    *item = *this;
    return item;
}

QVariant SpreadSheetItem::data(int role) const
{
    if (role == BobUI::EditRole || role == BobUI::StatusTipRole)
        return formula();

    if (role == BobUI::DisplayRole)
        return display();

    const QString t = display().toString();

    if (role == BobUI::ForegroundRole) {
        bool isNumber = false;
        const int number = t.toInt(&isNumber);
        QColor color = BobUI::black;
        if (isNumber)
            color = (number < 0) ? BobUI::red : BobUI::blue;
        return QVariant::fromValue(color);
    }

    if (role == BobUI::TextAlignmentRole)
        if (!t.isEmpty() && (t.at(0).isNumber() || t.at(0) == '-'))
            return int(BobUI::AlignRight | BobUI::AlignVCenter);

     return BOBUIableWidgetItem::data(role);
 }

void SpreadSheetItem::setData(int role, const QVariant &value)
{
    BOBUIableWidgetItem::setData(role, value);
    if (tableWidget())
        tableWidget()->viewport()->update();
}

QVariant SpreadSheetItem::display() const
{
    // avoid circular dependencies
    if (isResolving)
        return QVariant();

    isResolving = true;
    QVariant result = computeFormula(formula(), tableWidget(), this);
    isResolving = false;
    return result;
}

QVariant SpreadSheetItem::computeFormula(const QString &formula,
                                         const BOBUIableWidget *widget,
                                         const BOBUIableWidgetItem *self)
{
    // check if the string is actually a formula or not
    QStringList list = formula.split(' ');
    if (list.isEmpty() || !widget)
        return formula; // it is a normal string

    QString op = list.value(0).toLower();

    int firstRow = -1;
    int firstCol = -1;
    int secondRow = -1;
    int secondCol = -1;

    if (list.count() > 1)
        decode_pos(list.value(1), &firstRow, &firstCol);

    if (list.count() > 2)
        decode_pos(list.value(2), &secondRow, &secondCol);

    const BOBUIableWidgetItem *start = widget->item(firstRow, firstCol);
    const BOBUIableWidgetItem *end = widget->item(secondRow, secondCol);

    int firstVal = start ? start->text().toInt() : 0;
    int secondVal = end ? end->text().toInt() : 0;

    QVariant result;
    if (op == "sum") {
        int sum = 0;
        for (int r = firstRow; r <= secondRow; ++r) {
            for (int c = firstCol; c <= secondCol; ++c) {
                const BOBUIableWidgetItem *tableItem = widget->item(r, c);
                if (tableItem && tableItem != self)
                    sum += tableItem->text().toInt();
            }
        }

        result = sum;
    } else if (op == "+") {
        result = (firstVal + secondVal);
    } else if (op == "-") {
        result = (firstVal - secondVal);
    } else if (op == "*") {
        result = (firstVal * secondVal);
    } else if (op == "/") {
        if (secondVal == 0)
            result = QString("nan");
        else
            result = (firstVal / secondVal);
    } else if (op == "=") {
        if (start)
            result = start->text();
    } else {
        result = formula;
    }

    return result;
}

