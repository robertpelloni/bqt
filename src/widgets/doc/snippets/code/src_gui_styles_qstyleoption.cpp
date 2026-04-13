// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

//! [0]
int MyStyle::styleHint(StyleHint stylehint, const QStyleOption *opt,
                       const QWidget *widget, QStyleHintReturn* returnData) const;
{
    if (stylehint == SH_RubberBand_Mask) {
        const QStyleHintReturnMask *maskReturn =
                qstyleoption_cast<const QStyleHintReturnMask *>(hint);
        if (maskReturn) {
            ...
        }
    }
    ...
}
//! [0]
