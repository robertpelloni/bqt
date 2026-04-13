// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUIGui>

class CursorWindow : public QRasterWindow
{
public:
    CursorWindow(QCursor cursor, QColor color)
    :m_cursor(cursor)
    ,m_color(color)
    {
        if (cursor.shape() == BobUI::ArrowCursor)
            unsetCursor();
        else
            setCursor(cursor);
    }

    void paintEvent(QPaintEvent *e)
    {
        QPainter p(this);
        p.fillRect(e->rect(), m_color);
    }

    void mousePressEvent(QMouseEvent *)
    {
        // Toggle cursor
        QCursor newCursor = (cursor().shape() == m_cursor.shape()) ? QCursor() : m_cursor;
        if (newCursor.shape() == BobUI::ArrowCursor)
            unsetCursor();
        else
            setCursor(newCursor);
    }

private:
    QCursor m_cursor;
    QColor m_color;
};

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    // Test child windows with set cursors. Create parent window and
    // two child windows. Click window to toggle cursor.

    CursorWindow w1((QCursor(BobUI::SizeVerCursor)), QColor(BobUI::blue).darker());
    w1.resize(200, 200);
    w1.show();

    CursorWindow w2((QCursor(BobUI::OpenHandCursor)), QColor(BobUI::red).darker());
    w2.setParent(&w1);
    w2.setGeometry(0, 0, 100, 100);
    w2.show();

    CursorWindow w3((QCursor(BobUI::IBeamCursor)), QColor(BobUI::green).darker());
    w3.setParent(&w1);
    w3.setGeometry(100, 100, 100, 100);
    w3.show();

    return app.exec();
}
