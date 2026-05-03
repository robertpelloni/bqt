// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QGRAPHICSWIDGET_P_H
#define QGRAPHICSWIDGET_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of other BobUI classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIWidgets/private/bobuiwidgetsglobal_p.h>
#include <private/qobject_p.h>
#include "qgraphicsitem_p.h"
#include "qgraphicswidget.h"
#include <BobUIGui/qfont.h>
#include <BobUIGui/qpalette.h>
#include <BobUIWidgets/qsizepolicy.h>
#include <BobUIWidgets/qstyle.h>

#include <memory>

BOBUI_REQUIRE_CONFIG(graphicsview);

BOBUI_BEGIN_NAMESPACE

class QGraphicsLayout;
class QStyleOptionTitleBar;

class QGraphicsWidgetPrivate : public QGraphicsItemPrivate
{
    Q_DECLARE_PUBLIC(QGraphicsWidget)
public:
    QGraphicsWidgetPrivate();
    virtual ~QGraphicsWidgetPrivate();

    void init(QGraphicsItem *parentItem, BobUI::WindowFlags wFlags);
    qreal titleBarHeight(const QStyleOptionTitleBar &options) const;

    // Margins
    mutable std::unique_ptr<QMarginsF> margins;
    void ensureMargins() const;

    void fixFocusChainBeforeReparenting(QGraphicsWidget *newParent, QGraphicsScene *oldScene, QGraphicsScene *newScene = nullptr);
    void setLayout_helper(QGraphicsLayout *l);

    // Layouts
    QGraphicsLayout *layout;
    void setLayoutDirection_helper(BobUI::LayoutDirection direction);
    void resolveLayoutDirection();

    // Style
    QPalette palette;
    uint inheritedPaletteResolveMask;
    void setPalette_helper(const QPalette &palette);
    void resolvePalette(uint inheritedMask) override;
    void updatePalette(const QPalette &palette);
    QPalette naturalWidgetPalette() const;
    QFont font;
    uint inheritedFontResolveMask;
    void setFont_helper(const QFont &font);
    void resolveFont(uint inheritedMask) override;
    void updateFont(const QFont &font);
    QFont naturalWidgetFont() const;

    // Window specific
    void initStyleOptionTitleBar(QStyleOptionTitleBar *option);
    void adjustWindowFlags(BobUI::WindowFlags *wFlags);
    void windowFrameMouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void windowFrameMousePressEvent(QGraphicsSceneMouseEvent *event);
    void windowFrameMouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void windowFrameHoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void windowFrameHoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    bool hasDecoration() const;

    // Private Properties
    qreal width() const override;
    void setWidth(qreal) override;
    void resetWidth() override;

    qreal height() const override;
    void setHeight(qreal) override;
    void resetHeight() override;
    void setGeometryFromSetPos();

    // State
    inline int attributeToBitIndex(BobUI::WidgetAttribute att) const
    {
        int bit = -1;
        switch (att) {
        case BobUI::WA_SetLayoutDirection: bit = 0; break;
        case BobUI::WA_RightToLeft: bit = 1; break;
        case BobUI::WA_SetStyle: bit = 2; break;
        case BobUI::WA_Resized: bit = 3; break;
        case BobUI::WA_DeleteOnClose: bit = 4; break;
        case BobUI::WA_NoSystemBackground: bit = 5; break;
        case BobUI::WA_OpaquePaintEvent: bit = 6; break;
        case BobUI::WA_SetPalette: bit = 7; break;
        case BobUI::WA_SetFont: bit = 8; break;
        case BobUI::WA_WindowPropagation: bit = 9; break;
        default: break;
        }
        return bit;
    }
    inline void setAttribute(BobUI::WidgetAttribute att, bool value)
    {
        int bit = attributeToBitIndex(att);
        if (bit == -1) {
            qWarning("QGraphicsWidget::setAttribute: unsupported attribute %d", int(att));
            return;
        }
        if (value)
            attributes |= (1 << bit);
        else
            attributes &= ~(1 << bit);
    }
    inline bool testAttribute(BobUI::WidgetAttribute att) const
    {
        int bit = attributeToBitIndex(att);
        if (bit == -1)
            return false;
        return (attributes & (1 << bit)) != 0;
    }
    quint32 attributes : 10;
    quint32 inSetGeometry : 1;
    quint32 polished: 1;
    quint32 inSetPos : 1;
    quint32 autoFillBackground : 1;

    // Focus
    BobUI::FocusPolicy focusPolicy;
    QGraphicsWidget *focusNext;
    QGraphicsWidget *focusPrev;

    // Windows
    BobUI::WindowFlags windowFlags;
    struct WindowData {
        QString windowTitle;
        QStyle::SubControl hoveredSubControl;
        BobUI::WindowFrameSection grabbedSection;
        uint buttonMouseOver : 1;
        uint buttonSunken : 1;
        QRectF startGeometry;
        QRect buttonRect;
        WindowData()
            : hoveredSubControl(QStyle::SC_None)
            , grabbedSection(BobUI::NoSection)
            , buttonMouseOver(false)
            , buttonSunken(false)
        {}
    };
    std::unique_ptr<WindowData> windowData;
    void ensureWindowData();

    bool setWindowFrameMargins;
    mutable std::unique_ptr<QMarginsF> windowFrameMargins;
    void ensureWindowFrameMargins() const;

#ifndef BOBUI_NO_ACTION
    QList<QAction *> actions;
#endif
};

BOBUI_END_NAMESPACE

#endif //QGRAPHICSWIDGET_P_H

