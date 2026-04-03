#include "OmniTerminal.h"
#include <QPainter>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QDebug>

OmniTerminal::OmniTerminal(QQuickItem *parent)
    : QQuickPaintedItem(parent),
      m_process(nullptr),
      m_backgroundColor(QColor("#0C0C0C")),
      m_textColor(QColor("#CCCCCC")),
      m_scrollY(0),
      m_maxLines(1000)
{
    setObjectName("OmniTerminal");
    setAcceptedMouseButtons(Qt::LeftButton);
    setFlag(ItemAcceptsInputMethod, true);
    setFlag(ItemHasContents, true);
    setFocusOnTouch(true);

    setWidth(800);
    setHeight(400);

#ifndef Q_OS_WASM
    m_process = new QProcess(this);
    connect(m_process, &QProcess::readyReadStandardOutput, this, &OmniTerminal::readStandardOutput);
    connect(m_process, &QProcess::readyReadStandardError, this, &OmniTerminal::readStandardError);
    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &OmniTerminal::processFinished);

    startShell();
#else
    appendOutput("OmniTerminal: Native OS shell sub-processing is disabled in WebAssembly (WASM) Sandbox.");
    appendOutput("Please use the native desktop .exe to access host OS command lines.");
#endif
}

OmniTerminal::~OmniTerminal() {
#ifndef Q_OS_WASM
    if (m_process && m_process->state() == QProcess::Running) {
        m_process->terminate();
        m_process->waitForFinished(1000);
    }
#endif
}

QColor OmniTerminal::backgroundColor() const { return m_backgroundColor; }
void OmniTerminal::setBackgroundColor(const QColor& color) {
    if (m_backgroundColor != color) {
        m_backgroundColor = color;
        emit backgroundColorChanged();
        update();
    }
}

QColor OmniTerminal::textColor() const { return m_textColor; }
void OmniTerminal::setTextColor(const QColor& color) {
    if (m_textColor != color) {
        m_textColor = color;
        emit textColorChanged();
        update();
    }
}

void OmniTerminal::startShell() {
#ifndef Q_OS_WASM
    QString program;
#ifdef Q_OS_WIN
    program = "cmd.exe";
#else
    program = "/bin/bash";
#endif
    
    m_process->start(program);
    m_outputBuffer.append(QString("OmniTerminal: Spawning OS Shell (%1)").arg(program));
#endif
}

void OmniTerminal::readStandardOutput() {
#ifndef Q_OS_WASM
    QString output = QString::fromLocal8Bit(m_process->readAllStandardOutput());
    appendOutput(output);
#endif
}

void OmniTerminal::readStandardError() {
#ifndef Q_OS_WASM
    QString output = QString::fromLocal8Bit(m_process->readAllStandardError());
    appendOutput("ERROR: " + output);
#endif
}

void OmniTerminal::processFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    Q_UNUSED(exitStatus);
    appendOutput(QString("\n[Process Exited with code %1]").arg(exitCode));
}

void OmniTerminal::appendOutput(const QString& text) {
    QStringList newLines = text.split('\n');
    for (const QString& line : newLines) {
        if (line.contains('\r') && !m_outputBuffer.isEmpty()) {
            m_outputBuffer.last() = line.simplified();
        } else {
            m_outputBuffer.append(line.simplified());
        }
    }

    while (m_outputBuffer.size() > m_maxLines) {
        m_outputBuffer.removeFirst();
    }

    m_scrollY = std::max(0, (int)m_outputBuffer.size() - 20); 
    update();
    emit outputReceived(text);
}

void OmniTerminal::writeCommand(const QString& command) {
#ifndef Q_OS_WASM
    if (m_process && m_process->state() == QProcess::Running) {
        QString fullCmd = command + "\n";
        m_process->write(fullCmd.toLocal8Bit());
        m_currentInput.clear();
        update();
    }
#else
    m_currentInput.clear();
    appendOutput("ERROR: Cannot execute commands in WASM Sandbox.");
    update();
#endif
}

void OmniTerminal::mousePressEvent(QMouseEvent *event) {
    forceActiveFocus();
    QQuickPaintedItem::mousePressEvent(event);
}

void OmniTerminal::wheelEvent(QWheelEvent *event) {
    m_scrollY -= (event->angleDelta().y() / 15);
    m_scrollY = std::clamp(m_scrollY, 0, std::max(0, (int)m_outputBuffer.size() - 5));
    update();
    QQuickPaintedItem::wheelEvent(event);
}

void OmniTerminal::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        writeCommand(m_currentInput);
    } else if (event->key() == Qt::Key_Backspace) {
        if (!m_currentInput.isEmpty()) {
            m_currentInput.chop(1);
            update();
        }
    } else if (!event->text().isEmpty()) {
        m_currentInput.append(event->text());
        update();
    } else {
        QQuickPaintedItem::keyPressEvent(event);
    }
}

void OmniTerminal::paint(QPainter *painter) {
    QRectF rect = boundingRect();
    painter->fillRect(rect, m_backgroundColor);

    QFont font("Consolas", 11);
    painter->setFont(font);
    painter->setPen(m_textColor);

    QFontMetrics metrics(font);
    int lineHeight = metrics.lineSpacing();
    int linesPerPage = rect.height() / lineHeight;
    
    m_scrollY = std::clamp(m_scrollY, 0, std::max(0, (int)m_outputBuffer.size() - linesPerPage + 1));

    int y = lineHeight;
    int endLine = std::min(m_scrollY + linesPerPage - 1, (int)m_outputBuffer.size());

    for (int i = m_scrollY; i < endLine; ++i) {
        painter->drawText(5, y, m_outputBuffer[i]);
        y += lineHeight;
    }

    if (endLine >= m_outputBuffer.size() - 1) {
        painter->setPen(QColor("#00FF00")); // Hacker green for input
        QString prompt = "> " + m_currentInput + "_";
        painter->drawText(5, y, prompt);
    }
}
