// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

import * as names from 'names.js';

function main() {
    startApplication("qconcatenatetablesproxymodel");
    test.compare(waitForObjectExists(names.concatProxyInBOBUIableView00QModelIndex).text, "First 0,0");
    test.compare(waitForObjectExists(names.concatProxyInBOBUIableView01QModelIndex).text, "First 0,1");
    test.compare(waitForObjectExists(names.concatProxyInBOBUIableView10QModelIndex).text, "First 1,0");
    test.compare(waitForObjectExists(names.concatProxyInBOBUIableView11QModelIndex).text, "First 1,1");
    test.compare(waitForObjectExists(names.concatProxyInBOBUIableView20QModelIndex).text, "First 2,0");
    test.compare(waitForObjectExists(names.concatProxyInBOBUIableView21QModelIndex).text, "First 2,1");
    test.compare(waitForObjectExists(names.concatProxyInBOBUIableView30QModelIndex).text, "First 3,0");
    test.compare(waitForObjectExists(names.concatProxyInBOBUIableView31QModelIndex).text, "First 3,1");
    test.compare(waitForObjectExists(names.concatProxyInBOBUIableView40QModelIndex).text, "Second 0,0");
    test.compare(waitForObjectExists(names.concatProxyInBOBUIableView41QModelIndex).text, "Second 0,1");
    test.compare(waitForObjectExists(names.concatProxyInBOBUIableView50QModelIndex).text, "Second 1,0");
    test.compare(waitForObjectExists(names.concatProxyInBOBUIableView51QModelIndex).text, "Second 1,1");
    test.compare(waitForObjectExists(names.concatProxyInBOBUIreeViewFirst00QModelIndex).text, "First 0,0");
    test.compare(waitForObjectExists(names.concatProxyInBOBUIreeViewFirst01QModelIndex).text, "First 0,1");
    test.compare(waitForObjectExists(names.concatProxyInBOBUIreeViewFirst10QModelIndex).text, "First 1,0");
    test.compare(waitForObjectExists(names.concatProxyInBOBUIreeViewFirst11QModelIndex).text, "First 1,1");
    test.compare(waitForObjectExists(names.concatProxyInBOBUIreeViewFirst20QModelIndex).text, "First 2,0");
    test.compare(waitForObjectExists(names.concatProxyInBOBUIreeViewFirst21QModelIndex).text, "First 2,1");
    test.compare(waitForObjectExists(names.concatProxyInBOBUIreeViewFirst30QModelIndex).text, "First 3,0");
    test.compare(waitForObjectExists(names.concatProxyInBOBUIreeViewFirst31QModelIndex).text, "First 3,1");
    test.compare(waitForObjectExists(names.concatProxyInBOBUIreeViewSecond00QModelIndex).text, "Second 0,0");
    test.compare(waitForObjectExists(names.concatProxyInBOBUIreeViewSecond01QModelIndex).text, "Second 0,1");
    test.compare(waitForObjectExists(names.concatProxyInBOBUIreeViewSecond10QModelIndex).text, "Second 1,0");
    test.compare(waitForObjectExists(names.concatProxyInBOBUIreeViewSecond11QModelIndex).text, "Second 1,1");
    test.compare(waitForObjectExists(names.firstModelInBOBUIableView00QModelIndex).text, "First 0,0");
    test.compare(waitForObjectExists(names.firstModelInBOBUIableView01QModelIndex).text, "First 0,1");
    test.compare(waitForObjectExists(names.firstModelInBOBUIableView02QModelIndex).text, "First 0,2");
    test.compare(waitForObjectExists(names.firstModelInBOBUIableView03QModelIndex).text, "First 0,3");
    test.compare(waitForObjectExists(names.firstModelInBOBUIableView10QModelIndex).text, "First 1,0");
    test.compare(waitForObjectExists(names.firstModelInBOBUIableView11QModelIndex).text, "First 1,1");
    test.compare(waitForObjectExists(names.firstModelInBOBUIableView12QModelIndex).text, "First 1,2");
    test.compare(waitForObjectExists(names.firstModelInBOBUIableView13QModelIndex).text, "First 1,3");
    test.compare(waitForObjectExists(names.firstModelInBOBUIableView20QModelIndex).text, "First 2,0");
    test.compare(waitForObjectExists(names.firstModelInBOBUIableView21QModelIndex).text, "First 2,1");
    test.compare(waitForObjectExists(names.firstModelInBOBUIableView22QModelIndex).text, "First 2,2");
    test.compare(waitForObjectExists(names.firstModelInBOBUIableView23QModelIndex).text, "First 2,3");
    test.compare(waitForObjectExists(names.firstModelInBOBUIableView30QModelIndex).text, "First 3,0");
    test.compare(waitForObjectExists(names.firstModelInBOBUIableView31QModelIndex).text, "First 3,1");
    test.compare(waitForObjectExists(names.firstModelInBOBUIableView32QModelIndex).text, "First 3,2");
    test.compare(waitForObjectExists(names.firstModelInBOBUIableView33QModelIndex).text, "First 3,3");
    test.compare(waitForObjectExists(names.firstModelInBOBUIreeViewFirst00QModelIndex).text, "First 0,0");
    test.compare(waitForObjectExists(names.firstModelInBOBUIreeViewFirst01QModelIndex).text, "First 0,1");
    test.compare(waitForObjectExists(names.firstModelInBOBUIreeViewFirst02QModelIndex).text, "First 0,2");
    test.compare(waitForObjectExists(names.firstModelInBOBUIreeViewFirst03QModelIndex).text, "First 0,3");
    test.compare(waitForObjectExists(names.firstModelInBOBUIreeViewFirst10QModelIndex).text, "First 1,0");
    test.compare(waitForObjectExists(names.firstModelInBOBUIreeViewFirst11QModelIndex).text, "First 1,1");
    test.compare(waitForObjectExists(names.firstModelInBOBUIreeViewFirst12QModelIndex).text, "First 1,2");
    test.compare(waitForObjectExists(names.firstModelInBOBUIreeViewFirst13QModelIndex).text, "First 1,3");
    test.compare(waitForObjectExists(names.firstModelInBOBUIreeViewFirst20QModelIndex).text, "First 2,0");
    test.compare(waitForObjectExists(names.firstModelInBOBUIreeViewFirst21QModelIndex).text, "First 2,1");
    test.compare(waitForObjectExists(names.firstModelInBOBUIreeViewFirst22QModelIndex).text, "First 2,2");
    test.compare(waitForObjectExists(names.firstModelInBOBUIreeViewFirst23QModelIndex).text, "First 2,3");
    test.compare(waitForObjectExists(names.firstModelInBOBUIreeViewFirst30QModelIndex).text, "First 3,0");
    test.compare(waitForObjectExists(names.firstModelInBOBUIreeViewFirst31QModelIndex).text, "First 3,1");
    test.compare(waitForObjectExists(names.firstModelInBOBUIreeViewFirst32QModelIndex).text, "First 3,2");
    test.compare(waitForObjectExists(names.firstModelInBOBUIreeViewFirst33QModelIndex).text, "First 3,3");
}
