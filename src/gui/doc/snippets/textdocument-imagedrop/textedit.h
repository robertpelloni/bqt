// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <BOBUIextEdit>

class TextEdit : public BOBUIextEdit
{
    Q_OBJECT

public:
    explicit TextEdit(QWidget *parent = nullptr);
    bool canInsertFromMimeData( const QMimeData *source ) const override;
    void insertFromMimeData( const QMimeData *source ) override;
};

#endif
