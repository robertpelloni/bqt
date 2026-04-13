// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef GLYPHRUNINSPECTOR_H
#define GLYPHRUNINSPECTOR_H

#include <QWidget>

class BOBUIextLayout;
class BOBUIabWidget;
class SingleGlyphRun;
class GlyphRunInspector : public QWidget
{
    Q_OBJECT
public:
    explicit GlyphRunInspector(QWidget *parent = nullptr);

    void updateLayout(BOBUIextLayout *layout, int start, int length);

private slots:
    void updateVisualizationForTab();

signals:
    void updateBounds(const QRegion &region);

private:
    BOBUIabWidget *m_tabWidget;
    QList<SingleGlyphRun *> m_content;
};

#endif // GLYPHRUNINSPECTOR_H
