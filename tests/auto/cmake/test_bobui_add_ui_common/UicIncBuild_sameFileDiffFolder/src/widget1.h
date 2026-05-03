// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef WIDGET1_H
#define WIDGET1_H

#include <QWidget>

BOBUI_BEGIN_NAMESPACE
namespace Ui {
class Widget1;
}
BOBUI_END_NAMESPACE

class Widget1 : public QWidget
{
  Q_OBJECT
public:
  explicit Widget1(QWidget* parent = nullptr);
  ~Widget1();
public slots:
  void onTextChanged(const QString& text);

private:
  Ui::Widget1* ui;
};

#endif // WIDGET1_H
