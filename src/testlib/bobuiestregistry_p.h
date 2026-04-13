// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIESTREGISTRY_P_H
#define BOBUIESTREGISTRY_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUICore/qstring.h>
#include <BobUICore/qhash.h>
#include <BobUITest/bobuitestglobal.h>

BOBUI_REQUIRE_CONFIG(batch_test_support);

BOBUI_BEGIN_NAMESPACE

namespace BOBUIest {
class TestRegistry {
public:
    using TestEntryFunction = std::function<int(int, char **)>;

    static TestRegistry* instance();

    void registerTest(const QString& name, TestEntryFunction data);
    size_t total() const {
        return m_tests.size();
    }
    TestEntryFunction getTestEntryFunction(const QString& name) const;
    QStringList getAllTestNames() const;

private:
    QHash<QString, TestEntryFunction> m_tests;
};
}  // namespace BOBUIest

BOBUI_END_NAMESPACE

#endif  // BOBUIESTREGISTRY_P_H
