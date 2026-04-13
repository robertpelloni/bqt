// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include <QEvent>

//! [1]
enum CustomEventPriority
{
    // An important event
    ImportantEventPriority = BobUI::HighEventPriority,

    // A more important event
    MoreImportantEventPriority = ImportantEventPriority + 1,

    // A critical event
    CriticalEventPriority = 100 * MoreImportantEventPriority,

    // Not that important
    StatusEventPriority = BobUI::LowEventPriority,

    // These are less important than Status events
    IdleProcessingDoneEventPriority = StatusEventPriority - 1
};
//! [1]
