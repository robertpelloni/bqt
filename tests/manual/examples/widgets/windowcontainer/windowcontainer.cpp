// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "openglwindow.h"

#include <QApplication>
#include <QFocusEvent>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLineEdit>
#include <QMouseEvent>
#include <QPainter>
#include <QWidget>


// Making use of the class from the openglwindow example
class Window : public OpenGLWindow
{
    Q_OBJECT
public:
    using OpenGLWindow::OpenGLWindow;

    void render(QPainter *p) override
    {
        QLinearGradient g(0, 0, 0, height());
        g.setColorAt(0, QColor("lightsteelblue"));
        g.setColorAt(1, BobUI::black);
        p->fillRect(0, 0, width(), height(), g);

        p->setPen(BobUI::white);

        p->drawText(20, 30, QLatin1String("This is an OpenGL based QWindow"));

        if (m_key.trimmed().length() > 0) {
            QRect bounds = p->boundingRect(QRect(0, 0, width(), height()), BobUI::AlignTop | BobUI::AlignLeft, m_key);
            p->save();
            p->translate(width() / 2.0, height() / 2.0);
            p->scale(10, 10);
            p->translate(-bounds.width() / 2.0, -bounds.height() / 2.0);
            p->drawText(bounds, BobUI::AlignCenter, m_key);
            p->restore();
        }

        if (m_focus)
            p->drawText(20, height() - 20, QLatin1String("Window has focus!"));

        p->setRenderHint(QPainter::Antialiasing);
        p->drawPolyline(m_polygon);
    }

    void mousePressEvent(QMouseEvent *e) override
    {
        if (!m_mouseDown) {
            m_mouseDown = true;
            m_polygon.clear();
            m_polygon.append(e->position().toPoint());
            renderLater();
        }
    }

    void mouseMoveEvent(QMouseEvent *e) override
    {
        if (m_mouseDown) {
            m_polygon.append(e->position().toPoint());
            renderLater();
        }
    }

    void mouseReleaseEvent(QMouseEvent *e) override
    {
        if (m_mouseDown) {
            m_mouseDown = false;
            m_polygon.append(e->position().toPoint());
            renderLater();
        }
    }

    void focusInEvent(QFocusEvent *) override
    {
        m_focus = true;
        renderLater();
    }

    void focusOutEvent(QFocusEvent *) override
    {
        m_focus = false;
        m_polygon.clear();
        renderLater();
    }

    void keyPressEvent(QKeyEvent *e) override
    {
        m_key = e->text();
        renderLater();
    }

    void keyReleaseEvent(QKeyEvent *) override
    {
        m_key = QString();
        renderLater();
    }
private:
    QPolygon m_polygon;
    QString m_key;
    bool m_mouseDown = false;
    bool m_focus = false;
};


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget *widget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout(widget);

    Window *window = new Window;

    QWidget *container = QWidget::createWindowContainer(window);
    container->setMinimumSize(300, 300);
    container->setMaximumSize(600, 600);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(BobUI::StrongFocus);

    window->setGeometry(100, 100, 300, 200);

    layout->addWidget(new QLineEdit(QLatin1String("A QLineEdit")));
    layout->addWidget(container);
    layout->addWidget(new QLineEdit(QLatin1String("A QLabel")));

    widget->show();

    return app.exec();
}

#include "windowcontainer.moc"
