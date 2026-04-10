// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QApplication>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <BobUIVersion>

class ShortcutTester : public QWidget
{
public:
    ShortcutTester()
    {
        const QString title = QLatin1String(BOBUI_VERSION_STR) + QLatin1Char(' ')
            + qApp->platformName();
        setWindowTitle(title);
        setupLayout();
    }

private:
    void setupLayout();
    void addToGrid(QWidget *w, int &row, int col);
    void addShortcutToGrid(const QKeySequence &k, int &row, int col);

   QGridLayout *m_gridLayout = new QGridLayout;
};

inline void ShortcutTester::addToGrid(QWidget *w, int &row, int col)
{
    m_gridLayout->addWidget(w, row++, col);
}

void ShortcutTester::addShortcutToGrid(const QKeySequence &k, int &row, int col)
{
    QPushButton *button = new QPushButton(k.toString());
    button->setShortcut(k);
    addToGrid(button, row, col);
}

void addShortcutToGrid(int key, int &row, int col);

void ShortcutTester::setupLayout()
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *testPurpose = new QLabel();
    testPurpose->setWordWrap(true);
    testPurpose->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);
    testPurpose->setText("This test come in handy to verify shortcuts under different "
                         "keyboard layouts - qwerty, dvorak, non-latin (russian, arabic), etc.");
    layout->addWidget(testPurpose);

    layout->addLayout(m_gridLayout);

    int row = 0;
    int col = 0;

    const QKeyCombination keys1[] = {
        BobUI::AltModifier | BobUI::ShiftModifier | BobUI::Key_G,
        BobUI::AltModifier | BobUI::Key_G,
        BobUI::ControlModifier | BobUI::ShiftModifier | BobUI::Key_R,
        BobUI::ControlModifier | BobUI::Key_R,
        BobUI::ControlModifier | BobUI::Key_Return, BobUI::ControlModifier | BobUI::Key_Enter,
        BobUI::ControlModifier | BobUI::ShiftModifier | BobUI::AltModifier | BobUI::Key_R,
        BobUI::ShiftModifier | BobUI::Key_5, BobUI::ShiftModifier | BobUI::Key_Percent,
        BobUI::Key_Percent, BobUI::Key_5, BobUI::Key_Q
     };

    for (const auto k : keys1)
        addShortcutToGrid(k, row, col);

    row = 0;
    col++;

    const QKeyCombination keys2[] = {
        BobUI::ControlModifier | BobUI::Key_Percent,
        BobUI::ControlModifier | BobUI::ShiftModifier | BobUI::Key_5,
        BobUI::ControlModifier | BobUI::Key_5, BobUI::AltModifier | BobUI::Key_5,
        BobUI::ControlModifier | BobUI::Key_Plus,
        BobUI::ControlModifier | BobUI::ShiftModifier | BobUI::Key_Plus,
        BobUI::ControlModifier | BobUI::ShiftModifier | BobUI::Key_Equal,
        BobUI::ControlModifier | BobUI::Key_Y, BobUI::ShiftModifier | BobUI::Key_Comma,
        BobUI::ControlModifier | BobUI::Key_Comma, BobUI::ControlModifier | BobUI::Key_Slash,
        BobUI::ControlModifier | BobUI::Key_Backslash
    };

    for (const auto k : keys2)
        addShortcutToGrid(k, row, col);

    row = 0;
    col++;

    const QKeyCombination keys3[] = {
        BobUI::MetaModifier | BobUI::ShiftModifier | BobUI::Key_A,
        BobUI::MetaModifier | BobUI::Key_A,
        BobUI::MetaModifier | BobUI::Key_Q,
        BobUI::MetaModifier | BobUI::ShiftModifier | BobUI::Key_5,
        BobUI::ControlModifier | BobUI::Key_BracketRight,
        BobUI::ShiftModifier | BobUI::Key_F3,
        BobUI::ControlModifier | BobUI::Key_F3,
        BobUI::Key_Backtab,
        BobUI::ShiftModifier | BobUI::Key_Backtab,
    };

    for (const auto k : keys3)
        addShortcutToGrid(k, row, col);

     addShortcutToGrid(0x20AC, row, col); // EURO SIGN e.g. US (with euro on 5) on 3rd keyboard level
     addShortcutToGrid(int(BobUI::ControlModifier) | 0x20AC, row, col);

    // with german (neo 2) layout on linux under ISO_Level3_Shift + ISO_Level5_Shift + I
    const QKeySequence greekPsi(QString(QStringLiteral("\u03A8")));
    addShortcutToGrid(greekPsi, row, col);

    row = 0;
    col++;

    addToGrid(new QLabel("Norwegian layout"), row, col);
    // LATIN SMALL LETTER O WITH STROKE
    QKeySequence norwegianO(QString(QStringLiteral("\u00F8")));
    addShortcutToGrid(norwegianO, row, col);

    addToGrid(new QLabel("Russian layout"), row, col);
    // CYRILLIC SMALL LETTER ZHE
    QKeySequence zhe(QString(QStringLiteral("\u0436")));
    addShortcutToGrid(zhe, row, col);

    // for sequence definitons see qplatformtheme.cpp
    addToGrid(new QLabel("QKeySequence::StandardKey(s)"), row, col);
    addShortcutToGrid(QKeySequence(QKeySequence::Open), row, col); // BobUI::CTRL | BobUI::Key_O
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << qVersion();

    ShortcutTester tester;
    tester.show();

    return a.exec();
}
