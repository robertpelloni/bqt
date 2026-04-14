#ifndef BOBQGRID_H
#define BOBQGRID_H

#include <QQuickItem>
#include <QMap>
#include <QPair>

/**
 * @class BobQGrid
 * @brief A QQuickItem layout container achieving 1:1 parity with GtkGrid.
 *
 * Emulates GTK's grid attachment paradigm (attach, attach_next_to) natively
 * within the BobQ/Qt6 scenegraph.
 */
class BobQGrid : public QQuickItem {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int rowSpacing READ rowSpacing WRITE setRowSpacing NOTIFY rowSpacingChanged)
    Q_PROPERTY(int columnSpacing READ columnSpacing WRITE setColumnSpacing NOTIFY columnSpacingChanged)

public:
    explicit BobQGrid(QQuickItem *parent = nullptr);
    virtual ~BobQGrid() = default;

    int rowSpacing() const;
    void setRowSpacing(int spacing);

    int columnSpacing() const;
    void setColumnSpacing(int spacing);

    Q_INVOKABLE void attach(QQuickItem* child, int left, int top, int width = 1, int height = 1);
    Q_INVOKABLE void attachNextTo(QQuickItem* child, QQuickItem* sibling, Qt::Edge side, int width = 1, int height = 1);

signals:
    void rowSpacingChanged();
    void columnSpacingChanged();

protected:
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;

private:
    void relayout();

    struct GridChild {
        QQuickItem* item;
        int left;
        int top;
        int width;
        int height;
    };

    int m_rowSpacing = 0;
    int m_columnSpacing = 0;
    QList<GridChild> m_children;
};

#endif // BOBQGRID_H
