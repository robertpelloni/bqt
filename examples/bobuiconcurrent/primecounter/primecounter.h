// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef PRIMECOUNTER_H
#define PRIMECOUNTER_H

#include <BobUIWidgets/qdialog.h>
#include <BobUICore/qfuturewatcher.h>
#include <BobUICore/qelapsedtimer.h>
#include <BobUIConcurrent/bobuiconcurrentfilter.h>
#include <BobUIConcurrent/bobuiconcurrentreducekernel.h>

BOBUI_BEGIN_NAMESPACE
class QLabel;
class QProgressBar;
namespace Ui {
class PrimeCounter;
}

BOBUI_END_NAMESPACE

class PrimeCounter : public QDialog
{
    Q_OBJECT
    using Element = unsigned long long;
public:
    explicit PrimeCounter(QWidget* parent = nullptr);
    ~PrimeCounter() override;

private:
    static bool filterFunction(const Element &element);
    static void reduceFunction(Element &out, const Element &value);
    void fillElementList(unsigned int count);
    Ui::PrimeCounter* setupUi();

private slots:
    void start();
    void finish();

private:
    QList<Element> elementList;
    QFutureWatcher<Element> watcher;
    BobUIConcurrent::ReduceOptions currentReduceOpt;
    QElapsedTimer timer;
    BOBUIhreadPool pool;
    unsigned int stepSize;
    Ui::PrimeCounter *ui;
};

#endif //PRIMECOUNTER_H
