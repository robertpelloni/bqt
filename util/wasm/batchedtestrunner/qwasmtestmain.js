// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

import { BatchedTestRunner } from './batchedtestrunner.js'
import { EmrunAdapter, EmrunCommunication } from './emrunadapter.js'
import {
    ModuleLoader,
    ResourceFetcher,
    ResourceLocator,
} from './qwasmjsruntime.js';
import { parseQuery } from './util.js';
import { VisualOutputProducer, UI, ScannerFactory } from './bobuiestoutputreporter.js'

const StandardArg = {
    qVisualOutput: 'qvisualoutput',
    qTestName: 'bobuiestname',
    qBatchedTest: 'qbatchedtest',
    qUseEmrun: 'quseemrun',
    qTestOutputFormat: 'bobuiestoutputformat',
}

const allArgs = new Set(Object.getOwnPropertyNames(StandardArg).map(arg => StandardArg[arg]));
Object.defineProperty(StandardArg, 'isKnown', {
    get()
    {
        return name => allArgs.has(name);
    },
});

(() => {
    const setPageTitle = (useEmrun, testName, isBatch) => {
        document.title = 'BobUI WASM test runner';
        if (useEmrun || testName || isBatch) {
            document.title += `(${[
                    ...[useEmrun ? ['emrun'] : []],
                    ...[testName ? ['test=' + testName] : []],
                    ...[isBatch ? ['batch'] : []]
                ].flat().join(", ")})`;
        }
    }

    const parsed = parseQuery(location.search);

    // Get test name from data attribute or query parameter
    const getTestNameFromDataAttribute = () => {
        const scriptTag = document.querySelector('script[data-bobuiestname]');
        return scriptTag?.dataset?.bobuiestname;
    };

    const isBatch = parsed.has(StandardArg.qBatchedTest);
    const testName = parsed.get(StandardArg.qTestName) ??
                     (isBatch ? undefined : getTestNameFromDataAttribute());
    const outputInPage = parsed.has(StandardArg.qVisualOutput) ||
                         (!parsed.has(StandardArg.qUseEmrun) && testName !== undefined);
    const useEmrun = parsed.has(StandardArg.qUseEmrun);
    const functions = [...parsed.keys()].filter(arg => !StandardArg.isKnown(arg));

    if (testName === undefined) {
        if (!isBatch)
            throw new Error('The bobuiestname parameter is required if not running a batch');
    } else if (testName === '') {
        throw new Error(`The bobuiestname=${testName} parameter is incorrect`);
    }

    const testOutputFormat = (() => {
        const format = parsed.get(StandardArg.qTestOutputFormat) ?? 'txt';
        if (-1 === ['txt', 'xml', 'lightxml', 'junitxml', 'tap'].indexOf(format))
            throw new Error(`Bad file format: ${format}`);
        return format;
    })();

    const resourceLocator = new ResourceLocator('');

    // Get or create container elements
    const testOutputContainer = document.querySelector('#test-output-container') || document.querySelector('body');
    const bobuiGuiContainer = document.querySelector('#bobui-gui-container');

    const testRunner = new BatchedTestRunner(
        new ModuleLoader(new ResourceFetcher(resourceLocator), resourceLocator),
        bobuiGuiContainer ? [bobuiGuiContainer] : undefined
    );
    window.bobuiTestRunner = testRunner;

    if (useEmrun) {
        const adapter = new EmrunAdapter(new EmrunCommunication(), testRunner, () => {
            if (!outputInPage)
                window.close();
        });
        adapter.run();
    }
    if (outputInPage) {
        const scanner = ScannerFactory.createScannerForFormat(testOutputFormat);
        const ui = new UI(testOutputContainer, !!scanner);
        const adapter =
            new VisualOutputProducer(ui.outputArea, ui.counters, scanner, testRunner);
        adapter.run();
    }
    setPageTitle(useEmrun, testName, isBatch);

    testRunner.run(isBatch, testName, functions, testOutputFormat);
})();
