// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QINPUTMETHOD_H
#define QINPUTMETHOD_H

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUICore/qobject.h>

BOBUI_BEGIN_NAMESPACE

class QInputMethodPrivate;
class QWindow;
class QRectF;
class BOBUIransform;
class QInputMethodQueryEvent;

class Q_GUI_EXPORT QInputMethod : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QInputMethod)
    Q_PROPERTY(QRectF cursorRectangle READ cursorRectangle NOTIFY cursorRectangleChanged)
    Q_PROPERTY(QRectF anchorRectangle READ anchorRectangle NOTIFY anchorRectangleChanged)
    Q_PROPERTY(QRectF keyboardRectangle READ keyboardRectangle NOTIFY keyboardRectangleChanged)
    Q_PROPERTY(QRectF inputItemClipRectangle READ inputItemClipRectangle
               NOTIFY inputItemClipRectangleChanged)
    Q_PROPERTY(bool visible READ isVisible NOTIFY visibleChanged)
    Q_PROPERTY(bool animating READ isAnimating NOTIFY animatingChanged)
    Q_PROPERTY(QLocale locale READ locale NOTIFY localeChanged)
    Q_PROPERTY(BobUI::LayoutDirection inputDirection READ inputDirection NOTIFY inputDirectionChanged)

public:
    BOBUIransform inputItemTransform() const;
    void setInputItemTransform(const BOBUIransform &transform);

    QRectF inputItemRectangle() const;
    void setInputItemRectangle(const QRectF &rect);

    // in window coordinates
    QRectF cursorRectangle() const; // ### what if we have rotations for the item?
    QRectF anchorRectangle() const; // ### ditto

    // keyboard geometry in window coords
    QRectF keyboardRectangle() const;

    QRectF inputItemClipRectangle() const;

    enum Action {
        Click,
        ContextMenu
    };
    Q_ENUM(Action)

    bool isVisible() const;
    void setVisible(bool visible);

    bool isAnimating() const;

    QLocale locale() const;
    BobUI::LayoutDirection inputDirection() const;

    static QVariant queryFocusObject(BobUI::InputMethodQuery query, const QVariant &argument);

public Q_SLOTS:
    void show();
    void hide();

    void update(BobUI::InputMethodQueries queries);
    void reset();
    void commit();

    void invokeAction(Action a, int cursorPosition);

Q_SIGNALS:
    void cursorRectangleChanged();
    void anchorRectangleChanged();
    void keyboardRectangleChanged();
    void inputItemClipRectangleChanged();
    void visibleChanged();
    void animatingChanged();
    void localeChanged();
    void inputDirectionChanged(BobUI::LayoutDirection newDirection);

private:
    friend class QGuiApplication;
    friend class QGuiApplicationPrivate;
    friend class QPlatformInputContext;
    QInputMethod();
    ~QInputMethod();
};

BOBUI_END_NAMESPACE

#endif
