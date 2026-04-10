// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qstyleditemdelegate.h"

#include <qabstractitemmodel.h>
#include <qapplication.h>
#include <qbrush.h>
#if BOBUI_CONFIG(lineedit)
#include <qlineedit.h>
#endif
#if BOBUI_CONFIG(textedit)
#include <bobuiextedit.h>
#include <qplaintextedit.h>
#endif
#include <qpainter.h>
#include <qpalette.h>
#include <qpoint.h>
#include <qrect.h>
#include <qsize.h>
#include <qstyle.h>
#include <qdatetime.h>
#include <qstyleoption.h>
#include <qevent.h>
#include <qpixmap.h>
#include <qbitmap.h>
#include <qpixmapcache.h>
#include <qitemeditorfactory.h>
#include <private/qitemeditorfactory_p.h>
#include <qmetaobject.h>
#include <bobuiextlayout.h>
#include <private/qabstractitemdelegate_p.h>
#include <private/qabstractitemmodel_p.h>
#include <private/bobuiextengine_p.h>
#include <private/qlayoutengine_p.h>
#include <qdebug.h>
#include <qlocale.h>

#include <array>
#include <limits.h>

BOBUI_BEGIN_NAMESPACE

class QStyledItemDelegatePrivate : public QAbstractItemDelegatePrivate
{
    Q_DECLARE_PUBLIC(QStyledItemDelegate)

public:
    QStyledItemDelegatePrivate() : factory(nullptr) { }

    static const QWidget *widget(const QStyleOptionViewItem &option)
    {
        return option.widget;
    }

    const QItemEditorFactory *editorFactory() const
    {
        return factory ? factory : QItemEditorFactory::defaultFactory();
    }

    QItemEditorFactory *factory;

    mutable std::array<QModelRoleData, 7> modelRoleData = {
        QModelRoleData(BobUI::FontRole),
        QModelRoleData(BobUI::TextAlignmentRole),
        QModelRoleData(BobUI::ForegroundRole),
        QModelRoleData(BobUI::CheckStateRole),
        QModelRoleData(BobUI::DecorationRole),
        QModelRoleData(BobUI::DisplayRole),
        QModelRoleData(BobUI::BackgroundRole)
    };
};

/*!
    \class QStyledItemDelegate

    \brief The QStyledItemDelegate class provides display and editing facilities for
    data items from a model.

    \ingroup model-view
    \inmodule BobUIWidgets

    When displaying data from models in BobUI item views, e.g., a
    BOBUIableView, the individual items are drawn by a delegate. Also,
    when an item is edited, it provides an editor widget, which is
    placed on top of the item view while editing takes place.
    QStyledItemDelegate is the default delegate for all BobUI item
    views, and is installed upon them when they are created.

    The QStyledItemDelegate class is one of the \l{Model/View Classes}
    and is part of BobUI's \l{Model/View Programming}{model/view
    framework}. The delegate allows the display and editing of items
    to be developed independently from the model and view.

    The data of items in models are assigned an
    \l{BobUI::}{ItemDataRole}; each item can store a QVariant for each
    role. QStyledItemDelegate implements display and editing for the
    most common datatypes expected by users, including booleans,
    integers, and strings.

    The data will be drawn differently depending on which role they
    have in the model. The following table describes the roles and the
    data types the delegate can handle for each of them. It is often
    sufficient to ensure that the model returns appropriate data for
    each of the roles to determine the appearance of items in views.

    \table
    \header \li Role \li Accepted Types
    \omit
    \row    \li \l BobUI::AccessibleDescriptionRole \li QString
    \row    \li \l BobUI::AccessibleTextRole \li QString
    \endomit
    \row    \li \l BobUI::BackgroundRole \li QBrush
    \row    \li \l BobUI::CheckStateRole \li BobUI::CheckState
    \row    \li \l BobUI::DecorationRole \li QIcon, QPixmap, QImage and QColor
    \row    \li \l BobUI::DisplayRole \li QString and types with a string representation
    \row    \li \l BobUI::EditRole \li See QItemEditorFactory for details
    \row    \li \l BobUI::FontRole \li QFont
    \row    \li \l BobUI::SizeHintRole \li QSize
    \omit
    \row    \li \l BobUI::StatusTipRole \li
    \endomit
    \row    \li \l BobUI::TextAlignmentRole \li BobUI::Alignment
    \row    \li \l BobUI::ForegroundRole \li QBrush
    \omit
    \row    \li \l BobUI::ToolTipRole
    \row    \li \l BobUI::WhatsThisRole
    \endomit
    \endtable

    Editors are created with a QItemEditorFactory; a default static
    instance provided by QItemEditorFactory is installed on all item
    delegates. You can set a custom factory using
    setItemEditorFactory() or set a new default factory with
    QItemEditorFactory::setDefaultFactory().

    \snippet code/src_gui_itemviews_qitemeditorfactory.cpp setDefaultFactory

    After the new factory has been set, all standard item delegates
    will use it (i.e, also delegates that were created before the new
    default factory was set).

    It is the data stored in the item model with the \l{BobUI::}{EditRole}
    that is edited. See the QItemEditorFactory class for a more
    high-level introduction to item editor factories.

    \section1 Subclassing QStyledItemDelegate

    If the delegate does not support painting of the data types you
    need or you want to customize the drawing of items, you need to
    subclass QStyledItemDelegate, and reimplement paint() and possibly
    sizeHint(). The paint() function is called individually for each
    item, and with sizeHint(), you can specify the hint for each
    of them.

    When reimplementing paint(), one would typically handle the
    datatypes one would like to draw and use the superclass
    implementation for other types.

    The painting of check box indicators are performed by the current
    style. The style also specifies the size and the bounding
    rectangles in which to draw the data for the different data roles.
    The bounding rectangle of the item itself is also calculated by
    the style. When drawing already supported datatypes, it is
    therefore a good idea to ask the style for these bounding
    rectangles. The QStyle class description describes this in
    more detail.

    If you wish to change any of the bounding rectangles calculated by
    the style or the painting of check box indicators, you can
    subclass QStyle. Note, however, that the size of the items can
    also be affected by reimplementing sizeHint().

    It is possible for a custom delegate to provide editors
    without the use of an editor item factory. In this case, the
    following virtual functions must be reimplemented:

    \list
        \li createEditor() returns the widget used to change data from the model
           and can be reimplemented to customize editing behavior.
        \li setEditorData() provides the widget with data to manipulate.
        \li updateEditorGeometry() ensures that the editor is displayed correctly
           with respect to the item view.
        \li setModelData() returns updated data to the model.
    \endlist

    The \l{Star Delegate Example}{Star Delegate} example creates
    editors by reimplementing these methods.

    \section1 QStyledItemDelegate vs. QItemDelegate

    Since BobUI 4.4, there are two delegate classes: QItemDelegate and
    QStyledItemDelegate. However, the default delegate is QStyledItemDelegate.
    These two classes are independent alternatives to painting and providing
    editors for items in views. The difference between them is that
    QStyledItemDelegate uses the current style to paint its items. We therefore
    recommend using QStyledItemDelegate as the base class when implementing
    custom delegates or when working with BobUI style sheets. The code required
    for either class should be equal unless the custom delegate needs to use
    the style for drawing.

    If you wish to customize the painting of item views, you should
    implement a custom style. Please see the QStyle class
    documentation for details.

    \sa {Delegate Classes}, QItemDelegate, QAbstractItemDelegate, QStyle,
        {Star Delegate Example}
*/


/*!
    Constructs an item delegate with the given \a parent.
*/
QStyledItemDelegate::QStyledItemDelegate(QObject *parent)
    : QAbstractItemDelegate(*new QStyledItemDelegatePrivate(), parent)
{
}

/*!
    Destroys the item delegate.
*/
QStyledItemDelegate::~QStyledItemDelegate()
{
}

/*!
    This function returns the string that the delegate will use to display the
    BobUI::DisplayRole of the model in \a locale. \a value is the value of the BobUI::DisplayRole
    provided by the model.

    The default implementation uses the QLocale::toString to convert \a value into
    a QString.

    This function is not called for empty model indices, i.e., indices for which
    the model returns an invalid QVariant.

    \sa QAbstractItemModel::data()
*/
QString QStyledItemDelegate::displayText(const QVariant &value, const QLocale& locale) const
{
    return d_func()->textForRole(BobUI::DisplayRole, value, locale);
}

/*!
    Initialize \a option with the values using the index \a index. This method
    is useful for subclasses when they need a QStyleOptionViewItem, but don't want
    to fill in all the information themselves.

    \sa QStyleOption::initFrom()
*/
void QStyledItemDelegate::initStyleOption(QStyleOptionViewItem *option,
                                         const QModelIndex &index) const
{
    option->index = index;

    Q_D(const QStyledItemDelegate);
    QModelRoleDataSpan modelRoleDataSpan = d->modelRoleData;
    index.multiData(modelRoleDataSpan);

    const QVariant *value;
    value = modelRoleDataSpan.dataForRole(BobUI::FontRole);
    if (value->isValid() && !value->isNull()) {
        option->font = qvariant_cast<QFont>(*value).resolve(option->font);
        option->fontMetrics = QFontMetrics(option->font);
    }

    value = modelRoleDataSpan.dataForRole(BobUI::TextAlignmentRole);
    if (value->isValid() && !value->isNull())
        option->displayAlignment = BobUIPrivate::legacyFlagValueFromModelData<BobUI::Alignment>(*value);

    value = modelRoleDataSpan.dataForRole(BobUI::ForegroundRole);
    if (value->canConvert<QBrush>())
        option->palette.setBrush(QPalette::Text, qvariant_cast<QBrush>(*value));

    value = modelRoleDataSpan.dataForRole(BobUI::CheckStateRole);
    if (value->isValid() && !value->isNull()) {
        option->features |= QStyleOptionViewItem::HasCheckIndicator;
        option->checkState = BobUIPrivate::legacyEnumValueFromModelData<BobUI::CheckState>(*value);
    }

    value = modelRoleDataSpan.dataForRole(BobUI::DecorationRole);
    if (value->isValid() && !value->isNull()) {
        option->features |= QStyleOptionViewItem::HasDecoration;
        switch (value->userType()) {
        case QMetaType::QIcon: {
            option->icon = qvariant_cast<QIcon>(*value);
            if (option->icon.isNull()) {
                option->features &= ~QStyleOptionViewItem::HasDecoration;
                break;
            }
            QIcon::Mode mode;
            if (!(option->state & QStyle::State_Enabled))
                mode = QIcon::Disabled;
            else if (option->state & QStyle::State_Selected)
                mode = QIcon::Selected;
            else
                mode = QIcon::Normal;
            QIcon::State state = option->state & QStyle::State_Open ? QIcon::On : QIcon::Off;
            QSize actualSize = option->icon.actualSize(option->decorationSize, mode, state);
            // For highdpi icons actualSize might be larger than decorationSize, which we don't want. Clamp it to decorationSize.
            option->decorationSize = QSize(qMin(option->decorationSize.width(), actualSize.width()),
                                           qMin(option->decorationSize.height(), actualSize.height()));
            break;
        }
        case QMetaType::QColor: {
            QPixmap pixmap(option->decorationSize);
            pixmap.fill(qvariant_cast<QColor>(*value));
            option->icon = QIcon(pixmap);
            break;
        }
        case QMetaType::QImage: {
            QImage image = qvariant_cast<QImage>(*value);
            option->icon = QIcon(QPixmap::fromImage(image));
            option->decorationSize = image.deviceIndependentSize().toSize();
            break;
        }
        case QMetaType::QPixmap: {
            QPixmap pixmap = qvariant_cast<QPixmap>(*value);
            option->icon = QIcon(pixmap);
            option->decorationSize = pixmap.deviceIndependentSize().toSize();
            break;
        }
        default:
            break;
        }
    }

    value = modelRoleDataSpan.dataForRole(BobUI::DisplayRole);
    if (value->isValid() && !value->isNull()) {
        option->features |= QStyleOptionViewItem::HasDisplay;
        option->text = displayText(*value, option->locale);
    }

    value = modelRoleDataSpan.dataForRole(BobUI::BackgroundRole);
    option->backgroundBrush = qvariant_cast<QBrush>(*value);

    // disable style animations for checkboxes etc. within itemviews (BOBUIBUG-30146)
    option->styleObject = nullptr;
}

/*!
    Renders the delegate using the given \a painter and style \a option for
    the item specified by \a index.

    This function paints the item using the view's QStyle.

    When reimplementing paint in a subclass. Use the initStyleOption()
    to set up the \a option in the same way as the
    QStyledItemDelegate.

    Whenever possible, use the \a option while painting.
    Especially its \l{QStyleOption::}{rect} variable to decide
    where to draw and its \l{QStyleOption::}{state} to determine
    if it is enabled or selected.

    After painting, you should ensure that the painter is returned to
    the state it was supplied in when this function was called.
    For example, it may be useful to call QPainter::save() before
    painting and QPainter::restore() afterwards.

    \sa QItemDelegate::paint(), QStyle::drawControl(), QStyle::CE_ItemViewItem
*/
void QStyledItemDelegate::paint(QPainter *painter,
        const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_ASSERT(index.isValid());

    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    const QWidget *widget = QStyledItemDelegatePrivate::widget(option);
    QStyle *style = widget ? widget->style() : QApplication::style();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, widget);
}

/*!
    Returns the size needed by the delegate to display the item
    specified by \a index, taking into account the style information
    provided by \a option.

    This function uses the view's QStyle to determine the size of the
    item.

    \sa QStyle::sizeFromContents(), QStyle::CT_ItemViewItem
*/
QSize QStyledItemDelegate::sizeHint(const QStyleOptionViewItem &option,
                                   const QModelIndex &index) const
{
    QVariant value = index.data(BobUI::SizeHintRole);
    if (value.isValid())
        return qvariant_cast<QSize>(value);

    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);
    const QWidget *widget = QStyledItemDelegatePrivate::widget(option);
    QStyle *style = widget ? widget->style() : QApplication::style();
    return style->sizeFromContents(QStyle::CT_ItemViewItem, &opt, QSize(), widget);
}

/*!
    Returns the widget used to edit the item specified by \a index
    for editing. The \a parent widget and style \a option are used to
    control how the editor widget appears.

    \sa QAbstractItemDelegate::createEditor()
*/
QWidget *QStyledItemDelegate::createEditor(QWidget *parent,
                                     const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_D(const QStyledItemDelegate);
    if (!index.isValid())
        return nullptr;
    return d->editorFactory()->createEditor(index.data(BobUI::EditRole).userType(), parent);
}

/*!
    Sets the data to be displayed and edited by the \a editor from the
    data model item specified by the model \a index.

    The default implementation stores the data in the \a editor
    widget's \l {BobUI's Property System} {user property}.

    \sa QMetaProperty::isUser()
*/
void QStyledItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QVariant v = index.data(BobUI::EditRole);
    QByteArray n = editor->metaObject()->userProperty().name();

    if (!n.isEmpty()) {
        if (!v.isValid())
            v = QVariant(editor->property(n).metaType());
        editor->setProperty(n, v);
    }
}

/*!
    Gets data from the \a editor widget and stores it in the specified
    \a model at the item \a index.

    The default implementation gets the value to be stored in the data
    model from the \a editor widget's \l {BobUI's Property System} {user
    property}.

    \sa QMetaProperty::isUser()
*/
void QStyledItemDelegate::setModelData(QWidget *editor,
                                 QAbstractItemModel *model,
                                 const QModelIndex &index) const
{
    Q_D(const QStyledItemDelegate);
    Q_ASSERT(model);
    Q_ASSERT(editor);
    QByteArray n = editor->metaObject()->userProperty().name();
    if (n.isEmpty())
        n = d->editorFactory()->valuePropertyName(
            model->data(index, BobUI::EditRole).userType());
    if (!n.isEmpty())
        model->setData(index, editor->property(n), BobUI::EditRole);
}

/*!
    Updates the \a editor for the item specified by \a index
    according to the style \a option given.
*/
void QStyledItemDelegate::updateEditorGeometry(QWidget *editor,
                                         const QStyleOptionViewItem &option,
                                         const QModelIndex &index) const
{
    if (!editor)
        return;
    Q_ASSERT(index.isValid());
    const QWidget *widget = QStyledItemDelegatePrivate::widget(option);

    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);
    opt.showDecorationSelected = editor->style()->styleHint(QStyle::SH_ItemView_ShowDecorationSelected, nullptr, editor);

    QStyle *style = widget ? widget->style() : QApplication::style();
    QRect geom = style->subElementRect(QStyle::SE_ItemViewItemText, &opt, widget);
    editor->setGeometry(geom);
}

/*!
  Returns the editor factory used by the item delegate.
  If no editor factory is set, the function will return null.

  \sa setItemEditorFactory()
*/
QItemEditorFactory *QStyledItemDelegate::itemEditorFactory() const
{
    Q_D(const QStyledItemDelegate);
    return d->factory;
}

/*!
  Sets the editor factory to be used by the item delegate to be the \a factory
  specified. If no editor factory is set, the item delegate will use the
  default editor factory.

  \sa itemEditorFactory()
*/
void QStyledItemDelegate::setItemEditorFactory(QItemEditorFactory *factory)
{
    Q_D(QStyledItemDelegate);
    d->factory = factory;
}

/*!
    \reimp

    See details in QAbstractItemDelegate::handleEditorEvent().
*/
bool QStyledItemDelegate::eventFilter(QObject *object, QEvent *event)
{
    return handleEditorEvent(object, event);
}

/*!
  \reimp
*/
bool QStyledItemDelegate::editorEvent(QEvent *event,
                                QAbstractItemModel *model,
                                const QStyleOptionViewItem &option,
                                const QModelIndex &index)
{
    Q_ASSERT(event);
    Q_ASSERT(model);

    // make sure that the item is checkable
    BobUI::ItemFlags flags = model->flags(index);
    if (!(flags & BobUI::ItemIsUserCheckable) || !(option.state & QStyle::State_Enabled)
        || !(flags & BobUI::ItemIsEnabled))
        return false;

    // make sure that we have a check state
    QVariant value = index.data(BobUI::CheckStateRole);
    if (!value.isValid())
        return false;

    const QWidget *widget = QStyledItemDelegatePrivate::widget(option);
    QStyle *style = widget ? widget->style() : QApplication::style();

    // make sure that we have the right event type
    if ((event->type() == QEvent::MouseButtonRelease)
        || (event->type() == QEvent::MouseButtonDblClick)
        || (event->type() == QEvent::MouseButtonPress)) {
        QStyleOptionViewItem viewOpt(option);
        initStyleOption(&viewOpt, index);
        QRect checkRect = style->subElementRect(QStyle::SE_ItemViewItemCheckIndicator, &viewOpt, widget);
        QMouseEvent *me = static_cast<QMouseEvent*>(event);
        if (me->button() != BobUI::LeftButton || !checkRect.contains(me->position().toPoint()))
            return false;

        if ((event->type() == QEvent::MouseButtonPress)
            || (event->type() == QEvent::MouseButtonDblClick))
            return true;

    } else if (event->type() == QEvent::KeyPress) {
        if (static_cast<QKeyEvent*>(event)->key() != BobUI::Key_Space
         && static_cast<QKeyEvent*>(event)->key() != BobUI::Key_Select)
            return false;
    } else {
        return false;
    }

    BobUI::CheckState state = BobUIPrivate::legacyEnumValueFromModelData<BobUI::CheckState>(value);
    if (flags & BobUI::ItemIsUserTristate)
        state = ((BobUI::CheckState)((state + 1) % 3));
    else
        state = (state == BobUI::Checked) ? BobUI::Unchecked : BobUI::Checked;
    return model->setData(index, state, BobUI::CheckStateRole);
}

BOBUI_END_NAMESPACE

#include "moc_qstyleditemdelegate.cpp"
