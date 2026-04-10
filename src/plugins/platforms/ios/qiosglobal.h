// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QIOSGLOBAL_H
#define QIOSGLOBAL_H

#import <UIKit/UIKit.h>
#include <BobUICore/BobUICore>

@class QIOSViewController;

BOBUI_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(lcQpaApplication);
Q_DECLARE_LOGGING_CATEGORY(lcQpaInputMethods);
Q_DECLARE_LOGGING_CATEGORY(lcQpaWindow);
Q_DECLARE_LOGGING_CATEGORY(lcQpaWindowScene);

#if !defined(BOBUI_NO_DEBUG)
#define qImDebug \
    for (bool bobui_category_enabled = lcQpaInputMethods().isDebugEnabled(); bobui_category_enabled; bobui_category_enabled = false) \
        QMessageLogger(BOBUI_MESSAGELOG_FILE, BOBUI_MESSAGELOG_LINE, BOBUI_MESSAGELOG_FUNC, lcQpaInputMethods().categoryName()).debug
#else
#define qImDebug BOBUI_NO_QDEBUG_MACRO
#endif

class QPlatformScreen;

bool isBobUIApplication();
bool isRunningOnVisionOS();

#ifndef Q_OS_TVOS
BobUI::ScreenOrientation toBobUIScreenOrientation(UIDeviceOrientation uiDeviceOrientation);
UIDeviceOrientation fromBobUIScreenOrientation(BobUI::ScreenOrientation bobuiOrientation);
#endif

int infoPlistValue(NSString* key, int defaultValue);

class QWindow;
class QScreen;
UIWindow *presentationWindow(QWindow *);
UIView *rootViewForScreen(const QPlatformScreen *);

BOBUI_END_NAMESPACE

@interface UIResponder (BobUIFirstResponder)
+ (id)bobui_currentFirstResponder;
@end

BOBUI_BEGIN_NAMESPACE

class FirstResponderCandidate : public QScopedValueRollback<UIResponder *>
{
public:
     FirstResponderCandidate(UIResponder *);
     static UIResponder *currentCandidate() { return s_firstResponderCandidate; }

private:
    static UIResponder *s_firstResponderCandidate;
};

BOBUI_END_NAMESPACE

#endif // QIOSGLOBAL_H
