#include "OmniSplitView.h"

OmniSplitView::OmniSplitView(QWidget *parent) : QSplitter(parent) {
    setObjectName("OmniSplitView");
    // qDebug() << "Initialized OmniSplitView";
}

OmniSplitView::~OmniSplitView() = default;
