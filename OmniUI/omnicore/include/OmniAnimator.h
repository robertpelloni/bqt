#ifndef OMNIANIMATOR_H
#define OMNIANIMATOR_H

#include <QObject>
#include <QString>
#include <QList>
#include <QPointer>
#include <BOBUIimer>
#include <QElapsedTimer>

struct OmniAnimation {
    QPointer<QObject> target;
    QByteArray propertyName;
    qreal startValue;
    qreal endValue;
    qint64 startTimeMs;
    int durationMs;
};

// Native C++ Animation Engine (JavaFX Timeline Parity)
// Bypasses QML declarative animations to allow programmatic C++ physics/morphs
class OmniAnimator : public QObject {
    Q_OBJECT

public:
    static OmniAnimator* instance();

    Q_INVOKABLE void animateTo(QObject* target, const QString& propertyName, qreal targetValue, int durationMs = 300);
    Q_INVOKABLE void stopAll(QObject* target);

signals:
    void animationFinished(QObject* target, const QString& propertyName);

private slots:
    void processFrame();

private:
    explicit OmniAnimator(QObject *parent = nullptr);
    ~OmniAnimator() override;

    QList<OmniAnimation> m_animations;
    BOBUIimer m_frameTimer;
    QElapsedTimer m_clock;
};

#endif // OMNIANIMATOR_H
