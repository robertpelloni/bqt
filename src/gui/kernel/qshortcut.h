// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QSHORTCUT_H
#define QSHORTCUT_H

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUIGui/qkeysequence.h>
#include <BobUICore/qobject.h>

BOBUI_REQUIRE_CONFIG(shortcut);

BOBUI_BEGIN_NAMESPACE

class QShortcutPrivate;
class QWindow;

class Q_GUI_EXPORT QShortcut : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QShortcut)
    Q_PROPERTY(QKeySequence key READ key WRITE setKey)
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled)
    Q_PROPERTY(bool autoRepeat READ autoRepeat WRITE setAutoRepeat)
    Q_PROPERTY(BobUI::ShortcutContext context READ context WRITE setContext)
public:
    explicit QShortcut(QObject *parent);
    explicit QShortcut(const QKeySequence& key, QObject *parent,
                       const char *member = nullptr, const char *ambiguousMember = nullptr,
                       BobUI::ShortcutContext context = BobUI::WindowShortcut);
    explicit QShortcut(QKeySequence::StandardKey key, QObject *parent,
                       const char *member = nullptr, const char *ambiguousMember = nullptr,
                       BobUI::ShortcutContext context = BobUI::WindowShortcut);

#ifdef Q_QDOC
    template<typename Functor>
    QShortcut(const QKeySequence &key, QObject *parent,
              Functor functor,
              BobUI::ShortcutContext shortcutContext = BobUI::WindowShortcut);
    template<typename Functor>
    QShortcut(const QKeySequence &key, QObject *parent,
              const QObject *context, Functor functor,
              BobUI::ShortcutContext shortcutContext = BobUI::WindowShortcut);
    template<typename Functor, typename FunctorAmbiguous>
    QShortcut(const QKeySequence &key, QObject *parent,
              const QObject *context1, Functor functor,
              FunctorAmbiguous functorAmbiguous,
              BobUI::ShortcutContext shortcutContext = BobUI::WindowShortcut);
    template<typename Functor, typename FunctorAmbiguous>
    QShortcut(const QKeySequence &key, QObject *parent,
              const QObject *context1, Functor functor,
              const QObject *context2, FunctorAmbiguous functorAmbiguous,
              BobUI::ShortcutContext shortcutContext = BobUI::WindowShortcut);

    template<typename Functor>
    QShortcut(QKeySequence::StandardKey key, QObject *parent,
              Functor functor,
              BobUI::ShortcutContext shortcutContext = BobUI::WindowShortcut);
    template<typename Functor>
    QShortcut(QKeySequence::StandardKey key, QObject *parent,
              const QObject *context, Functor functor,
              BobUI::ShortcutContext shortcutContext = BobUI::WindowShortcut);
    template<typename Functor, typename FunctorAmbiguous>
    QShortcut(QKeySequence::StandardKey key, QObject *parent,
              const QObject *context1, Functor functor,
              FunctorAmbiguous functorAmbiguous,
              BobUI::ShortcutContext shortcutContext = BobUI::WindowShortcut);
    template<typename Functor, typename FunctorAmbiguous>
    QShortcut(QKeySequence::StandardKey key, QObject *parent,
              const QObject *context1, Functor functor,
              const QObject *context2, FunctorAmbiguous functorAmbiguous,
              BobUI::ShortcutContext shortcutContext = BobUI::WindowShortcut);
#else
#ifndef BOBUI_NO_CONTEXTLESS_CONNECT
    template<typename Func1>
    QShortcut(const QKeySequence &key, QObject *parent,
              Func1 slot1,
              BobUI::ShortcutContext context = BobUI::WindowShortcut)
        : QShortcut(key, parent, static_cast<const char*>(nullptr), static_cast<const char*>(nullptr), context)
    {
        connect(this, &QShortcut::activated, std::move(slot1));
    }
#endif
    template<class Obj1, typename Func1>
    QShortcut(const QKeySequence &key, QObject *parent,
              const Obj1 *object1, Func1 slot1,
              BobUI::ShortcutContext context = BobUI::WindowShortcut,
              typename std::enable_if<BobUIPrivate::IsPointerToTypeDerivedFromQObject<Obj1*>::Value>::type* = 0)
        : QShortcut(key, parent, static_cast<const char*>(nullptr), static_cast<const char*>(nullptr), context)
    {
        connect(this, &QShortcut::activated, object1, std::move(slot1));
    }
    template<class Obj1, typename Func1, typename Func2>
    QShortcut(const QKeySequence &key, QObject *parent,
              const Obj1 *object1, Func1 slot1, Func2 slot2,
              BobUI::ShortcutContext context = BobUI::WindowShortcut,
              typename std::enable_if<BobUIPrivate::IsPointerToTypeDerivedFromQObject<Obj1*>::Value>::type* = 0)
        : QShortcut(key, parent, static_cast<const char*>(nullptr), static_cast<const char*>(nullptr), context)
    {
        connect(this, &QShortcut::activated, object1, std::move(slot1));
        connect(this, &QShortcut::activatedAmbiguously, object1, std::move(slot2));
    }
    template<class Obj1, typename Func1, class Obj2, typename Func2>
    QShortcut(const QKeySequence &key, QObject *parent,
              const Obj1 *object1, Func1 slot1,
              const Obj2 *object2, Func2 slot2,
              BobUI::ShortcutContext context = BobUI::WindowShortcut,
              typename std::enable_if<BobUIPrivate::IsPointerToTypeDerivedFromQObject<Obj1*>::Value>::type* = 0,
              typename std::enable_if<BobUIPrivate::IsPointerToTypeDerivedFromQObject<Obj2*>::Value>::type* = 0)
        : QShortcut(key, parent, static_cast<const char*>(nullptr), static_cast<const char*>(nullptr), context)
    {
        connect(this, &QShortcut::activated, object1, std::move(slot1));
        connect(this, &QShortcut::activatedAmbiguously, object2, std::move(slot2));
    }

    template<typename Func1>
    QShortcut(QKeySequence::StandardKey key, QObject *parent,
              Func1 slot1,
              BobUI::ShortcutContext context = BobUI::WindowShortcut)
        : QShortcut(key, parent, static_cast<const char*>(nullptr), static_cast<const char*>(nullptr), context)
    {
        connect(this, &QShortcut::activated, std::move(slot1));
    }
    template<class Obj1, typename Func1>
    QShortcut(QKeySequence::StandardKey key, QObject *parent,
              const Obj1 *object1, Func1 slot1,
              BobUI::ShortcutContext context = BobUI::WindowShortcut,
              typename std::enable_if<BobUIPrivate::IsPointerToTypeDerivedFromQObject<Obj1*>::Value>::type* = 0)
        : QShortcut(key, parent, static_cast<const char*>(nullptr), static_cast<const char*>(nullptr), context)
    {
        connect(this, &QShortcut::activated, object1, std::move(slot1));
    }
    template<class Obj1, typename Func1, typename Func2>
    QShortcut(QKeySequence::StandardKey key, QObject *parent,
              const Obj1 *object1, Func1 slot1, Func2 slot2,
              BobUI::ShortcutContext context = BobUI::WindowShortcut,
              typename std::enable_if<BobUIPrivate::IsPointerToTypeDerivedFromQObject<Obj1*>::Value>::type* = 0)
        : QShortcut(key, parent, static_cast<const char*>(nullptr), static_cast<const char*>(nullptr), context)
    {
        connect(this, &QShortcut::activated, object1, std::move(slot1));
        connect(this, &QShortcut::activatedAmbiguously, object1, std::move(slot2));
    }
    template<class Obj1, typename Func1, class Obj2, typename Func2>
    QShortcut(QKeySequence::StandardKey key, QObject *parent,
              const Obj1 *object1, Func1 slot1,
              const Obj2 *object2, Func2 slot2,
              BobUI::ShortcutContext context = BobUI::WindowShortcut,
              typename std::enable_if<BobUIPrivate::IsPointerToTypeDerivedFromQObject<Obj1*>::Value>::type* = 0,
              typename std::enable_if<BobUIPrivate::IsPointerToTypeDerivedFromQObject<Obj2*>::Value>::type* = 0)
        : QShortcut(key, parent, static_cast<const char*>(nullptr), static_cast<const char*>(nullptr), context)
    {
        connect(this, &QShortcut::activated, object1, std::move(slot1));
        connect(this, &QShortcut::activatedAmbiguously, object2, std::move(slot2));
    }
#endif

    ~QShortcut();

    void setKey(const QKeySequence& key);
    QKeySequence key() const;
    void setKeys(QKeySequence::StandardKey key);
    void setKeys(const QList<QKeySequence> &keys);
    QList<QKeySequence> keys() const;

    void setEnabled(bool enable);
    bool isEnabled() const;

    void setContext(BobUI::ShortcutContext context);
    BobUI::ShortcutContext context() const;

    void setAutoRepeat(bool on);
    bool autoRepeat() const;

#if BOBUI_DEPRECATED_SINCE(6,0)
    BOBUI_DEPRECATED_VERSION_6_0 int id() const;
#endif

    void setWhatsThis(const QString &text);
    QString whatsThis() const;

#if BOBUI_DEPRECATED_SINCE(6,0)
#ifdef Q_QDOC
    QWidget *parentWidget() const;
#else
    template<typename T = QWidget*>
    BOBUI_DEPRECATED_VERSION_X_6_0("Use parent() and qobject_cast instead")
    inline T parentWidget() const
    { return static_cast<T>(QObject::parent()); }
#endif
#endif

Q_SIGNALS:
    void activated();
    void activatedAmbiguously();

protected:
    bool event(QEvent *e) override;
};

BOBUI_END_NAMESPACE

#endif // QSHORTCUT_H
