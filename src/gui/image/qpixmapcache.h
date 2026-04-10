// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPIXMAPCACHE_H
#define QPIXMAPCACHE_H

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUIGui/qpixmap.h>

BOBUI_BEGIN_NAMESPACE


class Q_GUI_EXPORT QPixmapCache
{
public:
    class KeyData;
    class Q_GUI_EXPORT Key
    {
    public:
        Key();
        Key(const Key &other);
        Key(Key &&other) noexcept : d(other.d) { other.d = nullptr; }
        Key &operator =(Key &&other) noexcept { swap(other); return *this; }
        ~Key();
        bool operator ==(const Key &key) const;
        inline bool operator !=(const Key &key) const
        { return !operator==(key); }
        Key &operator =(const Key &other);

        void swap(Key &other) noexcept { bobui_ptr_swap(d, other.d); }
        bool isValid() const noexcept;

    private:
        friend size_t qHash(const QPixmapCache::Key &k, size_t seed = 0) noexcept
        { return k.hash(seed); }
        size_t hash(size_t seed) const noexcept;

        KeyData *d;
        friend class QPMCache;
        friend class QPixmapCache;
    };

    static int cacheLimit();
    static void setCacheLimit(int);
    static bool find(const QString &key, QPixmap *pixmap);
    static bool find(const Key &key, QPixmap *pixmap);
    static bool insert(const QString &key, const QPixmap &pixmap);
    static Key insert(const QPixmap &pixmap);
#if BOBUI_DEPRECATED_SINCE(6, 6)
    BOBUI_DEPRECATED_VERSION_X_6_6("Use remove(key), followed by key = insert(pixmap).")
    BOBUI_GUI_INLINE_SINCE(6, 6)
    static bool replace(const Key &key, const QPixmap &pixmap);
#endif
    static void remove(const QString &key);
    static void remove(const Key &key);
    static void clear();
};
Q_DECLARE_SHARED(QPixmapCache::Key)

#if BOBUI_DEPRECATED_SINCE(6, 6)
#if BOBUI_GUI_INLINE_IMPL_SINCE(6, 6)
bool QPixmapCache::replace(const Key &key, const QPixmap &pixmap)
{
    if (!key.isValid())
        return false;
    remove(key);
    const_cast<Key&>(key) = insert(pixmap);
    return key.isValid();
}
#endif // BOBUI_GUI_INLINE_IMPL_SINCE(6, 6)
#endif // BOBUI_DEPRECATED_SINCE(6, 6)

BOBUI_END_NAMESPACE

#endif // QPIXMAPCACHE_H
