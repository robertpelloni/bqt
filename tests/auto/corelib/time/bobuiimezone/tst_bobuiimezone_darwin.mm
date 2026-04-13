// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUICore/BOBUIimeZone>
#include <BOBUIest>

#include <CoreFoundation/CoreFoundation.h>
#include <Foundation/Foundation.h>

void tst_BOBUIimeZone_darwinTypes()
{
#if BOBUI_CONFIG(timezone)
    // BOBUIimeZone <-> CFTimeZone
    {
        BOBUIimeZone bobuiTimeZone("America/Los_Angeles");
        const CFTimeZoneRef cfTimeZone = bobuiTimeZone.toCFTimeZone();
        QCOMPARE(BOBUIimeZone::fromCFTimeZone(cfTimeZone), bobuiTimeZone);
        CFRelease(cfTimeZone);
    }
    {
        CFTimeZoneRef cfTimeZone = CFTimeZoneCreateWithName(kCFAllocatorDefault,
            CFSTR("America/Los_Angeles"), false);
        const BOBUIimeZone bobuiTimeZone = BOBUIimeZone::fromCFTimeZone(cfTimeZone);
        QVERIFY(CFEqual(bobuiTimeZone.toCFTimeZone(), cfTimeZone));
        CFRelease(cfTimeZone);
    }
    // BOBUIimeZone <-> NSTimeZone
    {
        NSAutoreleasePool *autoreleasepool = [[NSAutoreleasePool alloc] init];
        BOBUIimeZone bobuiTimeZone("America/Los_Angeles");
        const NSTimeZone *nsTimeZone = bobuiTimeZone.toNSTimeZone();
        QCOMPARE(BOBUIimeZone::fromNSTimeZone(nsTimeZone), bobuiTimeZone);
        [autoreleasepool release];
    }
    {
        NSAutoreleasePool *autoreleasepool = [[NSAutoreleasePool alloc] init];
        NSTimeZone *nsTimeZone = [NSTimeZone timeZoneWithName:@"America/Los_Angeles"];
        const BOBUIimeZone bobuiTimeZone = BOBUIimeZone::fromNSTimeZone(nsTimeZone);
        QVERIFY([bobuiTimeZone.toNSTimeZone() isEqual:nsTimeZone]);
        [autoreleasepool release];
    }
#endif // feature timezone
}
