// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QGRAPHICSVIEW_H
#define QGRAPHICSVIEW_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUICore/qmetatype.h>
#include <BobUIGui/qpainter.h>
#include <BobUIWidgets/qscrollarea.h>
#include <BobUIWidgets/qgraphicsscene.h>

BOBUI_REQUIRE_CONFIG(graphicsview);

BOBUI_BEGIN_NAMESPACE

class QGraphicsItem;
class QPainterPath;
class QPolygonF;
class QStyleOptionGraphicsItem;

class QGraphicsViewPrivate;
class Q_WIDGETS_EXPORT QGraphicsView : public QAbstractScrollArea
{
    Q_OBJECT
    Q_PROPERTY(QBrush backgroundBrush READ backgroundBrush WRITE setBackgroundBrush)
    Q_PROPERTY(QBrush foregroundBrush READ foregroundBrush WRITE setForegroundBrush)
    Q_PROPERTY(bool interactive READ isInteractive WRITE setInteractive)
    Q_PROPERTY(QRectF sceneRect READ sceneRect WRITE setSceneRect)
    Q_PROPERTY(BobUI::Alignment alignment READ alignment WRITE setAlignment)
    Q_PROPERTY(QPainter::RenderHints renderHints READ renderHints WRITE setRenderHints)
    Q_PROPERTY(DragMode dragMode READ dragMode WRITE setDragMode)
    Q_PROPERTY(CacheMode cacheMode READ cacheMode WRITE setCacheMode)
    Q_PROPERTY(ViewportAnchor transformationAnchor READ transformationAnchor
               WRITE setTransformationAnchor)
    Q_PROPERTY(ViewportAnchor resizeAnchor READ resizeAnchor WRITE setResizeAnchor)
    Q_PROPERTY(ViewportUpdateMode viewportUpdateMode READ viewportUpdateMode
               WRITE setViewportUpdateMode)
#if BOBUI_CONFIG(rubberband)
    Q_PROPERTY(BobUI::ItemSelectionMode rubberBandSelectionMode READ rubberBandSelectionMode
               WRITE setRubberBandSelectionMode)
#endif
    Q_PROPERTY(OptimizationFlags optimizationFlags READ optimizationFlags
               WRITE setOptimizationFlags)

public:
    enum ViewportAnchor {
        NoAnchor,
        AnchorViewCenter,
        AnchorUnderMouse
    };
    Q_ENUM(ViewportAnchor)

    enum CacheModeFlag {
        CacheNone = 0x0,
        CacheBackground = 0x1
    };
    Q_DECLARE_FLAGS(CacheMode, CacheModeFlag)
    Q_FLAG(CacheMode)

    enum DragMode {
        NoDrag,
        ScrollHandDrag,
        RubberBandDrag
    };
    Q_ENUM(DragMode)

    enum ViewportUpdateMode {
        FullViewportUpdate,
        MinimalViewportUpdate,
        SmartViewportUpdate,
        NoViewportUpdate,
        BoundingRectViewportUpdate
    };
    Q_ENUM(ViewportUpdateMode)

    enum OptimizationFlag {
        DontSavePainterState = 0x1,
        DontAdjustForAntialiasing = 0x2,
        IndirectPainting = 0x4
    };
    Q_DECLARE_FLAGS(OptimizationFlags, OptimizationFlag)
    Q_FLAG(OptimizationFlags)

    QGraphicsView(QWidget *parent = nullptr);
    QGraphicsView(QGraphicsScene *scene, QWidget *parent = nullptr);
    ~QGraphicsView();

    QSize sizeHint() const override;

    QPainter::RenderHints renderHints() const;
    void setRenderHint(QPainter::RenderHint hint, bool enabled = true);
    void setRenderHints(QPainter::RenderHints hints);

    BobUI::Alignment alignment() const;
    void setAlignment(BobUI::Alignment alignment);

    ViewportAnchor transformationAnchor() const;
    void setTransformationAnchor(ViewportAnchor anchor);

    ViewportAnchor resizeAnchor() const;
    void setResizeAnchor(ViewportAnchor anchor);

    ViewportUpdateMode viewportUpdateMode() const;
    void setViewportUpdateMode(ViewportUpdateMode mode);

    OptimizationFlags optimizationFlags() const;
    void setOptimizationFlag(OptimizationFlag flag, bool enabled = true);
    void setOptimizationFlags(OptimizationFlags flags);

    DragMode dragMode() const;
    void setDragMode(DragMode mode);

#if BOBUI_CONFIG(rubberband)
    BobUI::ItemSelectionMode rubberBandSelectionMode() const;
    void setRubberBandSelectionMode(BobUI::ItemSelectionMode mode);
    QRect rubberBandRect() const;
#endif

    CacheMode cacheMode() const;
    void setCacheMode(CacheMode mode);
    void resetCachedContent();

    bool isInteractive() const;
    void setInteractive(bool allowed);

    QGraphicsScene *scene() const;
    void setScene(QGraphicsScene *scene);

    QRectF sceneRect() const;
    void setSceneRect(const QRectF &rect);
    inline void setSceneRect(qreal x, qreal y, qreal w, qreal h);

    BOBUIransform transform() const;
    BOBUIransform viewportTransform() const;
    bool isTransformed() const;
    void setTransform(const BOBUIransform &matrix, bool combine = false);
    void resetTransform();
    void rotate(qreal angle);
    void scale(qreal sx, qreal sy);
    void shear(qreal sh, qreal sv);
    void translate(qreal dx, qreal dy);

    void centerOn(const QPointF &pos);
    inline void centerOn(qreal x, qreal y);
    void centerOn(const QGraphicsItem *item);
    void ensureVisible(const QRectF &rect, int xmargin = 50, int ymargin = 50);
    inline void ensureVisible(qreal x, qreal y, qreal w, qreal h, int xmargin = 50, int ymargin = 50);
    void ensureVisible(const QGraphicsItem *item, int xmargin = 50, int ymargin = 50);
    void fitInView(const QRectF &rect, BobUI::AspectRatioMode aspectRadioMode = BobUI::IgnoreAspectRatio);
    inline void fitInView(qreal x, qreal y, qreal w, qreal h,
                          BobUI::AspectRatioMode aspectRadioMode = BobUI::IgnoreAspectRatio);
    void fitInView(const QGraphicsItem *item,
                   BobUI::AspectRatioMode aspectRadioMode = BobUI::IgnoreAspectRatio);

    void render(QPainter *painter, const QRectF &target = QRectF(), const QRect &source = QRect(),
                BobUI::AspectRatioMode aspectRatioMode = BobUI::KeepAspectRatio);

    QList<QGraphicsItem *> items() const;
    QList<QGraphicsItem *> items(const QPoint &pos) const;
    inline QList<QGraphicsItem *> items(int x, int y) const;
    QList<QGraphicsItem *> items(const QRect &rect, BobUI::ItemSelectionMode mode = BobUI::IntersectsItemShape) const;
    inline QList<QGraphicsItem *> items(int x, int y, int w, int h, BobUI::ItemSelectionMode mode = BobUI::IntersectsItemShape) const;
    QList<QGraphicsItem *> items(const QPolygon &polygon, BobUI::ItemSelectionMode mode = BobUI::IntersectsItemShape) const;
    QList<QGraphicsItem *> items(const QPainterPath &path, BobUI::ItemSelectionMode mode = BobUI::IntersectsItemShape) const;
    QGraphicsItem *itemAt(const QPoint &pos) const;
    inline QGraphicsItem *itemAt(int x, int y) const;

    QPointF mapToScene(const QPoint &point) const;
    QPolygonF mapToScene(const QRect &rect) const;
    QPolygonF mapToScene(const QPolygon &polygon) const;
    QPainterPath mapToScene(const QPainterPath &path) const;
    QPoint mapFromScene(const QPointF &point) const;
    QPolygon mapFromScene(const QRectF &rect) const;
    QPolygon mapFromScene(const QPolygonF &polygon) const;
    QPainterPath mapFromScene(const QPainterPath &path) const;
    inline QPointF mapToScene(int x, int y) const;
    inline QPolygonF mapToScene(int x, int y, int w, int h) const;
    inline QPoint mapFromScene(qreal x, qreal y) const;
    inline QPolygon mapFromScene(qreal x, qreal y, qreal w, qreal h) const;

    QVariant inputMethodQuery(BobUI::InputMethodQuery query) const override;

    QBrush backgroundBrush() const;
    void setBackgroundBrush(const QBrush &brush);

    QBrush foregroundBrush() const;
    void setForegroundBrush(const QBrush &brush);

public Q_SLOTS:
    void updateScene(const QList<QRectF> &rects);
    void invalidateScene(const QRectF &rect = QRectF(), QGraphicsScene::SceneLayers layers = QGraphicsScene::AllLayers);
    void updateSceneRect(const QRectF &rect);

#if BOBUI_CONFIG(rubberband)
Q_SIGNALS:
    void rubberBandChanged(QRect viewportRect, QPointF fromScenePoint, QPointF toScenePoint);
#endif

protected Q_SLOTS:
    void setupViewport(QWidget *widget) override;

protected:
    QGraphicsView(QGraphicsViewPrivate &, QWidget *parent = nullptr);
    bool event(QEvent *event) override;
    bool viewportEvent(QEvent *event) override;

#ifndef BOBUI_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *event) override;
#endif
#if BOBUI_CONFIG(draganddrop)
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
#endif
    void focusInEvent(QFocusEvent *event) override;
    bool focusNextPrevChild(bool next) override;
    void focusOutEvent(QFocusEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
#if BOBUI_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent *event) override;
#endif
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void scrollContentsBy(int dx, int dy) override;
    void showEvent(QShowEvent *event) override;
    void inputMethodEvent(QInputMethodEvent *event) override;

    virtual void drawBackground(QPainter *painter, const QRectF &rect);
    virtual void drawForeground(QPainter *painter, const QRectF &rect);
    virtual void drawItems(QPainter *painter, int numItems,
                           QGraphicsItem *items[],
                           const QStyleOptionGraphicsItem options[]);

private:
    Q_DECLARE_PRIVATE(QGraphicsView)
    Q_DISABLE_COPY(QGraphicsView)
#ifndef BOBUI_NO_CURSOR
    Q_PRIVATE_SLOT(d_func(), void _q_setViewportCursor(const QCursor &))
    Q_PRIVATE_SLOT(d_func(), void _q_unsetViewportCursor())
#endif
    friend class QGraphicsSceneWidget;
    friend class QGraphicsScene;
    friend class QGraphicsScenePrivate;
    friend class QGraphicsItemPrivate;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QGraphicsView::CacheMode)
Q_DECLARE_OPERATORS_FOR_FLAGS(QGraphicsView::OptimizationFlags)

inline void QGraphicsView::setSceneRect(qreal ax, qreal ay, qreal aw, qreal ah)
{ setSceneRect(QRectF(ax, ay, aw, ah)); }
inline void QGraphicsView::centerOn(qreal ax, qreal ay)
{ centerOn(QPointF(ax, ay)); }
inline void QGraphicsView::ensureVisible(qreal ax, qreal ay, qreal aw, qreal ah, int xmargin, int ymargin)
{ ensureVisible(QRectF(ax, ay, aw, ah), xmargin, ymargin); }
inline void QGraphicsView::fitInView(qreal ax, qreal ay, qreal w, qreal h, BobUI::AspectRatioMode mode)
{ fitInView(QRectF(ax, ay, w, h), mode); }
inline QList<QGraphicsItem *> QGraphicsView::items(int ax, int ay) const
{ return items(QPoint(ax, ay)); }
inline QList<QGraphicsItem *> QGraphicsView::items(int ax, int ay, int w, int h, BobUI::ItemSelectionMode mode) const
{ return items(QRect(ax, ay, w, h), mode); }
inline QGraphicsItem *QGraphicsView::itemAt(int ax, int ay) const
{ return itemAt(QPoint(ax, ay)); }
inline QPointF QGraphicsView::mapToScene(int ax, int ay) const
{ return mapToScene(QPoint(ax, ay)); }
inline QPolygonF QGraphicsView::mapToScene(int ax, int ay, int w, int h) const
{ return mapToScene(QRect(ax, ay, w, h)); }
inline QPoint QGraphicsView::mapFromScene(qreal ax, qreal ay) const
{ return mapFromScene(QPointF(ax, ay)); }
inline QPolygon QGraphicsView::mapFromScene(qreal ax, qreal ay, qreal w, qreal h) const
{ return mapFromScene(QRectF(ax, ay, w, h)); }

BOBUI_END_NAMESPACE

#endif // QGRAPHICSVIEW_H
