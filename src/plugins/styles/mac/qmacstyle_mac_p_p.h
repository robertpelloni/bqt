// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QMACSTYLE_MAC_P_P_H
#define QMACSTYLE_MAC_P_P_H

#include "qmacstyle_mac_p.h"

#include <BobUICore/qhash.h>
#include <BobUICore/qlist.h>
#include <BobUICore/qmap.h>
#include <BobUICore/qmath.h>
#include <BobUICore/qpointer.h>
#include <BobUICore/bobuiextstream.h>

#include <BobUIGui/private/qpainter_p.h>

#include <BobUIGui/qbitmap.h>
#include <BobUIGui/qevent.h>
#include <BobUIGui/qpaintdevice.h>
#include <BobUIGui/qpainter.h>
#include <BobUIGui/qpixmapcache.h>

#include <BobUIWidgets/private/qapplication_p.h>
#include <BobUIWidgets/private/qcommonstyle_p.h>
#include <BobUIWidgets/private/qstylehelper_p.h>

#include <BobUIWidgets/qapplication.h>
#include <BobUIWidgets/qfocusframe.h>
#include <BobUIWidgets/qformlayout.h>
#include <BobUIWidgets/qlayout.h>
#include <BobUIWidgets/qstyleoption.h>
#include <BobUIWidgets/bobuiextedit.h>

#if BOBUI_CONFIG(checkbox)
#include <BobUIWidgets/qcheckbox.h>
#endif
#if BOBUI_CONFIG(combobox)
#include <BobUIWidgets/private/qcombobox_p.h>
#include <BobUIWidgets/qcombobox.h>
#endif
#if BOBUI_CONFIG(datetimeedit)
#include <BobUIWidgets/qdatetimeedit.h>
#endif
#if BOBUI_CONFIG(dialogbuttonbox)
#include <BobUIWidgets/qdialogbuttonbox.h>
#endif
#if BOBUI_CONFIG(dockwidget)
#include <BobUIWidgets/qdockwidget.h>
#endif
#if BOBUI_CONFIG(graphicsview)
#include <BobUIWidgets/qgraphicsproxywidget.h>
#include <BobUIWidgets/qgraphicsview.h>
#endif
#if BOBUI_CONFIG(groupbox)
#include <BobUIWidgets/qgroupbox.h>
#endif
#if BOBUI_CONFIG(itemviews)
#include <BobUIWidgets/qheaderview.h>
#endif
#if BOBUI_CONFIG(lineedit)
#include <BobUIWidgets/qlineedit.h>
#endif
#if BOBUI_CONFIG(listview)
#include <BobUIWidgets/qlistview.h>
#endif
#if BOBUI_CONFIG(mainwindow)
#include <BobUIWidgets/qmainwindow.h>
#endif
#if BOBUI_CONFIG(menubar)
#include <BobUIWidgets/qmenubar.h>
#endif
#if BOBUI_CONFIG(progressbar)
#include <BobUIWidgets/qprogressbar.h>
#endif
#if BOBUI_CONFIG(pushbutton)
#include <BobUIWidgets/qpushbutton.h>
#endif
#include <BobUIWidgets/qradiobutton.h>
#if BOBUI_CONFIG(rubberband)
#include <BobUIWidgets/qrubberband.h>
#endif
#if BOBUI_CONFIG(sizegrip)
#include <BobUIWidgets/qsizegrip.h>
#endif
#if BOBUI_CONFIG(spinbox)
#include <BobUIWidgets/qspinbox.h>
#endif
#if BOBUI_CONFIG(splitter)
#include <BobUIWidgets/qsplitter.h>
#endif
#if BOBUI_CONFIG(tableview)
#include <BobUIWidgets/bobuiableview.h>
#endif
#if BOBUI_CONFIG(toolbar)
#include <BobUIWidgets/bobuioolbar.h>
#endif
#if BOBUI_CONFIG(toolbutton)
#include <BobUIWidgets/bobuioolbutton.h>
#endif
#if BOBUI_CONFIG(treeview)
#include <BobUIWidgets/bobuireeview.h>
#endif

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

Q_FORWARD_DECLARE_MUTABLE_CG_TYPE(CGContext);

Q_FORWARD_DECLARE_OBJC_CLASS(NSView);
Q_FORWARD_DECLARE_OBJC_CLASS(NSCell);

BOBUI_BEGIN_NAMESPACE

/*
    AHIG:
        macOS Human Interface Guidelines
        https://developer.apple.com/macos/human-interface-guidelines/overview/themes/

    Builder:
        Interface Builder in Xcode 8 or later
*/

// this works as long as we have at most 16 different control types
#define CT1(c) CT2(c, c)
#define CT2(c1, c2) ((uint(c1) << 16) | uint(c2))

#define SIZE(large, small, mini) \
    (controlSize == QStyleHelper::SizeLarge ? (large) : controlSize == QStyleHelper::SizeSmall ? (small) : (mini))

// same as return SIZE(...) but optimized
#define return_SIZE(large, small, mini) \
    do { \
        static const int sizes[] = { (large), (small), (mini) }; \
        return sizes[controlSize]; \
    } while (false)

class QMacStylePrivate : public QCommonStylePrivate
{
    Q_DECLARE_PUBLIC(QMacStyle)
public:
    enum Direction {
        North, South, East, West
    };

    enum CocoaControlType {
        NoControl,    // For when there's no such a control in Cocoa
        Box,          // QGroupBox
        Box_Dark,     // FIXME See render code in drawPrimitive(PE_FrameTabWidget)
        Button_CheckBox,
        Button_Disclosure,  // Disclosure triangle, like in BOBUIreeView
        Button_PopupButton,  // Non-editable QComboBox
        Button_PullDown, // QPushButton with menu
        Button_PushButton, // Plain QPushButton and BOBUIabBar buttons
        Button_RadioButton,
        Button_SquareButton, // Oversized QPushButton
        Button_WindowClose,
        Button_WindowMiniaturize,
        Button_WindowZoom,
        ComboBox,     // Editable QComboBox
        ProgressIndicator_Determinate,
        ProgressIndicator_Indeterminate,
        Scroller_Horizontal,
        Scroller_Vertical,
        SegmentedControl_First, // BOBUIabBar buttons focus ring
        SegmentedControl_Middle,
        SegmentedControl_Last,
        SegmentedControl_Single,
        Slider_Horizontal,
        Slider_Vertical,
        SplitView_Horizontal,
        SplitView_Vertical,
        Stepper,      // QSpinBox buttons
        TextField
    };

    struct CocoaControl {
        CocoaControl();
        CocoaControl(CocoaControlType t, QStyleHelper::WidgetSizePolicy s);

        CocoaControlType type;
        QStyleHelper::WidgetSizePolicy size;

        bool operator==(const CocoaControl &other) const;

        QSizeF defaultFrameSize() const;
        QRectF adjustedControlFrame(const QRectF &rect) const;
        QMarginsF titleMargins() const;

        bool getCocoaButtonTypeAndBezelStyle(NSButtonType *buttonType, NSBezelStyle *bezelStyle) const;
    };


    typedef void (^DrawRectBlock)(CGContextRef, const CGRect &);

    QMacStylePrivate();
    ~QMacStylePrivate();

    // Ideally these wouldn't exist, but since they already exist we need some accessors.
    static const int PushButtonLeftOffset;
    static const int PushButtonRightOffset;
    static const int PushButtonContentPadding;

    enum Animates { AquaPushButton, AquaProgressBar, AquaListViewItemOpen, AquaScrollBar };
    QStyleHelper::WidgetSizePolicy aquaSizeConstrain(const QStyleOption *option, const QWidget *widg,
                             QStyle::ContentsType ct = QStyle::CT_CustomBase,
                             QSize szHint=QSize(-1, -1), QSize *insz = 0) const;
    QStyleHelper::WidgetSizePolicy effectiveAquaSizeConstrain(const QStyleOption *option, const QWidget *widg,
                             QStyle::ContentsType ct = QStyle::CT_CustomBase,
                             QSize szHint=QSize(-1, -1), QSize *insz = 0) const;
    inline int animateSpeed(Animates) const { return 33; }

    // Utility functions
    static CGRect comboboxInnerBounds(const CGRect &outterBounds, const CocoaControl &cocoaWidget);

    static QRectF comboboxEditBounds(const QRectF &outterBounds, const CocoaControl &cw);

    void setAutoDefaultButton(QObject *button) const;

    NSView *cocoaControl(CocoaControl widget) const;
    NSCell *cocoaCell(CocoaControl widget) const;

    void setupNSGraphicsContext(CGContextRef cg, bool flipped) const;
    void restoreNSGraphicsContext(CGContextRef cg) const;

    void setupVerticalInvertedXform(CGContextRef cg, bool reverse, bool vertical, const CGRect &rect) const;

    void drawNSViewInRect(NSView *view, const QRectF &rect, QPainter *p, __attribute__((noescape)) DrawRectBlock drawRectBlock = nil) const;
    void resolveCurrentNSView(QWindow *window) const;

    void drawProgressBar(QPainter *p, const QStyleOptionProgressBar *pb) const;
    void drawFocusRing(QPainter *p, const QRectF &targetRect, int hMargin, int vMargin, const CocoaControl &cw) const;

    void drawToolbarButtonArrow(const QStyleOption *opt, QPainter *p) const;

    QPainterPath windowPanelPath(const QRectF &r) const;

    CocoaControlType windowButtonCocoaControl(QStyle::SubControl sc) const;

#if BOBUI_CONFIG(tabbar)
    void tabLayout(const QStyleOptionTab *opt, const QWidget *widget, QRect *textRect, QRect *iconRect) const override;
    static Direction tabDirection(BOBUIabBar::Shape shape);
    static bool verticalTabs(QMacStylePrivate::Direction tabDirection);
#endif

public:
    mutable QPointer<QObject> autoDefaultButton;
    static  QList<QPointer<QObject> > scrollBars;

    mutable QPointer<QFocusFrame> focusWidget;
    mutable NSView *backingStoreNSView;
    mutable QHash<CocoaControl, NSView *> cocoaControls;
    mutable QHash<CocoaControl, NSCell *> cocoaCells;

    std::optional<QFont> smallSystemFont;

};

BOBUI_END_NAMESPACE

#endif // QMACSTYLE_MAC_P_P_H
