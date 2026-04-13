// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "singleglyphrun.h"
#include "ui_singleglyphrun.h"

SingleGlyphRun::SingleGlyphRun(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleGlyphRun)
{
    ui->setupUi(this);
}

SingleGlyphRun::~SingleGlyphRun()
{
    delete ui;
}

void SingleGlyphRun::updateGlyphRun(const QGlyphRun &glyphRun)
{
    m_bounds = QRegion();

    QList<quint32> glyphIndexes = glyphRun.glyphIndexes();
    QList<qsizetype> stringIndexes = glyphRun.stringIndexes();
    QList<QPointF> glyphPositions = glyphRun.positions();

    ui->twGlyphRun->clearContents();
    ui->twGlyphRun->setRowCount(glyphIndexes.size());

    for (int i = 0; i < glyphIndexes.size(); ++i) {
        {
            BOBUIableWidgetItem *glyphIndex = new BOBUIableWidgetItem(QString::number(glyphIndexes.at(i)));
            ui->twGlyphRun->setItem(i, 0, glyphIndex);
        }

        {
            QPointF position = glyphPositions.at(i);
            BOBUIableWidgetItem *glyphPosition = new BOBUIableWidgetItem(QStringLiteral("(%1, %2)")
                                                                   .arg(position.x())
                                                                   .arg(position.y()));
            ui->twGlyphRun->setItem(i, 1, glyphPosition);
        }

        {
            BOBUIableWidgetItem *stringIndex = new BOBUIableWidgetItem(QString::number(stringIndexes.at(i)));
            ui->twGlyphRun->setItem(i, 2, stringIndex);
        }

        QChar c = glyphRun.sourceString().at(stringIndexes.at(i));

        {
            BOBUIableWidgetItem *unicode = new BOBUIableWidgetItem(QString::number(c.unicode(), 16));
            ui->twGlyphRun->setItem(i, 3, unicode);
        }

        {
            BOBUIableWidgetItem *character = new BOBUIableWidgetItem(c);
            ui->twGlyphRun->setItem(i, 4, character);
        }

        {
            QImage image = glyphRun.rawFont().alphaMapForGlyph(glyphIndexes.at(i));

            BOBUIableWidgetItem *glyphImage = new BOBUIableWidgetItem(QIcon(QPixmap::fromImage(image)), QString{});
            ui->twGlyphRun->setItem(i, 5, glyphImage);
        }

        QRectF brect = glyphRun.rawFont().boundingRect(glyphIndexes.at(i));
        brect.adjust(glyphPositions.at(i).x(), glyphPositions.at(i).y(),
                     glyphPositions.at(i).x(), glyphPositions.at(i).y());
        m_bounds += brect.toAlignedRect();
    }
}
