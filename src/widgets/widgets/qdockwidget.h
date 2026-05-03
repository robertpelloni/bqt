// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QDYNAMICDOCKWIDGET_H
#define QDYNAMICDOCKWIDGET_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUIWidgets/qwidget.h>

BOBUI_REQUIRE_CONFIG(dockwidget);

BOBUI_BEGIN_NAMESPACE

class QDockAreaLayout;
class QDockWidgetPrivate;
class QMainWindow;
class QStyleOptionDockWidget;

class Q_WIDGETS_EXPORT QDockWidget : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(bool floating READ isFloating WRITE setFloating NOTIFY topLevelChanged)
    Q_PROPERTY(DockWidgetFeatures features READ features WRITE setFeatures NOTIFY featuresChanged)
    Q_PROPERTY(BobUI::DockWidgetAreas allowedAreas READ allowedAreas
               WRITE setAllowedAreas NOTIFY allowedAreasChanged)
    Q_PROPERTY(QString windowTitle READ windowTitle WRITE setWindowTitle DESIGNABLE true)
    Q_PROPERTY(BobUI::DockWidgetArea dockLocation READ dockLocation WRITE setDockLocation
               NOTIFY dockLocationChanged)

public:
    explicit QDockWidget(const QString &title, QWidget *parent = nullptr,
                         BobUI::WindowFlags flags = BobUI::WindowFlags());
    explicit QDockWidget(QWidget *parent = nullptr, BobUI::WindowFlags flags = BobUI::WindowFlags());
    ~QDockWidget();

    QWidget *widget() const;
    void setWidget(QWidget *widget);

    enum DockWidgetFeature {
        DockWidgetClosable    = 0x01,
        DockWidgetMovable     = 0x02,
        DockWidgetFloatable   = 0x04,
        DockWidgetVerticalTitleBar = 0x08,

        DockWidgetFeatureMask = 0x0f,
        NoDockWidgetFeatures  = 0x00,

        Reserved              = 0xff
    };
    Q_DECLARE_FLAGS(DockWidgetFeatures, DockWidgetFeature)
    Q_FLAG(DockWidgetFeatures)

    void setFeatures(DockWidgetFeatures features);
    DockWidgetFeatures features() const;

    void setFloating(bool floating);
    inline bool isFloating() const { return isWindow(); }

    void setAllowedAreas(BobUI::DockWidgetAreas areas);
    BobUI::DockWidgetAreas allowedAreas() const;

    void setTitleBarWidget(QWidget *widget);
    QWidget *titleBarWidget() const;

    void setDockLocation(BobUI::DockWidgetArea area);
    BobUI::DockWidgetArea dockLocation() const;

    inline bool isAreaAllowed(BobUI::DockWidgetArea area) const
    { return (allowedAreas() & area) == area; }

#ifndef BOBUI_NO_DEBUG_STREAM
    friend Q_WIDGETS_EXPORT QDebug operator<<(QDebug dbg, const QDockWidget *dockWidget);
#endif

#ifndef BOBUI_NO_ACTION
    QAction *toggleViewAction() const;
#endif

Q_SIGNALS:
    void featuresChanged(QDockWidget::DockWidgetFeatures features);
    void topLevelChanged(bool topLevel);
    void allowedAreasChanged(BobUI::DockWidgetAreas allowedAreas);
    void visibilityChanged(bool visible); // ### BobUI7: Deprecate this. Better listen to hide/show events
    void dockLocationChanged(BobUI::DockWidgetArea area);

protected:
    void changeEvent(QEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    bool event(QEvent *event) override;
    virtual void initStyleOption(QStyleOptionDockWidget *option) const;

private:
    Q_DECLARE_PRIVATE(QDockWidget)
    Q_DISABLE_COPY(QDockWidget)
    friend class QDockAreaLayout;
    friend class QDockWidgetItem;
    friend class QMainWindowLayout;
    friend class QDockWidgetLayout;
    friend class QDockAreaLayoutInfo;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QDockWidget::DockWidgetFeatures)

BOBUI_END_NAMESPACE

#endif // QDYNAMICDOCKWIDGET_H
