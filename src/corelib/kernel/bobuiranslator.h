// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:header-decls-only

#ifndef BOBUIRANSLATOR_H
#define BOBUIRANSLATOR_H

#include <BobUICore/qobject.h>
#include <BobUICore/qbytearray.h>

BOBUI_BEGIN_NAMESPACE


#ifndef BOBUI_NO_TRANSLATION

class QLocale;
class BOBUIranslatorPrivate;

class Q_CORE_EXPORT BOBUIranslator : public QObject
{
    Q_OBJECT
public:
    explicit BOBUIranslator(QObject *parent = nullptr);
    ~BOBUIranslator();

    virtual QString translate(const char *context, const char *sourceText,
                              const char *disambiguation = nullptr, int n = -1) const;

    virtual bool isEmpty() const;

    QString language() const;
    QString filePath() const;

    [[nodiscard]] bool load(const QString & filename,
                            const QString & directory = QString(),
                            const QString & search_delimiters = QString(),
                            const QString & suffix = QString());
    [[nodiscard]] bool load(const QLocale & locale,
                            const QString & filename,
                            const QString & prefix = QString(),
                            const QString & directory = QString(),
                            const QString & suffix = QString());
    [[nodiscard]] bool load(const uchar *data, int len,
                            const QString &directory = QString());

private:
    Q_DISABLE_COPY(BOBUIranslator)
    Q_DECLARE_PRIVATE(BOBUIranslator)
};

#endif // BOBUI_NO_TRANSLATION

BOBUI_END_NAMESPACE

#endif // BOBUIRANSLATOR_H
