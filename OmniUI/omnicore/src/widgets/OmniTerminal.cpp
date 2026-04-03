#include "OmniTerminal.h"
#include <QPainter>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QDebug>

OmniTerminal::OmniTerminal(QQuickItem *parent)
    : QQuickPaintedItem(parent),
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

    m_process = new QProcess(this);
    connect(m_process, &QProcess::readyReadStandardOutput, this, &OmniTerminal::readStandardOutput);
    connect(m_process, &QProcess::readyReadStandardError, this, &OmniTerminal::readStandardError);
    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &OmniTerminal::processFinished);

    startShell();
}

OmniTerminal::~OmniTerminal() {
    if (m_process->state() == QProcess::Running) {
        m_process->terminate();
        m_process->waitForFinished(1000);
    }
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
    QString program;
#ifdef Q_OS_WIN
    program = "cmd.exe";
#else
    program = "/bin/bash";
#endif
    
    m_process->start(program);
    m_outputBuffer.append(QString("OmniTerminal: Spawning OS Shell (%1)").arg(program));
}

void OmniTerminal::readStandardOutput() {
    QString output = QString::fromLocal8Bit(m_process->readAllStandardOutput());
    appendOutput(output);
}

void OmniTerminal::readStandardError() {
    QString output = QString::fromLocal8Bit(m_process->readAllStandardError());
    appendOutput("ERROR: " + output);
}

void OmniTerminal::processFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    Q_UNUSED(exitStatus);
    appendOutput(QString("\n[Process Exited with code %1]").arg(exitCode));
}

void OmniTerminal::appendOutput(const QString& text) {
    QStringList newLines = text.split('\n');
    for (const QString& line : newLines) {
        // Handle basic carriage returns replacing the last line
        if (line.contains('\r') && !m_outputBuffer.isEmpty()) {
            m_outputBuffer.last() = line.simplified();
        } else {
            m_outputBuffer.append(line.simplified());
        }
    }

    // Trim buffer to prevent infinite memory usage
    while (m_outputBuffer.size() > m_maxLines) {
        m_outputBuffer.removeFirst();
    }

    // Auto-scroll to bottom
    m_scrollY = std::max(0, m_outputBuffer.size() - 20); // Arbitrary lines per page fallback
    update();
    emit outputReceived(text);
}

void OmniTerminal::writeCommand(const QString& command) {
    if (m_process->state() == QProcess::Running) {
        QString fullCmd = command + "\n";
        m_process->write(fullCmd.toLocal8Bit());
        m_currentInput.clear();
        update();
    }
}

void OmniTerminal::mousePressEvent(QMouseEvent *event) {
    forceActiveFocus();
    QQuickPaintedItem::mousePressEvent(event);
}

void OmniTerminal::wheelEvent(QWheelEvent *event) {
    m_scrollY -= (event->angleDelta().y() / 15); // Simple scroll scaling
    m_scrollY = std::clamp(m_scrollY, 0, std::max(0, m_outputBuffer.size() - 5));
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
    
    // Correct scroll bounds dynamically
    m_scrollY = std::clamp(m_scrollY, 0, std::max(0, m_outputBuffer.size() - linesPerPage + 1));

    int y = lineHeight;
    int endLine = std::min(m_scrollY + linesPerPage - 1, m_outputBuffer.size());

    // Draw historical output
    for (int i = m_scrollY; i < endLine; ++i) {
        painter->drawText(5, y, m_outputBuffer[i]);
        y += lineHeight;
    }

    // Draw active input line if at the bottom of the scroll
    if (endLine >= m_outputBuffer.size() - 1) {
        painter->setPen(QColor("#00FF00")); // Hacker green for input
        QString prompt = "> " + m_currentInput + "_";
        painter->drawText(5, y, prompt);
    }
}
