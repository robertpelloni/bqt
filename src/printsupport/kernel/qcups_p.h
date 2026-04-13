// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QCUPS_P_H
#define QCUPS_P_H

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

#include <BobUIPrintSupport/private/bobuiprintsupportglobal_p.h>
#include <BobUIPrintSupport/private/qprint_p.h>
#include "BobUICore/qstring.h"
#include "BobUICore/qstringlist.h"
#include "BobUIPrintSupport/qprinter.h"
#include "BobUICore/qdatetime.h"

BOBUI_REQUIRE_CONFIG(cups);

BOBUI_BEGIN_NAMESPACE

class QPrintDevice;

// HACK! Define these here temporarily so they can be used in the dialogs
// without a circular reference to QCupsPrintEngine in the plugin.
// Move back to qcupsprintengine_p.h in the plugin once all usage
// removed from the dialogs.
#define PPK_CupsOptions QPrintEngine::PrintEnginePropertyKey(0xfe00)

#define PDPK_PpdFile          QPrintDevice::PrintDevicePropertyKey(QPrintDevice::PDPK_CustomBase)
#define PDPK_PpdOption        QPrintDevice::PrintDevicePropertyKey(QPrintDevice::PDPK_CustomBase + 1)
#define PDPK_CupsJobPriority  QPrintDevice::PrintDevicePropertyKey(QPrintDevice::PDPK_CustomBase + 2)
#define PDPK_CupsJobSheets    QPrintDevice::PrintDevicePropertyKey(QPrintDevice::PDPK_CustomBase + 3)
#define PDPK_CupsJobBilling   QPrintDevice::PrintDevicePropertyKey(QPrintDevice::PDPK_CustomBase + 4)
#define PDPK_CupsJobHoldUntil QPrintDevice::PrintDevicePropertyKey(QPrintDevice::PDPK_CustomBase + 5)
#define PDPK_PpdChoiceIsInstallableConflict QPrintDevice::PrintDevicePropertyKey(QPrintDevice::PDPK_CustomBase + 6)

class Q_PRINTSUPPORT_EXPORT QCUPSSupport
{
public:
    // Enum for values of job-hold-until option
    enum JobHoldUntil {
        NoHold = 0,  //CUPS Default
        Indefinite,
        DayTime,
        Night,
        SecondShift,
        ThirdShift,
        Weekend,
        SpecificTime
    };

    // Enum for valid banner pages
    enum BannerPage {
        NoBanner = 0,  //CUPS Default 'none'
        Standard,
        Unclassified,
        Confidential,
        Classified,
        Secret,
        TopSecret
    };

    // Enum for valid page set
    enum PageSet {
        AllPages = 0,  //CUPS Default
        OddPages,
        EvenPages
    };

    // Enum for valid number of pages per sheet
    enum PagesPerSheet {
        OnePagePerSheet = 0,
        TwoPagesPerSheet,
        FourPagesPerSheet,
        SixPagesPerSheet,
        NinePagesPerSheet,
        SixteenPagesPerSheet
    };

    // Enum for valid layouts of pages per sheet
    enum PagesPerSheetLayout {
        LeftToRightTopToBottom = 0,
        LeftToRightBottomToTop,
        RightToLeftTopToBottom,
        RightToLeftBottomToTop,
        BottomToTopLeftToRight,
        BottomToTopRightToLeft,
        TopToBottomLeftToRight,
        TopToBottomRightToLeft
    };

    static void setCupsOption(QPrinter *printer, const QString &option, const QString &value);
    static void clearCupsOption(QPrinter *printer, const QString &option);
    static void clearCupsOptions(QPrinter *printer);

    static void setJobHold(QPrinter *printer, const JobHoldUntil jobHold = NoHold, BOBUIime holdUntilTime = BOBUIime());
    static void setJobBilling(QPrinter *printer, const QString &jobBilling = QString());
    static void setJobPriority(QPrinter *printer, int priority = 50);
    static void setBannerPages(QPrinter *printer, const BannerPage startBannerPage, const BannerPage endBannerPage);
    static void setPageSet(QPrinter *printer, const PageSet pageSet);
    static void setPagesPerSheetLayout(QPrinter *printer, const PagesPerSheet pagesPerSheet,
                                       const PagesPerSheetLayout pagesPerSheetLayout);
    static void setPageRange(QPrinter *printer, int pageFrom, int pageTo);
    static void setPageRange(QPrinter *printer, const QString &pageRange);

    struct JobSheets
    {
        JobSheets(BannerPage s = NoBanner, BannerPage e = NoBanner)
         : startBannerPage(s), endBannerPage(e) {}

        BannerPage startBannerPage;
        BannerPage endBannerPage;
    };
    static JobSheets parseJobSheets(const QString &jobSheets);

    struct JobHoldUntilWithTime
    {
        JobHoldUntilWithTime(JobHoldUntil jh = NoHold, BOBUIime t = BOBUIime())
            : jobHold(jh), time(t) {}

        JobHoldUntil jobHold;
        BOBUIime time;
    };
    static JobHoldUntilWithTime parseJobHoldUntil(const QString &jobHoldUntil);

    static ppd_option_t *findPpdOption(const char *optionName, QPrintDevice *printDevice);
};
Q_DECLARE_TYPEINFO(QCUPSSupport::JobHoldUntil,        Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QCUPSSupport::BannerPage,          Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QCUPSSupport::PageSet,             Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QCUPSSupport::PagesPerSheetLayout, Q_PRIMITIVE_TYPE);
Q_DECLARE_TYPEINFO(QCUPSSupport::PagesPerSheet,       Q_PRIMITIVE_TYPE);

BOBUI_END_NAMESPACE

BOBUI_DECL_METATYPE_EXTERN_TAGGED(QCUPSSupport::JobHoldUntil,
                               QCUPSSupport__JobHoldUntil, Q_PRINTSUPPORT_EXPORT)
BOBUI_DECL_METATYPE_EXTERN_TAGGED(QCUPSSupport::BannerPage,
                               QCUPSSupport__BannerPage, Q_PRINTSUPPORT_EXPORT)
BOBUI_DECL_METATYPE_EXTERN_TAGGED(QCUPSSupport::PageSet, QCUPSSupport__PageSet, Q_PRINTSUPPORT_EXPORT)
BOBUI_DECL_METATYPE_EXTERN_TAGGED(QCUPSSupport::PagesPerSheetLayout,
                               QCUPSSupport__PagesPerSheetLayout, Q_PRINTSUPPORT_EXPORT)
BOBUI_DECL_METATYPE_EXTERN_TAGGED(QCUPSSupport::PagesPerSheet,
                               QCUPSSupport__PagesPerSheet, Q_PRINTSUPPORT_EXPORT)

#endif
