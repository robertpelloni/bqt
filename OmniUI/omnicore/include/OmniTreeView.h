#ifndef OMNITREEVIEW_H
#define OMNITREEVIEW_H

#include <QQuickPaintedItem>
#include <QString>
#include <QList>
#include <QVariantMap>

struct OmniTreeNode {
    QString text;
    bool isExpanded;
    QList<OmniTreeNode*> children;
    int depth;
};

class OmniTreeView : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(int rowHeight READ rowHeight WRITE setRowHeight NOTIFY rowHeightChanged)

public:
    explicit OmniTreeView(QQuickItem *parent = nullptr);
    virtual ~OmniTreeView();

    int rowHeight() const;
    void setRowHeight(int h);

    // Dynamic data population
    Q_INVOKABLE void addNode(const QString& text, const QString& parentText = "");
    Q_INVOKABLE void clear();

    void paint(QPainter *painter) override;

signals:
    void rowHeightChanged();
    void nodeClicked(const QString& text);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    void renderNode(QPainter* painter, OmniTreeNode* node, int& currentRow, int startRow, int endRow);
    void flattenTree(OmniTreeNode* node, QList<OmniTreeNode*>& list);

    OmniTreeNode* m_root;
    int m_rowHeight;
    int m_scrollY;
};

#endif // OMNITREEVIEW_H
