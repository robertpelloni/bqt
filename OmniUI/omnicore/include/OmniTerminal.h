#ifndef OMNITERMINAL_H
#define OMNITERMINAL_H

#include <QQuickPaintedItem>
#include <QProcess>
#include <QStringList>
#include <QColor>

class OmniTerminal : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor NOTIFY textColorChanged)

public:
    explicit OmniTerminal(QQuickItem *parent = nullptr);
    virtual ~OmniTerminal();

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor& color);

    QColor textColor() const;
    void setTextColor(const QColor& color);

    Q_INVOKABLE void writeCommand(const QString& command);

    void paint(QPainter *painter) override;

signals:
    void backgroundColorChanged();
    void textColorChanged();
    void outputReceived(const QString& newText);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private slots:
    void readStandardOutput();
    void readStandardError();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    void startShell();
    void appendOutput(const QString& text);

    QProcess* m_process;
    QStringList m_outputBuffer;
    QString m_currentInput;
    
    QColor m_backgroundColor;
    QColor m_textColor;

    int m_scrollY;
    int m_maxLines;
};

#endif // OMNITERMINAL_H
