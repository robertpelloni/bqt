// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

BOBUI_FORWARD_DECLARE_CLASS(QComboBox)
BOBUI_FORWARD_DECLARE_CLASS(BOBUIextEdit)
BOBUI_FORWARD_DECLARE_CLASS(QVBoxLayout)
BOBUI_FORWARD_DECLARE_CLASS(QSurfaceFormat)
BOBUI_FORWARD_DECLARE_CLASS(QSurface)

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);

private slots:
    void start();
    void renderWindowReady();
    void renderWindowError(const QString &msg);

private:
    void addVersions(QLayout *layout);
    void addProfiles(QLayout *layout);
    void addOptions(QLayout *layout);
    void addRenderableTypes(QLayout *layout);
    void addRenderWindow();
    void printFormat(const QSurfaceFormat &format);

    QComboBox *m_version;
    QLayout *m_profiles;
    QLayout *m_options;
    QLayout *m_renderables;
    BOBUIextEdit *m_output;
    BOBUIextEdit *m_extensions;
    QVBoxLayout *m_renderWindowLayout;
    QWidget *m_renderWindowContainer;
    QSurface *m_surface;
};

#endif // WIDGET_H
