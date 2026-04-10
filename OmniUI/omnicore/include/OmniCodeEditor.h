#ifndef OMNICODEEDITOR_H
#define OMNICODEEDITOR_H

#include <QQuickPaintedItem>
#include <QString>
#include <QStringList>
#include <QColor>
#include <BOBUIimer>

class OmniCodeEditor : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QString language READ language WRITE setLanguage NOTIFY languageChanged)
    Q_PROPERTY(QColor accentColor READ accentColor WRITE setAccentColor NOTIFY accentColorChanged)

public:
    explicit OmniCodeEditor(QQuickItem *parent = nullptr);
    virtual ~OmniCodeEditor();

    QString text() const;
    void setText(const QString& text);

    QString language() const;
    void setLanguage(const QString& lang);

    QColor accentColor() const;
    void setAccentColor(const QColor& color);

    void paint(QPainter *painter) override;

signals:
    void textChanged();
    void languageChanged();
    void accentColorChanged();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private slots:
    void blinkCursor();

private:
    void highlightSyntax(QPainter* painter, const QString& line, const QRectF& rect);

    QStringList m_lines;
    QString m_language;
    QColor m_accentColor;
    
    int m_cursorLine;
    int m_cursorCol;
    bool m_isFocused;
    bool m_cursorVisible;
    int m_scrollY;
    
    BOBUIimer m_blinkTimer;
};

#endif // OMNICODEEDITOR_H
