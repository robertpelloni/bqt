// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUISql>

int main(int argc, char ** argv)
{
   QCoreApplication app(argc, argv);
   QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
   return db.isValid() ? 0 : 1;
}
