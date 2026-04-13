// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIESTJUNITSTREAMER_P_H
#define BOBUIESTJUNITSTREAMER_P_H

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

#include <BobUICore/private/qglobal_p.h>
#include <vector>

BOBUI_BEGIN_NAMESPACE


class BOBUIestElement;
class BOBUIestElementAttribute;
class QJUnitTestLogger;
struct BOBUIestCharBuffer;

class BOBUIestJUnitStreamer
{
    public:
        BOBUIestJUnitStreamer(QJUnitTestLogger *logger);
        ~BOBUIestJUnitStreamer();

        void formatStart(const BOBUIestElement *element, BOBUIestCharBuffer *formatted) const;
        void formatEnd(const BOBUIestElement *element, BOBUIestCharBuffer *formatted) const;
        void formatAfterAttributes(const BOBUIestElement *element, BOBUIestCharBuffer *formatted) const;
        void output(BOBUIestElement *element) const;
        void outputElements(const std::vector<BOBUIestElement*> &) const;
        void outputElementAttributes(const BOBUIestElement *element, const std::vector<BOBUIestElementAttribute*> &attributes) const;

        void outputString(const char *msg) const;

    private:
        [[nodiscard]] bool formatAttributes(const BOBUIestElement *element,
                                            const BOBUIestElementAttribute *attribute,
                                            BOBUIestCharBuffer *formatted) const;
        static void indentForElement(const BOBUIestElement* element, char* buf, int size);

        QJUnitTestLogger *testLogger;
};

BOBUI_END_NAMESPACE

#endif
