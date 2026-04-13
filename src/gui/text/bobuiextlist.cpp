// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only


#include "bobuiextlist.h"
#include "bobuiextobject_p.h"
#include "bobuiextcursor.h"
#include "bobuiextdocument_p.h"
#include <qdebug.h>

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

class BOBUIextListPrivate : public BOBUIextBlockGroupPrivate
{
public:
    BOBUIextListPrivate(BOBUIextDocument *doc)
        : BOBUIextBlockGroupPrivate(doc)
    {
    }
};

/*!
    \class BOBUIextList
    \reentrant

    \brief The BOBUIextList class provides a decorated list of items in a BOBUIextDocument.
    \inmodule BobUIGui

    \ingroup richtext-processing

    A list contains a sequence of text blocks, each of which is marked with a
    bullet point or other symbol. Multiple levels of lists can be used, and
    the automatic numbering feature provides support for ordered numeric and
    alphabetical lists.

    Lists are created by using a text cursor to insert an empty list at the
    current position or by moving existing text into a new list.
    The \l{BOBUIextCursor::insertList()} function inserts an empty block into the
    document at the cursor position, and makes it the first item in a list.

    \snippet textdocument-lists/mainwindow.cpp 0

    The \l{BOBUIextCursor::createList()} function takes the contents of the
    cursor's current block and turns it into the first item of a new list.

    The cursor's current list is found with \l{BOBUIextCursor::currentList()}.

    The number of items in a list is given by count(). Each item can be
    obtained by its index in the list with the item() function. Similarly,
    the index of a given item can be found with itemNumber(). The text of
    each item can be found with the itemText() function.

    Note that the items in the list may not be adjacent elements in the
    document. For example, the top-level items in a multi-level list will
    be separated by the items in lower levels of the list.

    List items can be deleted by index with the removeItem() function.
    remove() deletes the specified item in the list.

    The list's format is set with setFormat() and read with format().
    The format describes the decoration of the list itself, and not the
    individual items.

    \sa BOBUIextBlock, BOBUIextListFormat, BOBUIextCursor
*/

/*! \internal
 */
BOBUIextList::BOBUIextList(BOBUIextDocument *doc)
    : BOBUIextBlockGroup(*new BOBUIextListPrivate(doc), doc)
{
}

/*!
  \internal
*/
BOBUIextList::~BOBUIextList()
{
}

/*!
    Returns the number of items in the list.
*/
int BOBUIextList::count() const
{
    Q_D(const BOBUIextList);
    return d->blocks.size();
}

/*!
    Returns the \a{i}-th text block in the list.

    \sa count(), itemText()
*/
BOBUIextBlock BOBUIextList::item(int i) const
{
    Q_D(const BOBUIextList);
    if (i < 0 || i >= d->blocks.size())
        return BOBUIextBlock();
    return d->blocks.at(i);
}

/*!
    \fn void BOBUIextList::setFormat(const BOBUIextListFormat &format)

    Sets the list's format to \a format.
*/

/*!
    \fn BOBUIextListFormat BOBUIextList::format() const

    Returns the list's format.
*/

/*!
    \fn int BOBUIextList::itemNumber(const BOBUIextBlock &block) const

    Returns the index of the list item that corresponds to the given \a block.
    Returns -1 if the block was not present in the list.
*/
int BOBUIextList::itemNumber(const BOBUIextBlock &blockIt) const
{
    Q_D(const BOBUIextList);
    return d->blocks.indexOf(blockIt);
}

/*!
    \fn QString BOBUIextList::itemText(const BOBUIextBlock &block) const

    Returns the text of the list item that corresponds to the given \a block.
*/
QString BOBUIextList::itemText(const BOBUIextBlock &blockIt) const
{
    Q_D(const BOBUIextList);
    int item = d->blocks.indexOf(blockIt) + 1;
    if (item <= 0)
        return QString();

    BOBUIextBlock block = d->blocks.at(item-1);
    BOBUIextBlockFormat blockFormat = block.blockFormat();

    QString result;

    const int style = format().style();
    QString numberPrefix;
    QString numberSuffix = u"."_s;

    // the number of the item might be offset by start, which defaults to 1
    const int itemNumber = item + format().start() - 1;

    if (format().hasProperty(BOBUIextFormat::ListNumberPrefix))
        numberPrefix = format().numberPrefix();
    if (format().hasProperty(BOBUIextFormat::ListNumberSuffix))
        numberSuffix = format().numberSuffix();

    switch (style) {
        case BOBUIextListFormat::ListDecimal:
            result = QString::number(itemNumber);
            break;
            // from the old richtext
        case BOBUIextListFormat::ListLowerAlpha:
        case BOBUIextListFormat::ListUpperAlpha:
            {
                // match the html default behavior of falling back to decimal numbers
                if (itemNumber < 1) {
                    result = QString::number(itemNumber);
                    break;
                }

                const char baseChar = style == BOBUIextListFormat::ListUpperAlpha ? 'A' : 'a';

                int c = itemNumber;
                while (c > 0) {
                    c--;
                    result.prepend(QChar::fromUcs2(baseChar + (c % 26)));
                    c /= 26;
                }
            }
            break;
        case BOBUIextListFormat::ListLowerRoman:
        case BOBUIextListFormat::ListUpperRoman:
            {
                // match the html default behavior of falling back to decimal numbers
                if (itemNumber < 1) {
                    result = QString::number(itemNumber);
                } else if (itemNumber < 5000) {
                    QString romanNumeral;

                    // works for up to 4999 items
                    QLatin1StringView romanSymbols;
                    if (style == BOBUIextListFormat::ListLowerRoman)
                        romanSymbols = "iiivixxxlxcccdcmmmm"_L1;
                    else
                        romanSymbols = "IIIVIXXXLXCCCDCMMMM"_L1;

                    int c[] = { 1, 4, 5, 9, 10, 40, 50, 90, 100, 400, 500, 900, 1000 };
                    int n = itemNumber;
                    for (int i = 12; i >= 0; n %= c[i], i--) {
                        int q = n / c[i];
                        if (q > 0) {
                            int startDigit = i + (i+3)/4;
                            int numDigits;
                            if (i % 4) {
                                // c[i] == 4|5|9|40|50|90|400|500|900
                                if ((i-2) % 4) {
                                    // c[i] == 4|9|40|90|400|900 => with subtraction (IV, IX, XL, XC, ...)
                                    numDigits = 2;
                                }
                                else {
                                    // c[i] == 5|50|500 (V, L, D)
                                    numDigits = 1;
                                }
                            }
                            else {
                                // c[i] == 1|10|100|1000 (I, II, III, X, XX, ...)
                                numDigits = q;
                            }

                            romanNumeral.append(romanSymbols.sliced(startDigit, numDigits));
                        }
                    }
                    result = std::move(romanNumeral);
                } else {
                    result = u"?"_s;
                }

            }
            break;
        default:
            Q_ASSERT(false);
    }
    if (blockIt.textDirection() == BobUI::RightToLeft)
        return numberSuffix + result + numberPrefix;
    else
        return numberPrefix + result + numberSuffix;
}

/*!
    Removes the item at item position \a i from the list. When the last item in the
    list is removed, the list is automatically deleted by the BOBUIextDocument that owns
    it.

    \sa add(), remove()
*/
void BOBUIextList::removeItem(int i)
{
    Q_D(BOBUIextList);
    if (i < 0 || i >= d->blocks.size())
        return;

    BOBUIextBlock block = d->blocks.at(i);
    remove(block);
}


/*!
    Removes the given \a block from the list.

    \sa add(), removeItem()
*/
void BOBUIextList::remove(const BOBUIextBlock &block)
{
    BOBUIextBlockFormat fmt = block.blockFormat();
    fmt.setIndent(fmt.indent() + format().indent());
    fmt.setObjectIndex(-1);
    const_cast<BOBUIextDocumentPrivate *>(BOBUIextDocumentPrivate::get(block))->setBlockFormat(block, block, fmt, BOBUIextDocumentPrivate::SetFormat);
}

/*!
    Makes the given \a block part of the list.

    \sa remove(), removeItem()
*/
void BOBUIextList::add(const BOBUIextBlock &block)
{
    BOBUIextBlockFormat fmt = block.blockFormat();
    fmt.setObjectIndex(objectIndex());
    const_cast<BOBUIextDocumentPrivate *>(BOBUIextDocumentPrivate::get(block))->setBlockFormat(block, block, fmt, BOBUIextDocumentPrivate::SetFormat);
}

BOBUI_END_NAMESPACE

#include "moc_bobuiextlist.cpp"
