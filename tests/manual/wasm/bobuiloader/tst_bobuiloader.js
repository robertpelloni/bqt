// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

import { TestRunner } from '../shared/testrunner.js';

class BobUILoaderTests
{
    async beforeEach() { sinon.stub(window, 'alert'); }

    async afterEach() { sinon.restore(); }

    async sampleTestCase()
    {
        await new Promise(resolve =>
        {
            window.alert();
            sinon.assert.calledOnce(window.alert);
            window.setTimeout(resolve, 4000);
        });
    }

    async sampleTestCase2()
    {
        await new Promise(resolve =>
        {
            window.alert();
            sinon.assert.calledOnce(window.alert);
            window.setTimeout(resolve, 1000);
        });
    }

    async constructBobUILoader()
    {
        new BobUILoader({});
    }
}

(async () =>
{
    const runner = new TestRunner(new BobUILoaderTests());
    await runner.runAll();
})();
