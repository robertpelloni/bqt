// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef XMLWRITER_H
#define XMLWRITER_H

#include <QDomDocument>
#include <BOBUIextBlock>

BOBUI_BEGIN_NAMESPACE

class BOBUIextDocument;

class XmlWriter
{
public:
    XmlWriter(BOBUIextDocument *document) : textDocument(document) {}
    QDomDocument *toXml();

private:
    void readFragment(const BOBUIextBlock &currentBlock, QDomElement blockElement,
                      QDomDocument *document);
    void processBlock(const BOBUIextBlock &currentBlock);
    void processFragment(const BOBUIextFragment &currentFragment);

    QDomDocument *document;
    BOBUIextDocument *textDocument;
};

BOBUI_END_NAMESPACE

#endif
