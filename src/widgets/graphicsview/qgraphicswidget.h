// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QGRAPHICSWIDGET_H
#define QGRAPHICSWIDGET_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUIGui/qfont.h>
#if BOBUI_CONFIG(action)
#  include <BobUIGui/qaction.h>
#endif
#include <BobUIWidgets/qgraphicslayoutitem.h>
#include <BobUIWidgets/qgraphicsitem.h>
#include <BobUIGui/qpalette.h>

BOBUI_REQUIRE_CONFIG(graphicsview);

BOBUI_BEGIN_NAMESPACE

class QFont;
class QFontMetrics;
class QGraphicsLayout;
class QGraphicsSceneMoveEvent;
class QGraphicsWidgetPrivate;
class QGraphicsSceneResizeEvent;
class QStyle;
class QStyleOption;

class QGraphicsWidgetPrivate;

class Q_WIDGETS_EXPORT QGraphicsWidget : public QGraphicsObject, public QGraphicsLayoutItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem QGraphicsLayoutItem)
    Q_PROPERTY(QPalette palette READ palette WRITE setPalette)
    Q_PROPERTY(QFont font READ font WRITE setFont)
    Q_PROPERTY(BobUI::LayoutDirection layoutDirection READ layoutDirection WRITE setLayoutDirection
               RESET unsetLayoutDirection)
    Q_PROPERTY(QSizeF size READ size WRITE resize NOTIFY geometryChanged)
    Q_PROPERTY(QSizeF minimumSize READ minimumSize WRITE setMinimumSize)
    Q_PROPERTY(QSizeF preferredSize READ preferredSize WRITE setPreferredSize)
    Q_PROPERTY(QSizeF maximumSize READ maximumSize WRITE setMaximumSize)
    Q_PROPERTY(QSizePolicy sizePolicy READ sizePolicy WRITE setSizePolicy)
    Q_PROPERTY(BobUI::FocusPolicy focusPolicy READ focusPolicy WRITE setFocusPolicy)
    Q_PROPERTY(BobUI::WindowFlags windowFlags READ windowFlags WRITE setWindowFlags)
    Q_PROPERTY(QString windowTitle READ windowTitle WRITE setWindowTitle)
    Q_PROPERTY(QRectF geometry READ geometry WRITE setGeometry NOTIFY geometryChanged)
    Q_PROPERTY(bool autoFillBackground READ autoFillBackground WRITE setAutoFillBackground)
    Q_PROPERTY(QGraphicsLayout* layout READ layout WRITE setLayout NOTIFY layoutChanged)
public:
    QGraphicsWidget(QGraphicsItem *parent = nullptr, BobUI::WindowFlags wFlags = BobUI::WindowFlags());
    ~QGraphicsWidget();
    QGraphicsLayout *layout() const;
    void setLayout(QGraphicsLayout *layout);
    void adjustSize();

    BobUI::LayoutDirection layoutDirection() const;
    void setLayoutDirection(BobUI::LayoutDirection direction);
    void unsetLayoutDirection();

    QStyle *style() const;
    void setStyle(QStyle *style);

    QFont font() const;
    void setFont(const QFont &font);

    QPalette palette() const;
    void setPalette(const QPalette &palette);

    bool autoFillBackground() const;
    void setAutoFillBackground(bool enabled);

    void resize(const QSizeF &size);
    inline void resize(qreal w, qreal h) { resize(QSizeF(w, h)); }
    QSizeF size() const;

    void setGeometry(const QRectF &rect) override;
    inline void setGeometry(qreal x, qreal y, qreal w, qreal h);
    inline QRectF rect() const { return QRectF(QPointF(), size()); }

    void setContentsMargins(qreal left, qreal top, qreal right, qreal bottom);
    void setContentsMargins(QMarginsF margins);
    void getContentsMargins(qreal *left, qreal *top, qreal *right, qreal *bottom) const override;

    void setWindowFrameMargins(qreal left, qreal top, qreal right, qreal bottom);
    void setWindowFrameMargins(QMarginsF margins);
    void getWindowFrameMargins(qreal *left, qreal *top, qreal *right, qreal *bottom) const;
    void unsetWindowFrameMargins();
    QRectF windowFrameGeometry() const;
    QRectF windowFrameRect() const;

    // Window handling
    BobUI::WindowFlags windowFlags() const;
    BobUI::WindowType windowType() const;
    void setWindowFlags(BobUI::WindowFlags wFlags);
    bool isActiveWindow() const;
    void setWindowTitle(const QString &title);
    QString windowTitle() const;

    // Focus handling
    BobUI::FocusPolicy focusPolicy() const;
    void setFocusPolicy(BobUI::FocusPolicy policy);
    static void setTabOrder(QGraphicsWidget *first, QGraphicsWidget *second);
    QGraphicsWidget *focusWidget() const;

#ifndef BOBUI_NO_SHORTCUT
    int grabShortcut(const QKeySequence &sequence, BobUI::ShortcutContext context = BobUI::WindowShortcut);
    void releaseShortcut(int id);
    void setShortcutEnabled(int id, bool enabled = true);
    void setShortcutAutoRepeat(int id, bool enabled = true);
#endif

#ifndef BOBUI_NO_ACTION
    //actions
    void addAction(QAction *action);
    void addActions(const QList<QAction*> &actions);
    void insertActions(QAction *before, const QList<QAction*> &actions);
    void insertAction(QAction *before, QAction *action);
    void removeAction(QAction *action);
    QList<QAction*> actions() const;
#endif

    void setAttribute(BobUI::WidgetAttribute attribute, bool on = true);
    bool testAttribute(BobUI::WidgetAttribute attribute) const;

    enum {
        Type = 11
    };
    int type() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    virtual void paintWindowFrame(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

#if 0
    void dumpFocusChain();
#endif

    using QObject::children;

Q_SIGNALS:
    void geometryChanged();
    void layoutChanged();

public Q_SLOTS:
    bool close();

protected:
    virtual void initStyleOption(QStyleOption *option) const;

    QSizeF sizeHint(BobUI::SizeHint which, const QSizeF &constraint = QSizeF()) const override;
    void updateGeometry() override;

    // Notification
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    virtual QVariant propertyChange(const QString &propertyName, const QVariant &value);

    // Scene events
    bool sceneEvent(QEvent *event) override;
    virtual bool windowFrameEvent(QEvent *e);
    virtual BobUI::WindowFrameSection windowFrameSectionAt(const QPointF& pos) const;

    // Base event handlers
    bool event(QEvent *event) override;
    //virtual void actionEvent(QActionEvent *event);
    virtual void changeEvent(QEvent *event);
    virtual void closeEvent(QCloseEvent *event);
    //void create(WId window = 0, bool initializeWindow = true, bool destroyOldWindow = true);
    //void destroy(bool destroyWindow = true, bool destroySubWindows = true);
    void focusInEvent(QFocusEvent *event) override;
    virtual bool focusNextPrevChild(bool next);
    void focusOutEvent(QFocusEvent *event) override;
    virtual void hideEvent(QHideEvent *event);
    //virtual int metric(PaintDeviceMetric m ) const;
    virtual void moveEvent(QGraphicsSceneMoveEvent *event);
    virtual void polishEvent();
    //void resetInputContext ();
    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
    virtual void showEvent(QShowEvent *event);
    //virtual void tabletEvent(BOBUIabletEvent *event);
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    virtual void grabMouseEvent(QEvent *event);
    virtual void ungrabMouseEvent(QEvent *event);
    virtual void grabKeyboardEvent(QEvent *event);
    virtual void ungrabKeyboardEvent(QEvent *event);
    QGraphicsWidget(QGraphicsWidgetPrivate &, QGraphicsItem *parent, BobUI::WindowFlags wFlags = BobUI::WindowFlags());

private:
    Q_DISABLE_COPY(QGraphicsWidget)
    Q_DECLARE_PRIVATE_D(QGraphicsItem::d_ptr.data(), QGraphicsWidget)
    friend class QGraphicsScene;
    friend class QGraphicsScenePrivate;
    friend class QGraphicsView;
    friend class QGraphicsItem;
    friend class QGraphicsItemPrivate;
    friend class QGraphicsLayout;
    friend class QWidget;
    friend class QApplication;
};

inline void QGraphicsWidget::setGeometry(qreal ax, qreal ay, qreal aw, qreal ah)
{ setGeometry(QRectF(ax, ay, aw, ah)); }

BOBUI_END_NAMESPACE

#endif

