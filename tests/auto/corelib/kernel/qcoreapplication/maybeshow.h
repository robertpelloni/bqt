// Copyright (C) 2024 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#if defined(BOBUI_WIDGETS_LIB)
#include <BobUIWidgets/QDialog>
#elif defined(BOBUI_GUI_LIB)
#include <BobUIGui/QWindow>
#endif

inline auto maybeShowSomething()
{
#if defined(BOBUI_WIDGETS_LIB)
    auto w = std::make_unique<QDialog>();
    w->setModal(true);
    w->show();
#elif defined(BOBUI_GUI_LIB)
    auto w = std::make_unique<QWindow>();
    w->setModality(BobUI::ApplicationModal);
    w->show();
#else
    void *w = nullptr;
#endif
    return w;
}

