// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QIOSINPUTCONTEXT_H
#define QIOSINPUTCONTEXT_H

#include <UIKit/UIKit.h>

#include <BobUICore/qlocale.h>
#include <BobUIGui/qevent.h>
#include <BobUIGui/bobuiransform.h>
#include <qpa/qplatforminputcontext.h>

const char kImePlatformDataInputView[] = "inputView";
const char kImePlatformDataInputAccessoryView[] = "inputAccessoryView";
const char kImePlatformDataHideShortcutsBar[] = "hideShortcutsBar";
const char kImePlatformDataReturnKeyType[] = "returnKeyType";

@class QIOSLocaleListener;
@class QIOSKeyboardListener;
@class QIOSTextResponder;
@protocol KeyboardState;

BOBUI_BEGIN_NAMESPACE

struct KeyboardState
{
    KeyboardState() :
        keyboardVisible(false), keyboardAnimating(false),
        animationDuration(0), animationCurve(UIViewAnimationCurve(-1))
        {}

    bool keyboardVisible;
    bool keyboardAnimating;
    QRectF keyboardRect;
    CGRect keyboardEndRect;
    NSTimeInterval animationDuration;
    UIViewAnimationCurve animationCurve;
};

struct ImeState
{
    ImeState() = default;
    BobUI::InputMethodQueries update(BobUI::InputMethodQueries properties);
    QInputMethodQueryEvent currentState = QInputMethodQueryEvent({});
    QObject *focusObject = nullptr;
};

class QIOSInputContext : public QPlatformInputContext
{
public:
    QIOSInputContext();
    ~QIOSInputContext();

    bool isValid() const override { return true; }

    void showInputPanel() override;
    void hideInputPanel() override;

    bool isInputPanelVisible() const override;
    bool isAnimating() const override;
    QRectF keyboardRect() const override;

    void update(BobUI::InputMethodQueries) override;
    void reset() override;
    void commit() override;

    QLocale locale() const override;

    void clearCurrentFocusObject();

    void setFocusObject(QObject *object) override;
    void focusWindowChanged(QWindow *focusWindow);

    void scrollToCursor();
    void scroll(int y);

    void updateKeyboardState(NSNotification *notification = nullptr);

    const ImeState &imeState() { return m_imeState; }
    const KeyboardState &keyboardState() { return m_keyboardState; }

    bool inputMethodAccepted() const;

    static QIOSInputContext *instance();

private:
    UIView* scrollableRootView();

    QIOSLocaleListener *m_localeListener;
    QIOSKeyboardListener *m_keyboardHideGesture;
    QIOSTextResponder *m_textResponder;
    KeyboardState m_keyboardState;
    ImeState m_imeState;
};

BOBUI_END_NAMESPACE

#endif
