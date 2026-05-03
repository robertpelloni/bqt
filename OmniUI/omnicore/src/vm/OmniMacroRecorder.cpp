#include "OmniMacroRecorder.h"
#include "OmniVM.h"
#include <QDebug>

OmniMacroRecorder* OmniMacroRecorder::instance() {
    static OmniMacroRecorder* recorder = new OmniMacroRecorder();
    return recorder;
}

OmniMacroRecorder::OmniMacroRecorder(QObject *parent)
    : QObject(parent),
      m_isRecording(false),
      m_isPlaying(false)
{
    setObjectName("OmniMacroRecorder");
}

OmniMacroRecorder::~OmniMacroRecorder() = default;

bool OmniMacroRecorder::isRecording() const { return m_isRecording; }
void OmniMacroRecorder::setIsRecording(bool recording) {
    if (m_isRecording != recording) {
        m_isRecording = recording;
        emit isRecordingChanged();
        if (m_isRecording) {
            qDebug() << "OmniMacroRecorder: Started tracking global OS physical interactions.";
        } else {
            qDebug() << "OmniMacroRecorder: Stopped recording. Synthesized" << m_recordedCommands.size() << "OmniScript AST nodes.";
        }
    }
}

bool OmniMacroRecorder::isPlaying() const { return m_isPlaying; }
void OmniMacroRecorder::setIsPlaying(bool playing) {
    if (m_isPlaying != playing) {
        m_isPlaying = playing;
        emit isPlayingChanged();
    }
}

void OmniMacroRecorder::startRecording() {
    clearMacro();
    setIsRecording(true);
}

void OmniMacroRecorder::stopRecording() {
    setIsRecording(false);
    emit macroGenerated(getGeneratedScript());
}

QString OmniMacroRecorder::getGeneratedScript() const {
    return m_recordedCommands.join(" ");
}

void OmniMacroRecorder::clearMacro() {
    m_recordedCommands.clear();
}

void OmniMacroRecorder::playMacro() {
    if (m_isRecording || m_recordedCommands.isEmpty()) return;

    setIsPlaying(true);
    QString script = getGeneratedScript();
    
    qDebug() << "OmniMacroRecorder: Engaging Auto-Pilot. Executing generated OmniScript...";
    // Pump the synthesized script directly into the C++ AST Interpreter
    OmniVM::instance()->evalScript(script);
    
    setIsPlaying(false);
}

void OmniMacroRecorder::recordClick(const QString& targetObjectName) {
    if (!m_isRecording || targetObjectName.isEmpty() || targetObjectName == "unnamed") return;
    
    // Synthesize the Lisp AST instruction
    QString cmd = QString("(click \"%1\")").arg(targetObjectName);
    m_recordedCommands.append(cmd);
    qDebug() << "OmniMacroRecorder: Logged ->" << cmd;
}

void OmniMacroRecorder::recordKey(const QString& targetObjectName, const QString& keyText) {
    if (!m_isRecording || targetObjectName.isEmpty() || targetObjectName == "unnamed") return;
    
    // Synthesize the Lisp AST instruction for typing (Assuming we add a 'type' hook to OmniVM later)
    // For now, we log it as a conceptual type command
    QString cmd = QString("(type \"%1\" \"%2\")").arg(targetObjectName, keyText);
    m_recordedCommands.append(cmd);
    qDebug() << "OmniMacroRecorder: Logged ->" << cmd;
}
