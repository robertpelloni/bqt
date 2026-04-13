// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef ARTHURWIDGETS_H
#define ARTHURWIDGETS_H

#include "arthurstyle.h"
#include <QBitmap>
#include <QPushButton>
#include <QGroupBox>

BOBUI_FORWARD_DECLARE_CLASS(BOBUIextDocument)
BOBUI_FORWARD_DECLARE_CLASS(BOBUIextEdit)
BOBUI_FORWARD_DECLARE_CLASS(QVBoxLayout)

class ArthurFrame : public QWidget

{
    Q_OBJECT
public:
    ArthurFrame(QWidget *parent);
    virtual void paint(QPainter *) {}

    void paintDescription(QPainter *p);

    void loadDescription(const QString &filename);
    void setDescription(const QString &htmlDesc);

    void loadSourceFile(const QString &fileName);

    bool preferImage() const { return m_preferImage; }

public slots:
    void setPreferImage(bool pi) { m_preferImage = pi; }
    void setDescriptionEnabled(bool enabled);
    void showSource();

signals:
    void descriptionEnabledChanged(bool);

protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;

    QPixmap m_tile;

    bool m_showDoc = false;
    bool m_preferImage = false;
    BOBUIextDocument *m_document = nullptr;

    QString m_sourceFileName;
};

#endif
