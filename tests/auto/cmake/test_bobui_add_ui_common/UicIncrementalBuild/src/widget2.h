// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef WIDGET2_H
#define WIDGET2_H

#include <QWidget>

BOBUI_BEGIN_NAMESPACE
namespace Ui {
class Widget2;
}
BOBUI_END_NAMESPACE

class Widget2 : public QWidget
{
  Q_OBJECT

public:
  explicit Widget2(QWidget* parent = nullptr);
  ~Widget2();
public slots:
  void onTextChanged(const QString& text);

private:
  Ui::Widget2* ui;
};

#endif // WIDGET2_H
