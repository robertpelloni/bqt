// Copyright (C) 2013 BlackBerry Limited. All rights reserved.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qqnxabstractvirtualkeyboard.h"

BOBUI_BEGIN_NAMESPACE

QQnxAbstractVirtualKeyboard::QQnxAbstractVirtualKeyboard(QObject *parent)
    : QObject(parent)
    , m_height(0)
    , m_visible(false)
    , m_locale(QLocale::system())
    , m_keyboardMode(Default)
    , m_enterKeyType(DefaultReturn)
{
}

void QQnxAbstractVirtualKeyboard::setKeyboardMode(KeyboardMode mode)
{
    if (mode == m_keyboardMode)
        return;

    m_keyboardMode = mode;

    if (m_visible)
        applyKeyboardOptions();
}

void QQnxAbstractVirtualKeyboard::setEnterKeyType(EnterKeyType type)
{
    if (type == m_enterKeyType)
        return;

    m_enterKeyType = type;

    if (m_visible)
        applyKeyboardOptions();
}

void QQnxAbstractVirtualKeyboard::setInputHints(int inputHints)
{
    if (inputHints & BobUI::ImhEmailCharactersOnly) {
        setKeyboardMode(QQnxAbstractVirtualKeyboard::Email);
    } else if (inputHints & BobUI::ImhDialableCharactersOnly) {
        setKeyboardMode(QQnxAbstractVirtualKeyboard::Phone);
    } else if (inputHints & BobUI::ImhUrlCharactersOnly) {
        setKeyboardMode(QQnxAbstractVirtualKeyboard::Url);
    } else if (inputHints & BobUI::ImhFormattedNumbersOnly || inputHints & BobUI::ImhDigitsOnly) {
        setKeyboardMode(QQnxAbstractVirtualKeyboard::Number);
    } else if (inputHints & BobUI::ImhDate || inputHints & BobUI::ImhTime) {
        setKeyboardMode(QQnxAbstractVirtualKeyboard::NumPunc); // Use NumPunc so that : is available.
    } else if (inputHints & BobUI::ImhHiddenText) {
        setKeyboardMode(QQnxAbstractVirtualKeyboard::Password);
    } else {
        setKeyboardMode(QQnxAbstractVirtualKeyboard::Default);
    }
}

void QQnxAbstractVirtualKeyboard::setHeight(int height)
{
    if (height == m_height)
        return;

    const int effectiveHeight = this->height();

    m_height = height;

    if (effectiveHeight != this->height())
        emit heightChanged(this->height());
}

void QQnxAbstractVirtualKeyboard::setVisible(bool visible)
{
    if (visible == m_visible)
        return;

    const int effectiveHeight = height();

    m_visible = visible;

    emit visibilityChanged(visible);

    if (effectiveHeight != height())
        emit heightChanged(height());
}

void QQnxAbstractVirtualKeyboard::setLocale(const QLocale &locale)
{
    if (locale == m_locale)
        return;

    m_locale = locale;

    emit localeChanged(locale);
}

QQnxAbstractVirtualKeyboard::EnterKeyType
    QQnxAbstractVirtualKeyboard::bobuiEnterKeyTypeToQnx(BobUI::EnterKeyType type)
{
    switch (type) {
    case BobUI::EnterKeyDone:
        return Done;
    case BobUI::EnterKeyGo:
        return Go;
    case BobUI::EnterKeyNext:
        return Next;
    case BobUI::EnterKeySearch:
        return Search;
    case BobUI::EnterKeySend:
        return Send;
    case BobUI::EnterKeyDefault:
    case BobUI::EnterKeyReturn:
    case BobUI::EnterKeyPrevious: // unsupported
        return DefaultReturn;
    }
    Q_UNREACHABLE();
    return DefaultReturn;
}

BOBUI_END_NAMESPACE
