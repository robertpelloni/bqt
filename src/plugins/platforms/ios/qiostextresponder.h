// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QIOSTEXTRESPONDER_H
#define QIOSTEXTRESPONDER_H

#import <UIKit/UIKit.h>

#include <BobUICore/qstring.h>
#include <BobUIGui/qevent.h>

BOBUI_BEGIN_NAMESPACE

class QIOSInputContext;

BOBUI_END_NAMESPACE

@interface QIOSTextResponder : UIResponder

- (instancetype)initWithInputContext:(BOBUI_PREPEND_NAMESPACE(QIOSInputContext) *)context;

- (void)notifyInputDelegate:(BobUI::InputMethodQueries)updatedProperties;
- (BOOL)needsKeyboardReconfigure:(BobUI::InputMethodQueries)updatedProperties;
- (void)reset;
- (void)commit;

@end

@interface QIOSTextInputResponder : QIOSTextResponder <UITextInputTraits, UIKeyInput, UITextInput>

- (instancetype)initWithInputContext:(BOBUI_PREPEND_NAMESPACE(QIOSInputContext) *)context;
- (BOOL)needsKeyboardReconfigure:(BobUI::InputMethodQueries)updatedProperties;
- (void)reset;
- (void)commit;

- (void)notifyInputDelegate:(BobUI::InputMethodQueries)updatedProperties;

@property(readwrite, retain) UIView *inputView;
@property(readwrite, retain) UIView *inputAccessoryView;

// UITextInputTraits
@property(nonatomic) UITextAutocapitalizationType autocapitalizationType;
@property(nonatomic) UITextAutocorrectionType autocorrectionType;
@property(nonatomic) UITextSpellCheckingType spellCheckingType;
@property(nonatomic) BOOL enablesReturnKeyAutomatically;
@property(nonatomic) UIKeyboardAppearance keyboardAppearance;
@property(nonatomic) UIKeyboardType keyboardType;
@property(nonatomic) UIReturnKeyType returnKeyType;
@property(nonatomic, getter=isSecureTextEntry) BOOL secureTextEntry;

// UITextInput
@property(nonatomic, assign) id<UITextInputDelegate> inputDelegate;

@end

#endif // QIOSTEXTRESPONDER_H
