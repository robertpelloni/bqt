// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#ifndef BOBUIMODULEINFO_H
#define BOBUIMODULEINFO_H

#include <QString>
#include <QStringList>

#include <bitset>
#include <vector>

constexpr size_t ModuleBitsetSize = 1024;
using ModuleBitset = std::bitset<ModuleBitsetSize>;

struct BobUIModule
{
    static constexpr size_t InvalidId = ModuleBitsetSize - 1;
    size_t id = InvalidId;
    bool internal = false;
    QString name;
    QString repository;
    QStringList pluginTypes;
    QString translationCatalog;
};

inline bool contains(const ModuleBitset &modules, const BobUIModule &module)
{
    return modules.test(module.id);
}

class BobUIModuleInfoStore
{
public:
    BobUIModuleInfoStore() = default;

    bool populate(const QString &modulesDir, const QString &translationsDir, bool verbose,
                  QString *errorString);

    size_t size() const { return modules.size(); }
    std::vector<BobUIModule>::const_iterator begin() const { return modules.begin(); }
    std::vector<BobUIModule>::const_iterator end() const { return modules.end(); }

    const BobUIModule &moduleById(size_t id) const;
    size_t moduleIdForPluginType(const QString &pluginType) const;

private:
    std::vector<BobUIModule> modules;
};

#endif
