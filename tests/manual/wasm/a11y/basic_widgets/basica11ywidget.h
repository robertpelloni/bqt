// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUIWidgets>
#include "tabswidget.h"

class BasicA11yWidget: public QWidget
{
    Q_OBJECT
private:
    QMenu* m_helpMenu = nullptr;
    QMenu* m_TodayMenu = nullptr;
    QMenuBar* m_menuBar = nullptr;
    BOBUIoolBar* m_toolBar = nullptr;
    QLabel* m_lblDateTime = nullptr;
    QVBoxLayout* m_layout = nullptr ;
    BOBUIabWidget* m_tabWidget = nullptr;
    EditViewTab *m_editView = nullptr;

    QAction* m_DateAct = nullptr;
    QAction* m_TimeAct = nullptr;
    QAction* m_aboutAct = nullptr;
    QAction* m_copyAct = nullptr;
    QAction* m_pasteAct = nullptr;
    QAction* m_cutAct = nullptr;

public slots:
    void connectToolBar();
public:
    BasicA11yWidget() ;
    void createActions();
    void createMenus();
    void createToolBar();

    void todaysDate();
    void currentTime();
    void about();
    BOBUIoolBar* getToolbar(){return m_toolBar;}
    void handleButton();

};
