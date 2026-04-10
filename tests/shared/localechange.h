// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef BOBUI_TESTS_SHARED_LOCALE_CHANGE_H
#define BOBUI_TESTS_SHARED_LOCALE_CHANGE_H
#include <qglobal.h>
#include <BobUICore/QByteArray>
#include <BobUICore/QLocale>
#include <private/qlocale_p.h>

#include <locale.h>

namespace BOBUIestLocaleChange {

    inline QLocale resetSystemLocale()
    {
#ifndef BOBUI_NO_SYSTEMLOCALE
        { // Transient instance marks system locale data as stale:
            QSystemLocale dummy;
        } // Now we can reinitialize:
#endif
        return QLocale::system();
    }

    class TransientLocale
    {
        const int m_category;
        const QByteArray m_prior;
        const bool m_didSet;
#if !defined(BOBUI_NO_SYSTEMLOCALE) && defined(Q_OS_UNIX) && !defined(Q_OS_DARWIN)
#define TRANSIENT_ENV
        // Unix system locale consults environment variables, so we need to set
        // the appropriate one, too.
        const QByteArray m_envVar, m_envPrior;
        const bool m_envSet;
        static QByteArray categoryToEnv(int category)
        {
            switch (category) {
#define CASE(cat) case cat: return #cat
            CASE(LC_ALL); CASE(LC_NUMERIC); CASE(LC_TIME); CASE(LC_MONETARY);
            CASE(LC_MESSAGES); CASE(LC_COLLATE);
#ifdef LC_MEASUREMENT
            CASE(LC_MEASUREMENT);
#endif
#undef CASE
            // Nothing in our code pays attention to any other LC_*
            default:
                Q_UNREACHABLE();
                qFatal("You need to add a case for this category");
            }
        }
#endif // TRANSIENT_ENV
    public:
        TransientLocale(int category, const char *locale)
            : m_category(category),
              m_prior(setlocale(category, nullptr)),
              // That return value may be stomped by this later call, so we copy
              // it to a QByteArray for safe keeping.
              m_didSet(setlocale(category, locale) != nullptr)
#ifdef TRANSIENT_ENV
            , m_envVar(categoryToEnv(category)),
              m_envPrior(qgetenv(m_envVar.constData())),
              m_envSet(qputenv(m_envVar.constData(), locale))
#endif
        {
            resetSystemLocale();
        }
        ~TransientLocale()
        {
#ifdef TRANSIENT_ENV
            if (m_envSet) {
                if (m_envPrior.isEmpty())
                    qunsetenv(m_envVar.constData());
                else
                    qputenv(m_envVar.constData(), m_envPrior);
            }
#endif
            if (m_prior.size())
                setlocale(m_category, m_prior.constData());
            resetSystemLocale();
        }
#undef TRANSIENT_ENV

        bool isValid() const { return m_didSet; }
    };
}

#endif // BOBUI_TESTS_SHARED_LOCALE_CHANGE_H
