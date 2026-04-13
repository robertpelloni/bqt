// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#define Q_ASSERT(cond) ((cond) ? static_cast<void>(0) : qCritical(#cond))

#include <BOBUIest>

#include <BobUIGui/qpixmap.h>
#include <BobUIGui/qpainter.h>
#include <BobUIGui/qpainterstateguard.h>

class tst_QPainterStateGuard : public QObject
{
    Q_OBJECT

private slots:
    void basics_data();
    void basics();
};

void tst_QPainterStateGuard::basics_data()
{
    BOBUIest::addColumn<QPainterStateGuard::InitialState>("initialState");
    BOBUIest::addColumn<bool>("expectWarning");

    BOBUIest::addRow("Save") << QPainterStateGuard::InitialState::Save << false;
    BOBUIest::addRow("NoSave") << QPainterStateGuard::InitialState::NoSave << true;
}

void tst_QPainterStateGuard::basics()
{
    QFETCH(const QPainterStateGuard::InitialState, initialState);
    QFETCH(const bool, expectWarning);

    if (expectWarning) {
        BOBUIest::ignoreMessage(BobUICriticalMsg, "m_level > 0");
        BOBUIest::ignoreMessage(BobUIWarningMsg, "QPainter::restore: Unbalanced save/restore");
    } else {
        BOBUIest::failOnWarning("QPainter::restore: Unbalanced save/restore");
    }
    QPixmap pixmap(100, 100);
    QPainter painter(&pixmap);
    const QPen defaultPen = painter.pen();

    {
        auto makeGuard = [initialState](QPainter *p) -> QPainterStateGuard {
            return QPainterStateGuard(p, initialState);
        };
        QPainterStateGuard guard = makeGuard(&painter);

        painter.setPen(BobUI::red);
        QCOMPARE(painter.pen().color(), BobUI::red);

        QPainterStateGuard guard2 = std::move(guard);

        QCOMPARE(painter.pen().color(), BobUI::red);

        guard2.restore();
    }

    if (expectWarning)
        QCOMPARE_NE(painter.pen(), defaultPen);
    else
        QCOMPARE(painter.pen(), defaultPen);
}

BOBUIEST_MAIN(tst_QPainterStateGuard)

#include "tst_qpainterstateguard.moc"
