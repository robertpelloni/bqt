#ifndef OMNIAUDIOGRAPH_H
#define OMNIAUDIOGRAPH_H

#include <QQuickItem>
#include <QList>

class OmniAudioGraph : public QQuickItem {
    Q_OBJECT

public:
    explicit OmniAudioGraph(QQuickItem *parent = nullptr);
    virtual ~OmniAudioGraph();

    // Adds a DSP node (like OmniFilter or OmniGain) to the graph
    Q_INVOKABLE void addProcessor(QObject* processor);

    // Connects two nodes in the graph
    Q_INVOKABLE void connectProcessors(QObject* source, QObject* dest);

    // Starts the audio processing thread (simulated for now)
    Q_INVOKABLE void start();
    
    // Stops the audio processing thread
    Q_INVOKABLE void stop();

signals:
    void graphChanged();
    void processingStarted();
    void processingStopped();

private:
    QList<QObject*> m_processors;
    bool m_isRunning;
};

#endif // OMNIAUDIOGRAPH_H
