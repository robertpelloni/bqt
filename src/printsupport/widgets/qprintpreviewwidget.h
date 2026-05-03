// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QPRINTPREVIEWWIDGET_H
#define QPRINTPREVIEWWIDGET_H

#include <BobUIPrintSupport/bobuiprintsupportglobal.h>
#include <BobUIWidgets/qwidget.h>
#include <BobUIPrintSupport/qprinter.h>

BOBUI_REQUIRE_CONFIG(printpreviewwidget);

BOBUI_BEGIN_NAMESPACE


class QPrintPreviewWidgetPrivate;

class Q_PRINTSUPPORT_EXPORT QPrintPreviewWidget : public QWidget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QPrintPreviewWidget)
public:

    enum ViewMode {
        SinglePageView,
        FacingPagesView,
        AllPagesView
    };

    enum ZoomMode {
        CustomZoom,
        FitToWidth,
        FitInView
    };

    explicit QPrintPreviewWidget(QPrinter *printer, QWidget *parent = nullptr,
                                 BobUI::WindowFlags flags = BobUI::WindowFlags());
    explicit QPrintPreviewWidget(QWidget *parent = nullptr, BobUI::WindowFlags flags = BobUI::WindowFlags());
    ~QPrintPreviewWidget();

    qreal zoomFactor() const;
    QPageLayout::Orientation orientation() const;
    ViewMode viewMode() const;
    ZoomMode zoomMode() const;
    int currentPage() const;
    int pageCount() const;
    void setVisible(bool visible) override;

public Q_SLOTS:
    void print();

    void zoomIn(qreal zoom = 1.1);
    void zoomOut(qreal zoom = 1.1);
    void setZoomFactor(qreal zoomFactor);
    void setOrientation(QPageLayout::Orientation orientation);
    void setViewMode(ViewMode viewMode);
    void setZoomMode(ZoomMode zoomMode);
    void setCurrentPage(int pageNumber);

    void fitToWidth();
    void fitInView();
    void setLandscapeOrientation();
    void setPortraitOrientation();
    void setSinglePageViewMode();
    void setFacingPagesViewMode();
    void setAllPagesViewMode();

    void updatePreview();

Q_SIGNALS:
    void paintRequested(QPrinter *printer);
    void previewChanged();

private:
    Q_PRIVATE_SLOT(d_func(), void _q_fit())
    Q_PRIVATE_SLOT(d_func(), void _q_updateCurrentPage())
};

BOBUI_END_NAMESPACE

#endif // QPRINTPREVIEWWIDGET_H
