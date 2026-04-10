// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QGraphicsSceneDragDropEvent>

class GraphicsSceneEvent : public QGraphicsSceneDragDropEvent
{
public:
    void exampleFunction() {
        //! [0]
        setDropAction(proposedAction());
        //! [0]
    }
};
