#ifndef OMNIMARKDOWNVIEW_H
#define OMNIMARKDOWNVIEW_H

#include <QQuickPaintedItem>
#include <QString>
#include <QList>
#include <QRectF>
#include <QFont>

struct OmniMarkdownBlock {
    enum Type {
        Paragraph,
        Header1,
        Header2,
        Header3,
        CodeBlock,
        ListItem,
        HorizontalRule
    };
    Type type;
    QString text;
    QRectF rect; // Calculated during layout phase
};

class OmniMarkdownView : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

public:
    explicit OmniMarkdownView(QQuickItem *parent = nullptr);
    virtual ~OmniMarkdownView();

    QString text() const;
    void setText(const QString& text);

    void paint(QPainter *painter) override;

signals:
    void textChanged();

protected:
    void wheelEvent(QWheelEvent *event) override;
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;

private:
    void parseMarkdown();
    void layoutDocument();

    QString m_text;
    QList<OmniMarkdownBlock> m_blocks;
    
    int m_scrollY;
    int m_maxScroll;
    bool m_needsLayout;
};

#endif // OMNIMARKDOWNVIEW_H
