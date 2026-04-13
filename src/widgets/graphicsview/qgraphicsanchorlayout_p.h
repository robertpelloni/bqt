// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QGRAPHICSANCHORLAYOUT_P_H
#define QGRAPHICSANCHORLAYOUT_P_H

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

#include <BobUIWidgets/private/bobuiwidgetsglobal_p.h>
#include <QGraphicsWidget>
#include <private/qobject_p.h>

#include "qgraphicslayout_p.h"
#include "qgraphicsanchorlayout.h"
#include "qgraph_p.h"
#include "qsimplex_p.h"

#include <BobUIGui/private/qgridlayoutengine_p.h>

#include <array>

BOBUI_REQUIRE_CONFIG(graphicsview);

BOBUI_BEGIN_NAMESPACE

/*
  The public QGraphicsAnchorLayout interface represents an anchorage point
  as a pair of a <QGraphicsLayoutItem *> and a <BobUI::AnchorPoint>.

  Internally though, it has a graph of anchorage points (vertices) and
  anchors (edges), represented by the AnchorVertex and AnchorData structs
  respectively.
*/

namespace BobUIGraphicsAnchorLayout {
/*!
  \internal

  Represents a vertex (anchorage point) in the internal graph
*/
struct AnchorVertex
{
    AnchorVertex(QGraphicsLayoutItem *item, BobUI::AnchorPoint edge)
        : m_item(item), m_edge(edge) {}

    AnchorVertex()
        : m_item(nullptr), m_edge(BobUI::AnchorPoint(0)) {}

    virtual ~AnchorVertex() = default;

#ifdef BOBUI_DEBUG
    virtual inline QString toString() const;
#endif

    QGraphicsLayoutItem *m_item;
    BobUI::AnchorPoint m_edge;

    // Current distance from this vertex to the layout edge (Left or Top)
    // Value is calculated from the current anchors sizes.
    qreal distance;
};

/*!
  \internal

  Represents an edge (anchor) in the internal graph.
*/
struct AnchorData : public QSimplexVariable {
    Q_DISABLE_COPY_MOVE(AnchorData)

    enum Type {
        Normal = 0,
        Sequential,
        Parallel
    };

    enum Dependency {
        Independent = 0,
        Master,
        Slave
    };

    AnchorData()
        : QSimplexVariable(), from(nullptr), to(nullptr),
          minSize(0), prefSize(0), maxSize(0),
          minPrefSize(0), maxPrefSize(0),
          sizeAtMinimum(0), sizeAtPreferred(0),
          sizeAtMaximum(0), item(nullptr), graphicsAnchor(nullptr),
          type(Normal), isLayoutAnchor(false),
          isCenterAnchor(false), isVertical(false),
          dependency(Independent) {}
    virtual ~AnchorData();

    virtual void updateChildrenSizes() {}
    void refreshSizeHints(const QLayoutStyleInfo *styleInfo = nullptr);

#ifdef BOBUI_DEBUG
    void dump(int indent = 2);
    inline QString toString() const;
    QString name;
#endif

    // Anchor is semantically directed
    AnchorVertex *from;
    AnchorVertex *to;

    // Nominal sizes
    // These are the intrinsic size restrictions for a given item. They are
    // used as input for the calculation of the actual sizes.
    // These values are filled by the refreshSizeHints method, based on the
    // anchor size policy, the size hints of the item it (possibly) represents
    // and the layout spacing information.
    qreal minSize;
    qreal prefSize;
    qreal maxSize;

    qreal minPrefSize;
    qreal maxPrefSize;

    // Calculated sizes
    // These attributes define which sizes should that anchor be in when the
    // layout is at its minimum, preferred or maximum sizes. Values are
    // calculated by the Simplex solver based on the current layout setup.
    qreal sizeAtMinimum;
    qreal sizeAtPreferred;
    qreal sizeAtMaximum;

    // References to the classes that represent this anchor in the public world
    // An anchor may represent a LayoutItem, it may also be accessible externally
    // through a GraphicsAnchor "handler".
    QGraphicsLayoutItem *item;
    QGraphicsAnchor *graphicsAnchor;

    uint type : 2;            // either Normal, Sequential or Parallel
    uint isLayoutAnchor : 1;  // if this anchor is an internal layout anchor
    uint isCenterAnchor : 1;
    uint isVertical : 1;
    uint dependency : 2;      // either Independent, Master or Slave
};

#ifdef BOBUI_DEBUG
inline QString AnchorData::toString() const
{
    return QString::fromLatin1("Anchor(%1)").arg(name);
}
#endif

struct SequentialAnchorData : public AnchorData
{
    SequentialAnchorData(const QList<AnchorVertex *> &vertices, const QList<AnchorData *> &edges)
        : AnchorData(), m_edges(edges)
    {
        type = AnchorData::Sequential;
        isVertical = m_edges.at(0)->isVertical;
#ifdef BOBUI_DEBUG
        name = QString::fromLatin1("%1 -- %2").arg(vertices.first()->toString(), vertices.last()->toString());
#else
        Q_UNUSED(vertices);
#endif
    }

    virtual void updateChildrenSizes() override;
    void calculateSizeHints();

    const QList<AnchorData *> m_edges; // keep the list of edges too.
};

struct ParallelAnchorData : public AnchorData
{
    ParallelAnchorData(AnchorData *first, AnchorData *second)
        : AnchorData(), firstEdge(first), secondEdge(second)
    {
        type = AnchorData::Parallel;
        isVertical = first->isVertical;

        // This assert whether the child anchors share their vertices
        Q_ASSERT(((first->from == second->from) && (first->to == second->to)) ||
                 ((first->from == second->to) && (first->to == second->from)));

        // Our convention will be that the parallel group anchor will have the same
        // direction as the first anchor.
        from = first->from;
        to = first->to;
#ifdef BOBUI_DEBUG
        name = QString::fromLatin1("%1 | %2").arg(first->toString(), second->toString());
#endif
    }

    virtual void updateChildrenSizes() override;
    bool calculateSizeHints();

    bool secondForward() const {
        // We have the convention that the first children will define the direction of the
        // pararell group. Note that we can't rely on 'this->from' or 'this->to'  because they
        // might be changed by vertex simplification.
        return firstEdge->from == secondEdge->from;
    }

    AnchorData* firstEdge;
    AnchorData* secondEdge;

    QList<QSimplexConstraint *> m_firstConstraints;
    QList<QSimplexConstraint *> m_secondConstraints;
};

struct AnchorVertexPair : public AnchorVertex {
    AnchorVertexPair(AnchorVertex *v1, AnchorVertex *v2, AnchorData *data)
        : AnchorVertex(), m_first(v1), m_second(v2), m_removedAnchor(data)
    {
    }

    AnchorVertex *m_first;
    AnchorVertex *m_second;

    AnchorData *m_removedAnchor;
    QList<AnchorData *> m_firstAnchors;
    QList<AnchorData *> m_secondAnchors;

#ifdef BOBUI_DEBUG
    inline QString toString() const override
    {
        return QString::fromLatin1("(%1, %2)").arg(m_first->toString(), m_second->toString());
    }
#endif
};

#ifdef BOBUI_DEBUG
inline QString AnchorVertex::toString() const
{
    using namespace BobUI::StringLiterals;

    if (!m_item)
        return QString::fromLatin1("NULL_%1").arg(quintptr(this));

    QString edge;
    switch (m_edge) {
    case BobUI::AnchorLeft:
        edge = "Left"_L1;
        break;
    case BobUI::AnchorHorizontalCenter:
        edge = "HorizontalCenter"_L1;
        break;
    case BobUI::AnchorRight:
        edge = "Right"_L1;
        break;
    case BobUI::AnchorTop:
        edge = "Top"_L1;
        break;
    case BobUI::AnchorVerticalCenter:
        edge = "VerticalCenter"_L1;
        break;
    case BobUI::AnchorBottom:
        edge = "Bottom"_L1;
        break;
    default:
        edge = "None"_L1;
        break;
    }
    QString itemName;
    if (m_item->isLayout()) {
        itemName = "layout"_L1;
    } else {
        if (QGraphicsItem *item = m_item->graphicsItem()) {
            itemName = item->data(0).toString();
        }
    }
    edge.insert(0, "%1_"_L1);
    return edge.arg(itemName);
}
#endif

/*!
  \internal

  Representation of a valid path for a given vertex in the graph.
  In this struct, "positives" is the set of anchors that have been
  traversed in the forward direction, while "negatives" is the set
  with the ones walked backwards.

  This paths are compared against each other to produce LP Constraints,
  the exact order in which the anchors were traversed is not relevant.
*/
class GraphPath
{
public:
    GraphPath() {}

    QSimplexConstraint *constraint(const GraphPath &path) const;
#ifdef BOBUI_DEBUG
    QString toString() const;
#endif
    QSet<AnchorData *> positives;
    QSet<AnchorData *> negatives;
};
} // namespace BobUIGraphicsAnchorLayout
using namespace BobUIGraphicsAnchorLayout;

Q_DECLARE_TYPEINFO(GraphPath, Q_RELOCATABLE_TYPE);

class QGraphicsAnchorLayoutPrivate;
/*!
    \internal
*/
class QGraphicsAnchorPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QGraphicsAnchor)

public:
    explicit QGraphicsAnchorPrivate(decltype(QObjectPrivateVersion) = QObjectPrivateVersion);
    ~QGraphicsAnchorPrivate();

    void setSpacing(qreal value);
    void unsetSpacing();
    qreal spacing() const;

    void setSizePolicy(QSizePolicy::Policy policy);

    static QGraphicsAnchorPrivate *get(QGraphicsAnchor *q)
    { return q->d_func(); }

    QGraphicsAnchorLayoutPrivate *layoutPrivate;
    AnchorData *data;

    // Size information for user controlled anchor
    QSizePolicy::Policy sizePolicy;
    qreal preferredSize;

    uint hasSize : 1;         // if false, get size from style.
};




/*!
  \internal

  QGraphicsAnchorLayout private methods and attributes.
*/
class Q_AUTOTEST_EXPORT QGraphicsAnchorLayoutPrivate : public QGraphicsLayoutPrivate
{
    Q_DECLARE_PUBLIC(QGraphicsAnchorLayout)

public:
    // When the layout geometry is different from its Minimum, Preferred
    // or Maximum values, interpolation is used to calculate the geometries
    // of the items.
    //
    // Interval represents which interpolation interval are we operating in.
    enum Interval {
        MinimumToMinPreferred = 0,
        MinPreferredToPreferred,
        PreferredToMaxPreferred,
        MaxPreferredToMaximum
    };

    typedef BobUI::Orientation Orientation [[deprecated]];
    [[deprecated]] static inline constexpr BobUI::Orientation Horizontal = BobUI::Horizontal;
    [[deprecated]] static inline constexpr BobUI::Orientation Vertical = BobUI::Vertical;

    QGraphicsAnchorLayoutPrivate();

    static QGraphicsAnchorLayoutPrivate *get(QGraphicsAnchorLayout *q)
    {
        return q ? q->d_func() : nullptr;
    }

    static BobUI::AnchorPoint oppositeEdge(
        BobUI::AnchorPoint edge);

    static BobUI::Orientation edgeOrientation(BobUI::AnchorPoint edge) noexcept;

    static BobUI::AnchorPoint pickEdge(BobUI::AnchorPoint edge, BobUI::Orientation orientation)
    {
        if (orientation == BobUI::Vertical && int(edge) <= 2)
            return (BobUI::AnchorPoint)(edge + 3);
        else if (orientation == BobUI::Horizontal && int(edge) >= 3) {
            return (BobUI::AnchorPoint)(edge - 3);
        }
        return edge;
    }

    // Init methods
    void createLayoutEdges();
    void deleteLayoutEdges();
    void createItemEdges(QGraphicsLayoutItem *item);
    void createCenterAnchors(QGraphicsLayoutItem *item, BobUI::AnchorPoint centerEdge);
    void removeCenterAnchors(QGraphicsLayoutItem *item, BobUI::AnchorPoint centerEdge, bool substitute = true);
    void removeCenterConstraints(QGraphicsLayoutItem *item, BobUI::Orientation orientation);

    QGraphicsAnchor *acquireGraphicsAnchor(AnchorData *data)
    {
        Q_Q(QGraphicsAnchorLayout);
        if (!data->graphicsAnchor) {
            data->graphicsAnchor = new QGraphicsAnchor(q);
            data->graphicsAnchor->d_func()->data = data;
        }
        return data->graphicsAnchor;
    }

    // function used by the 4 API functions
    QGraphicsAnchor *addAnchor(QGraphicsLayoutItem *firstItem,
                            BobUI::AnchorPoint firstEdge,
                            QGraphicsLayoutItem *secondItem,
                            BobUI::AnchorPoint secondEdge,
                            qreal *spacing = nullptr);

    // Helper for Anchor Manipulation methods
    void addAnchor_helper(QGraphicsLayoutItem *firstItem,
                   BobUI::AnchorPoint firstEdge,
                   QGraphicsLayoutItem *secondItem,
                   BobUI::AnchorPoint secondEdge,
                   AnchorData *data);

    QGraphicsAnchor *getAnchor(QGraphicsLayoutItem *firstItem, BobUI::AnchorPoint firstEdge,
                               QGraphicsLayoutItem *secondItem, BobUI::AnchorPoint secondEdge);

    void removeAnchor(AnchorVertex *firstVertex, AnchorVertex *secondVertex);
    void removeAnchor_helper(AnchorVertex *v1, AnchorVertex *v2);

    void removeAnchors(QGraphicsLayoutItem *item);

    void removeVertex(QGraphicsLayoutItem *item, BobUI::AnchorPoint edge);

    void correctEdgeDirection(QGraphicsLayoutItem *&firstItem,
                              BobUI::AnchorPoint &firstEdge,
                              QGraphicsLayoutItem *&secondItem,
                              BobUI::AnchorPoint &secondEdge);

    QLayoutStyleInfo &styleInfo() const;

    AnchorData *addAnchorMaybeParallel(AnchorData *newAnchor, bool *feasible);

    // Activation
    void calculateGraphs();
    void calculateGraphs(BobUI::Orientation orientation);

    // Simplification
    bool simplifyGraph(BobUI::Orientation orientation);
    bool simplifyVertices(BobUI::Orientation orientation);
    bool simplifyGraphIteration(BobUI::Orientation orientation, bool *feasible);

    bool replaceVertex(BobUI::Orientation orientation, AnchorVertex *oldV,
                       AnchorVertex *newV, const QList<AnchorData *> &edges);


    void restoreSimplifiedGraph(BobUI::Orientation orientation);
    void restoreSimplifiedAnchor(AnchorData *edge);
    void restoreSimplifiedConstraints(ParallelAnchorData *parallel);
    void restoreVertices(BobUI::Orientation orientation);

    bool calculateTrunk(BobUI::Orientation orientation, const GraphPath &trunkPath,
                        const QList<QSimplexConstraint *> &constraints,
                        const QList<AnchorData *> &variables);
    bool calculateNonTrunk(const QList<QSimplexConstraint *> &constraints,
                           const QList<AnchorData *> &variables);

    // Support functions for calculateGraph()
    void refreshAllSizeHints(BobUI::Orientation orientation);
    void findPaths(BobUI::Orientation orientation);
    void constraintsFromPaths(BobUI::Orientation orientation);
    void updateAnchorSizes(BobUI::Orientation orientation);
    QList<QSimplexConstraint *> constraintsFromSizeHints(const QList<AnchorData *> &anchors);
    struct GraphParts {
        QList<QSimplexConstraint *> trunkConstraints;
        QList<QSimplexConstraint *> nonTrunkConstraints;
    };
    GraphParts getGraphParts(BobUI::Orientation orientation);
    void identifyFloatItems(const QSet<AnchorData *> &visited, BobUI::Orientation orientation);
    void identifyNonFloatItems_helper(const AnchorData *ad, QSet<QGraphicsLayoutItem *> *nonFloatingItemsIdentifiedSoFar);

    inline AnchorVertex *internalVertex(const std::pair<QGraphicsLayoutItem*, BobUI::AnchorPoint> &itemEdge) const
    {
        return m_vertexList.value(itemEdge).first;
    }

    inline AnchorVertex *internalVertex(const QGraphicsLayoutItem *item, BobUI::AnchorPoint edge) const
    {
        return internalVertex(std::pair(const_cast<QGraphicsLayoutItem *>(item), edge));
    }

    inline void changeLayoutVertex(BobUI::Orientation orientation, AnchorVertex *oldV, AnchorVertex *newV)
    {
        if (layoutFirstVertex[orientation] == oldV)
            layoutFirstVertex[orientation] = newV;
        else if (layoutCentralVertex[orientation] == oldV)
            layoutCentralVertex[orientation] = newV;
        else if (layoutLastVertex[orientation] == oldV)
            layoutLastVertex[orientation] = newV;
    }


    AnchorVertex *addInternalVertex(QGraphicsLayoutItem *item, BobUI::AnchorPoint edge);
    void removeInternalVertex(QGraphicsLayoutItem *item, BobUI::AnchorPoint edge);

    // Geometry interpolation methods
    void setItemsGeometries(const QRectF &geom);

    void calculateVertexPositions(BobUI::Orientation orientation);
    void setupEdgesInterpolation(BobUI::Orientation orientation);
    void interpolateEdge(AnchorVertex *base, AnchorData *edge);

    // Linear Programming solver methods
    bool solveMinMax(const QList<QSimplexConstraint *> &constraints,
                     const GraphPath &path, qreal *min, qreal *max);
    bool solvePreferred(const QList<QSimplexConstraint *> &constraints,
                        const QList<AnchorData *> &variables);
    bool hasConflicts() const;

#ifdef BOBUI_DEBUG
    void dumpGraph(const QString &name = QString());
#endif


    QHVContainer<qreal> spacings = {-1, -1};
    // Size hints from simplex engine
    QHVContainer<std::array<qreal, 3>> sizeHints = {{-1, -1, -1}, {-1, -1, -1}};

    // Items
    QList<QGraphicsLayoutItem *> items;

    // Mapping between high level anchorage points (Item, Edge) to low level
    // ones (Graph Vertices)

    QHash<std::pair<QGraphicsLayoutItem*, BobUI::AnchorPoint>, std::pair<AnchorVertex *, int> > m_vertexList;

    // Internal graph of anchorage points and anchors, for both orientations
    QHVContainer<Graph<AnchorVertex, AnchorData>> graph;

    QHVContainer<AnchorVertex *> layoutFirstVertex = {};
    QHVContainer<AnchorVertex *> layoutCentralVertex = {};
    QHVContainer<AnchorVertex *> layoutLastVertex = {};

    // Combined anchors in order of creation
    QHVContainer<QList<AnchorVertexPair *>> simplifiedVertices;
    QHVContainer<QList<AnchorData *>> anchorsFromSimplifiedVertices;

    // Graph paths and constraints, for both orientations
    QHVContainer<QMultiHash<AnchorVertex *, GraphPath>> graphPaths;
    QHVContainer<QList<QSimplexConstraint *>> constraints;
    QHVContainer<QList<QSimplexConstraint *>> itemCenterConstraints;

    // The interpolation interval and progress based on the current size
    // as well as the key values (minimum, preferred and maximum)
    QHVContainer<Interval> interpolationInterval;
    QHVContainer<qreal> interpolationProgress = {-1, -1};

    QHVContainer<bool> graphHasConflicts = {};
    QHVContainer<QSet<QGraphicsLayoutItem *>> m_floatItems;

#if defined(BOBUI_DEBUG) || defined(BOBUI_BUILD_INTERNAL)
    QHVContainer<bool> lastCalculationUsedSimplex;
#endif

    uint calculateGraphCacheDirty : 1;
    mutable uint styleInfoDirty : 1;
    mutable QLayoutStyleInfo cachedStyleInfo;

    friend class QGraphicsAnchorPrivate;
};

BOBUI_END_NAMESPACE

#endif
