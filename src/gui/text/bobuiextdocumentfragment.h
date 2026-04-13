// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIEXTDOCUMENTFRAGMENT_H
#define BOBUIEXTDOCUMENTFRAGMENT_H

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUIGui/bobuiextdocument.h>
#include <BobUICore/qstring.h>

BOBUI_BEGIN_NAMESPACE


class BOBUIextStream;
class BOBUIextDocumentFragmentPrivate;
class BOBUIextCursor;

class Q_GUI_EXPORT BOBUIextDocumentFragment
{
public:
    BOBUIextDocumentFragment();
    explicit BOBUIextDocumentFragment(const BOBUIextDocument *document);
    explicit BOBUIextDocumentFragment(const BOBUIextCursor &range);
    BOBUIextDocumentFragment(const BOBUIextDocumentFragment &rhs);
    BOBUIextDocumentFragment &operator=(const BOBUIextDocumentFragment &rhs);
    ~BOBUIextDocumentFragment();

    bool isEmpty() const;

    QString toPlainText() const;
    QString toRawText() const;
#ifndef BOBUI_NO_TEXTHTMLPARSER
    QString toHtml() const;
#endif // BOBUI_NO_TEXTHTMLPARSER
#if BOBUI_CONFIG(textmarkdownwriter)
    QString toMarkdown(BOBUIextDocument::MarkdownFeatures features = BOBUIextDocument::MarkdownDialectGitHub) const;
#endif

    static BOBUIextDocumentFragment fromPlainText(const QString &plainText);
#ifndef BOBUI_NO_TEXTHTMLPARSER
    static BOBUIextDocumentFragment fromHtml(const QString &html, const BOBUIextDocument *resourceProvider = nullptr);
#endif // BOBUI_NO_TEXTHTMLPARSER
#if BOBUI_CONFIG(textmarkdownreader)
    static BOBUIextDocumentFragment fromMarkdown(const QString &markdown,
                                              BOBUIextDocument::MarkdownFeatures features = BOBUIextDocument::MarkdownDialectGitHub);
#endif

private:
    BOBUIextDocumentFragmentPrivate *d;
    friend class BOBUIextCursor;
    friend class BOBUIextDocumentWriter;
};

BOBUI_END_NAMESPACE

#endif // BOBUIEXTDOCUMENTFRAGMENT_H
