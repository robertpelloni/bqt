#include "OmniAudioGraph.h"
#include <QDebug>

OmniAudioGraph::OmniAudioGraph(QQuickItem *parent)
    : QQuickItem(parent),
      m_isRunning(false)
{
    setObjectName("OmniAudioGraph");
}

OmniAudioGraph::~OmniAudioGraph() {
    stop();
}

void OmniAudioGraph::addProcessor(QObject* processor) {
    if (!processor || m_processors.contains(processor)) return;

    m_processors.append(processor);
    qDebug() << "OmniAudioGraph: Added processor:" << processor->objectName();
    emit graphChanged();
}

void OmniAudioGraph::connectProcessors(QObject* source, QObject* dest) {
    if (!source || !dest) return;
    if (!m_processors.contains(source) || !m_processors.contains(dest)) {
        qWarning() << "OmniAudioGraph: Cannot connect processors not in graph!";
        return;
    }
    
    qDebug() << "OmniAudioGraph: Connected" << source->objectName() << "->" << dest->objectName();
    // In a real implementation, we would call juce::AudioProcessorGraph::addConnection()
    emit graphChanged();
}

void OmniAudioGraph::start() {
    if (m_isRunning) return;
    m_isRunning = true;
    qDebug() << "OmniAudioGraph: Audio processing thread STARTED. (Simulating JUCE DeviceManager)";
    emit processingStarted();
}

void OmniAudioGraph::stop() {
    if (!m_isRunning) return;
    m_isRunning = false;
    qDebug() << "OmniAudioGraph: Audio processing thread STOPPED.";
    emit processingStopped();
}
