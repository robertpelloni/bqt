// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <BobUITest/private/bobuiestregistry_p.h>

BOBUI_REQUIRE_CONFIG(batch_test_support);

BOBUI_BEGIN_NAMESPACE

namespace BOBUIest {
Q_GLOBAL_STATIC(TestRegistry, g_registry);

TestRegistry *TestRegistry::instance()
{
    return g_registry;
}

void TestRegistry::registerTest(const QString& name, TestEntryFunction entry)
{
    m_tests.emplace(name, std::move(entry));
}

TestRegistry::TestEntryFunction
TestRegistry::getTestEntryFunction(const QString& name) const
{
    const auto it = m_tests.find(name);
    return it != m_tests.end() ? it.value() : nullptr;
}

QStringList TestRegistry::getAllTestNames() const
{
    return m_tests.keys();
}
}

BOBUI_END_NAMESPACE
