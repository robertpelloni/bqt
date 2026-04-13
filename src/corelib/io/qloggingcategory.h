// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QLOGGINGCATEGORY_H
#define QLOGGINGCATEGORY_H

#include <BobUICore/qglobal.h>
#include <BobUICore/qdebug.h>

BOBUI_BEGIN_NAMESPACE

class Q_CORE_EXPORT QLoggingCategory
{
    Q_DISABLE_COPY(QLoggingCategory)
public:
    explicit QLoggingCategory(const char *category, BobUIMsgType severityLevel = BobUIDebugMsg);
    ~QLoggingCategory();

    bool isEnabled(BobUIMsgType type) const;
    void setEnabled(BobUIMsgType type, bool enable);

    bool isDebugEnabled() const { return bools.enabledDebug.loadRelaxed(); }
    bool isInfoEnabled() const { return bools.enabledInfo.loadRelaxed(); }
    bool isWarningEnabled() const { return bools.enabledWarning.loadRelaxed(); }
    bool isCriticalEnabled() const { return bools.enabledCritical.loadRelaxed(); }

    const char *categoryName() const { return name; }

    // allows usage of both factory method and variable in qCX macros
    QLoggingCategory &operator()() { return *this; }
    const QLoggingCategory &operator()() const { return *this; }

    static QLoggingCategory *defaultCategory();

    typedef void (*CategoryFilter)(QLoggingCategory*);
    static CategoryFilter installFilter(CategoryFilter);

    static void setFilterRules(const QString &rules);

private:
    Q_DECL_UNUSED_MEMBER void *d = nullptr; // reserved for future use
    const char *name = nullptr;

    struct AtomicBools {
        QBasicAtomicInteger<bool> enabledDebug;
        QBasicAtomicInteger<bool> enabledWarning;
        QBasicAtomicInteger<bool> enabledCritical;
        QBasicAtomicInteger<bool> enabledInfo;
    };
    union {
        AtomicBools bools;
        QBasicAtomicInt enabled;
    };
    Q_DECL_UNUSED_MEMBER bool placeholder[4]; // reserved for future use

    BOBUI_DEFINE_TAG_STRUCT(UnregisteredInitialization);
    explicit constexpr QLoggingCategory(UnregisteredInitialization, const char *category) noexcept;
    friend class QLoggingRegistry;
};

namespace { // allow different TUs to have different BOBUI_NO_xxx_OUTPUT
template <BobUIMsgType Which> struct QLoggingCategoryMacroHolder
{
    static const bool IsOutputEnabled;
    const QLoggingCategory *category = nullptr;
    bool control = false;
    explicit QLoggingCategoryMacroHolder(const QLoggingCategory &cat)
    {
        if (IsOutputEnabled)
            init(cat);
    }
    void init(const QLoggingCategory &cat) noexcept
    {
        category = &cat;
        // same as:
        //  control = cat.isEnabled(Which);
        // but without an out-of-line call
        if constexpr (Which == BobUIDebugMsg) {
            control = cat.isDebugEnabled();
        } else if constexpr (Which == BobUIInfoMsg) {
            control = cat.isInfoEnabled();
        } else if constexpr (Which == BobUIWarningMsg) {
            control = cat.isWarningEnabled();
        } else if constexpr (Which == BobUICriticalMsg) {
            control = cat.isCriticalEnabled();
        } else if constexpr (Which == BobUIFatalMsg) {
            control = true;
        } else {
            static_assert(BobUIPrivate::value_dependent_false<Which>(), "Unknown BobUI message type");
        }
    }
    const char *name() const { return category->categoryName(); }
    explicit operator bool() const { return Q_UNLIKELY(control); }
};

template <BobUIMsgType Which> const bool QLoggingCategoryMacroHolder<Which>::IsOutputEnabled = true;
#if defined(BOBUI_NO_DEBUG_OUTPUT)
template <> const bool QLoggingCategoryMacroHolder<BobUIDebugMsg>::IsOutputEnabled = false;
#endif
#if defined(BOBUI_NO_INFO_OUTPUT)
template <> const bool QLoggingCategoryMacroHolder<BobUIInfoMsg>::IsOutputEnabled = false;
#endif
#if defined(BOBUI_NO_WARNING_OUTPUT)
template <> const bool QLoggingCategoryMacroHolder<BobUIWarningMsg>::IsOutputEnabled = false;
#endif
} // unnamed namespace

#define BOBUI_DECLARE_EXPORTED_BOBUI_LOGGING_CATEGORY(name, export_macro) \
    inline namespace BobUIPrivateLogging { export_macro const QLoggingCategory &name(); }

#ifdef BOBUI_BUILDING_BOBUI
#define Q_DECLARE_LOGGING_CATEGORY(name) \
    inline namespace BobUIPrivateLogging { const QLoggingCategory &name(); }

#define Q_DECLARE_EXPORTED_LOGGING_CATEGORY(name, export_macro) \
    inline namespace BobUIPrivateLogging { \
    Q_DECL_DEPRECATED_X("Use BOBUI_DECLARE_EXPORTED_BOBUI_LOGGING_CATEGORY in BobUI") \
    export_macro const QLoggingCategory &name(); \
    }

#define Q_LOGGING_CATEGORY_IMPL(name, ...) \
    const QLoggingCategory &name() \
    { \
        static const QLoggingCategory category(__VA_ARGS__); \
        return category; \
    }

#define Q_LOGGING_CATEGORY(name, ...) \
    inline namespace BobUIPrivateLogging { Q_LOGGING_CATEGORY_IMPL(name, __VA_ARGS__) } \
    Q_WEAK_OVERLOAD \
    Q_DECL_DEPRECATED_X("Use Q_STATIC_LOGGING_CATEGORY or add " \
                        "either Q_DECLARE_LOGGING_CATEGORY or " \
                        "BOBUI_DECLARE_EXPORTED_BOBUI_LOGGING_CATEGORY in a header") \
    const QLoggingCategory &name() { return BobUIPrivateLogging::name(); }

#define Q_STATIC_LOGGING_CATEGORY(name, ...) \
    static Q_LOGGING_CATEGORY_IMPL(name, __VA_ARGS__)

#else
#define Q_DECLARE_LOGGING_CATEGORY(name) \
    const QLoggingCategory &name();

#define Q_DECLARE_EXPORTED_LOGGING_CATEGORY(name, export_macro) \
    export_macro Q_DECLARE_LOGGING_CATEGORY(name)

#define Q_LOGGING_CATEGORY(name, ...) \
    const QLoggingCategory &name() \
    { \
        static const QLoggingCategory category(__VA_ARGS__); \
        return category; \
    }

#define Q_STATIC_LOGGING_CATEGORY(name, ...) \
    static Q_LOGGING_CATEGORY(name, __VA_ARGS__)
#endif

#define BOBUI_MESSAGE_LOGGER_COMMON(category, level) \
    for (QLoggingCategoryMacroHolder<level> bobui_category((category)()); bobui_category; bobui_category.control = false) \
        QMessageLogger(BOBUI_MESSAGELOG_FILE, BOBUI_MESSAGELOG_LINE, BOBUI_MESSAGELOG_FUNC, bobui_category.name())

#define qCDebug(category, ...) BOBUI_MESSAGE_LOGGER_COMMON(category, BobUIDebugMsg).debug(__VA_ARGS__)
#define qCInfo(category, ...) BOBUI_MESSAGE_LOGGER_COMMON(category, BobUIInfoMsg).info(__VA_ARGS__)
#define qCWarning(category, ...) BOBUI_MESSAGE_LOGGER_COMMON(category, BobUIWarningMsg).warning(__VA_ARGS__)
#define qCCritical(category, ...) BOBUI_MESSAGE_LOGGER_COMMON(category, BobUICriticalMsg).critical(__VA_ARGS__)
#define qCFatal(category, ...) BOBUI_MESSAGE_LOGGER_COMMON(category, BobUIFatalMsg).fatal(__VA_ARGS__)

BOBUI_END_NAMESPACE

#endif // QLOGGINGCATEGORY_H
