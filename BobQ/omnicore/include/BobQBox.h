#ifndef BOBQBOX_H
#define BOBQBOX_H

#include <QQuickItem>
#include <QList>

/**
 * @class BobQBox
 * @brief A QQuickItem layout container achieving 1:1 parity with GtkBox.
 *
 * Emulates GTK's sequential packing paradigm (pack_start, pack_end) natively
 * within the BobQ/Qt6 scenegraph.
 */
class BobQBox : public QQuickItem {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation NOTIFY orientationChanged)
    Q_PROPERTY(int spacing READ spacing WRITE setSpacing NOTIFY spacingChanged)
    Q_PROPERTY(bool homogeneous READ homogeneous WRITE setHomogeneous NOTIFY homogeneousChanged)

public:
    explicit BobQBox(QQuickItem *parent = nullptr);
    virtual ~BobQBox() = default;

    Qt::Orientation orientation() const;
    void setOrientation(Qt::Orientation orientation);

    int spacing() const;
    void setSpacing(int spacing);

    bool homogeneous() const;
    void setHomogeneous(bool homogeneous);

    Q_INVOKABLE void packStart(QQuickItem* child, bool expand = false, bool fill = false, int padding = 0);
    Q_INVOKABLE void packEnd(QQuickItem* child, bool expand = false, bool fill = false, int padding = 0);

signals:
    void orientationChanged();
    void spacingChanged();
    void homogeneousChanged();

protected:
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;

private:
    void relayout();

    struct BoxChild {
        QQuickItem* item;
        bool expand;
        bool fill;
        int padding;
        bool packEnd;
    };

    Qt::Orientation m_orientation = Qt::Horizontal;
    int m_spacing = 0;
    bool m_homogeneous = false;
    QList<BoxChild> m_children;
};

#endif // BOBQBOX_H
