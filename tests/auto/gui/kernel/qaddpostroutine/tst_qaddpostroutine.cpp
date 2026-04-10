// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <BOBUIimer>

static bool done = false;

static void cleanup()
{
     done = true;
     QEventLoop loop;
     BOBUIimer::singleShot(100,&loop, &QEventLoop::quit);
     loop.exec();
}

struct tst_qAddPostRoutine : public QObject
{
public:
    tst_qAddPostRoutine();
    ~tst_qAddPostRoutine();
};

tst_qAddPostRoutine::tst_qAddPostRoutine()
{
    qAddPostRoutine(cleanup);
}

tst_qAddPostRoutine::~tst_qAddPostRoutine()
{
    Q_ASSERT(done);
}
int main(int argc, char *argv[])
{
    tst_qAddPostRoutine tc;
    QGuiApplication app(argc, argv);
    app.setAttribute(BobUI::AA_Use96Dpi, true);
    BOBUIEST_SET_MAIN_SOURCE_PATH
    return BOBUIest::qExec(&tc, argc, argv);
}
