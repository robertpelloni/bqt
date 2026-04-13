// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIESTELEMENT_P_H
#define BOBUIESTELEMENT_P_H

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

#include <BobUITest/bobuitestglobal.h>
#include <BobUITest/private/bobuiestcoreelement_p.h>

BOBUI_BEGIN_NAMESPACE


class BOBUIestElement : public BOBUIestCoreElement<BOBUIestElement>
{
    public:
        BOBUIestElement(BOBUIest::LogElementType type = BOBUIest::LET_Undefined);
        ~BOBUIestElement();

        bool addChild(BOBUIestElement *element);
        const std::vector<BOBUIestElement*> &childElements() const;

        const BOBUIestElement *parentElement() const;
        void setParent(const BOBUIestElement *p);

    private:
        std::vector<BOBUIestElement*> listOfChildren;
        const BOBUIestElement *parent = nullptr;

};

BOBUI_END_NAMESPACE

#endif
