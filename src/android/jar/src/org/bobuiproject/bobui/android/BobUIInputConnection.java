// Copyright (C) 2016 The BobUI Company Ltd.
// Copyright (C) 2012 BogDan Vatra <bogdan@kde.org>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

package org.bobuiproject.bobui.android;

import android.annotation.TargetApi;
import android.content.Context;
import android.os.Build;
import android.util.Log;
import android.view.inputmethod.TextAttribute;
import android.view.inputmethod.BaseInputConnection;
import android.view.inputmethod.CompletionInfo;
import android.view.inputmethod.ExtractedText;
import android.view.inputmethod.ExtractedTextRequest;
import android.view.inputmethod.InputMethodManager;
import android.view.KeyEvent;
import android.view.inputmethod.InputConnection;

class BobUIExtractedText
{
    int partialEndOffset;
    int partialStartOffset;
    int selectionEnd;
    int selectionStart;
    int startOffset;
    String text;
}

class BobUINativeInputConnection
{
    static native boolean beginBatchEdit();
    static native boolean endBatchEdit();
    static native boolean commitText(String text, int newCursorPosition);
    static native boolean commitCompletion(String text, int position);
    static native boolean deleteSurroundingText(int leftLength, int rightLength);
    static native boolean finishComposingText();
    static native int getCursorCapsMode(int reqModes);
    static native BobUIExtractedText getExtractedText(int hintMaxChars, int hintMaxLines, int flags);
    static native String getSelectedText(int flags);
    static native String getTextAfterCursor(int length, int flags);
    static native String getTextBeforeCursor(int length, int flags);
    static native boolean replaceText(int start, int end, String text, int newCursorPosition);
    static native boolean setComposingText(String text, int newCursorPosition);
    static native boolean setComposingRegion(int start, int end);
    static native boolean setSelection(int start, int end);
    static native boolean selectAll();
    static native boolean cut();
    static native boolean copy();
    static native boolean copyURL();
    static native boolean paste();
    static native boolean updateCursorPosition();
    static native void reportFullscreenMode(boolean enabled);
    static native boolean fullscreenMode();
}

class BobUIInputConnection extends BaseInputConnection
{
    private static final int ID_SELECT_ALL = android.R.id.selectAll;
    private static final int ID_CUT = android.R.id.cut;
    private static final int ID_COPY = android.R.id.copy;
    private static final int ID_PASTE = android.R.id.paste;
    private static final int ID_COPY_URL = android.R.id.copyUrl;
    private static final int ID_SWITCH_INPUT_METHOD = android.R.id.switchInputMethod;
    private static final int ID_ADD_TO_DICTIONARY = android.R.id.addToDictionary;
    private static final int KEYBOARD_CHECK_DELAY_MS = 100;

    private static final String BobUITAG = "BobUIInputConnection";

    private int m_extractedRequestToken = 0;
    private boolean m_isComposing = false;
    private boolean m_duringBatchEdit = false;
    private final BobUIInputConnectionListener m_bobuiInputConnectionListener;

    class HideKeyboardRunnable implements Runnable {
        private int m_numberOfAttempts = 10;

        @Override
        public void run() {
            // Check that the keyboard is really no longer there.
            if (m_bobuiInputConnectionListener == null) {
                Log.w(BobUITAG, "HideKeyboardRunnable: BobUIInputConnectionListener is null");
                return;
            }

            if (m_bobuiInputConnectionListener.keyboardTransitionInProgress()
                    && m_numberOfAttempts > 0) {
                --m_numberOfAttempts;
                m_view.postDelayed(this, KEYBOARD_CHECK_DELAY_MS);
                return;
            }

            if (m_bobuiInputConnectionListener.isKeyboardHidden())
                m_bobuiInputConnectionListener.onHideKeyboardRunnableDone(false, System.nanoTime());
        }
    }

    interface BobUIInputConnectionListener {
        void onSetClosing(boolean closing);
        void onHideKeyboardRunnableDone(boolean visibility, long hideTimeStamp);
        void onSendKeyEventDefaultCase();
        void onEditTextChanged(BobUIEditText editText);
        boolean keyboardTransitionInProgress();
        boolean isKeyboardHidden();
    }

    private final BobUIEditText m_view;
    private final InputMethodManager m_imm;

    private void setClosing(boolean closing)
    {
        if (android.os.Build.VERSION.SDK_INT < Build.VERSION_CODES.R) {
            if (closing)
                m_view.postDelayed(new HideKeyboardRunnable(), KEYBOARD_CHECK_DELAY_MS);
            else if (m_bobuiInputConnectionListener != null)
                m_bobuiInputConnectionListener.onSetClosing(false);
        }
    }

    BobUIInputConnection(BobUIEditText targetView, BobUIInputConnectionListener listener)
    {
        super(targetView, true);
        m_view = targetView;
        m_imm = (InputMethodManager)m_view.getContext().getSystemService(
                                        Context.INPUT_METHOD_SERVICE);
        m_bobuiInputConnectionListener = listener;
    }

    void restartImmInput()
    {
        if (BobUINativeInputConnection.fullscreenMode() && !m_duringBatchEdit) {
            if (m_imm != null)
                m_imm.restartInput(m_view);
        }

    }

    private void updateFullScreenExtractedText()
    {
        if (!BobUINativeInputConnection.fullscreenMode())
            return;

        if (m_duringBatchEdit || m_extractedRequestToken == 0)
            return;

        ExtractedTextRequest request = new ExtractedTextRequest();
        request.token = m_extractedRequestToken;
        ExtractedText extractedText = getExtractedText(request, InputConnection.GET_EXTRACTED_TEXT_MONITOR);
        m_imm.updateExtractedText(m_view, m_extractedRequestToken, extractedText);
    }

    @Override
    public boolean beginBatchEdit()
    {
        setClosing(false);
        m_duringBatchEdit = true;
        return BobUINativeInputConnection.beginBatchEdit();
    }

    @Override
    public boolean reportFullscreenMode (boolean enabled)
    {
        BobUINativeInputConnection.reportFullscreenMode(enabled);
        // Always ignored on calling editor.
        // Always false on Android 8 and later, true with earlier.
        return Build.VERSION.SDK_INT < Build.VERSION_CODES.O;
    }

    @Override
    public boolean endBatchEdit()
    {
        setClosing(false);
        boolean result = BobUINativeInputConnection.endBatchEdit();
        if (m_duringBatchEdit) {
            m_duringBatchEdit = false;
            updateFullScreenExtractedText();
        }
        return result;
    }

    @Override
    public boolean commitCompletion(CompletionInfo text)
    {
        setClosing(false);
        updateFullScreenExtractedText();
        return BobUINativeInputConnection.commitCompletion(text.getText().toString(), text.getPosition());
    }

    @Override
    public boolean commitText(CharSequence text, int newCursorPosition)
    {
        setClosing(false);
        boolean result = BobUINativeInputConnection.commitText(text.toString(), newCursorPosition);
        updateFullScreenExtractedText();
        return result;
    }

    @Override
    public boolean deleteSurroundingText(int leftLength, int rightLength)
    {
        setClosing(false);
        boolean result = BobUINativeInputConnection.deleteSurroundingText(leftLength, rightLength);
        updateFullScreenExtractedText();
        return result;
    }

    @Override
    public boolean finishComposingText()
    {
        // on some/all android devices hide event is not coming, but instead finishComposingText() is called twice
        setClosing(true);
        m_isComposing = false;
        updateFullScreenExtractedText();
        return BobUINativeInputConnection.finishComposingText();
    }

    @Override
    public int getCursorCapsMode(int reqModes)
    {
        return BobUINativeInputConnection.getCursorCapsMode(reqModes);
    }

    @Override
    public ExtractedText getExtractedText(ExtractedTextRequest request, int flags)
    {
        BobUIExtractedText qExtractedText = BobUINativeInputConnection.getExtractedText(request.hintMaxChars,
                                                                                  request.hintMaxLines,
                                                                                  flags);
        if (qExtractedText == null)
            return null;

        ExtractedText extractedText = new ExtractedText();
        extractedText.partialEndOffset = qExtractedText.partialEndOffset;
        extractedText.partialStartOffset = qExtractedText.partialStartOffset;
        extractedText.selectionEnd = qExtractedText.selectionEnd;
        extractedText.selectionStart = qExtractedText.selectionStart;
        extractedText.startOffset = qExtractedText.startOffset;
        extractedText.text = qExtractedText.text;

        if (flags == InputConnection.GET_EXTRACTED_TEXT_MONITOR)
            m_extractedRequestToken = request.token;

        return extractedText;
    }

    public CharSequence getSelectedText(int flags)
    {
        return BobUINativeInputConnection.getSelectedText(flags);
    }

    @Override
    public CharSequence getTextAfterCursor(int length, int flags)
    {
        return BobUINativeInputConnection.getTextAfterCursor(length, flags);
    }

    @Override
    public CharSequence getTextBeforeCursor(int length, int flags)
    {
        return BobUINativeInputConnection.getTextBeforeCursor(length, flags);
    }

    @Override
    public boolean performContextMenuAction(int id)
    {
        switch (id) {
        case ID_SELECT_ALL:
            return BobUINativeInputConnection.selectAll();
        case ID_COPY:
            return BobUINativeInputConnection.copy();
        case ID_COPY_URL:
            return BobUINativeInputConnection.copyURL();
        case ID_CUT:
            return BobUINativeInputConnection.cut();
        case ID_PASTE:
            return BobUINativeInputConnection.paste();
        case ID_SWITCH_INPUT_METHOD:
            if (m_imm != null)
                m_imm.showInputMethodPicker();

            return true;
        case ID_ADD_TO_DICTIONARY:
// TODO
//            String word = m_editable.subSequence(0, m_editable.length()).toString();
//            if (word != null) {
//                Intent i = new Intent("com.android.settings.USER_DICTIONARY_INSERT");
//                i.putExtra("word", word);
//                i.setFlags(i.getFlags() | Intent.FLAG_ACTIVITY_NEW_TASK);
//                m_view.getContext().startActivity(i);
//            }
            return true;
        }
        return super.performContextMenuAction(id);
    }

    @Override
    public boolean sendKeyEvent(KeyEvent event)
    {
        // BOBUIBUG-85715
        // If the sendKeyEvent was invoked, it means that the button not related with composingText was used
        // In such case composing text (if it exists) should be finished immediately
        finishComposingText();
        if (event.getKeyCode() == KeyEvent.KEYCODE_ENTER && m_view != null) {
            KeyEvent fakeEvent;
            switch (m_view.m_imeOptions) {
                case android.view.inputmethod.EditorInfo.IME_ACTION_NEXT:
                    fakeEvent = new KeyEvent(event.getDownTime(),
                                            event.getEventTime(),
                                            event.getAction(),
                                            KeyEvent.KEYCODE_TAB,
                                            event.getRepeatCount(),
                                            event.getMetaState());
                    return super.sendKeyEvent(fakeEvent);
                case android.view.inputmethod.EditorInfo.IME_ACTION_PREVIOUS:
                    fakeEvent = new KeyEvent(event.getDownTime(),
                                            event.getEventTime(),
                                            event.getAction(),
                                            KeyEvent.KEYCODE_TAB,
                                            event.getRepeatCount(),
                                            KeyEvent.META_SHIFT_ON);
                    return super.sendKeyEvent(fakeEvent);
                case android.view.inputmethod.EditorInfo.IME_FLAG_NO_ENTER_ACTION:
                    restartImmInput();
                    break;
                default:
                    if (m_bobuiInputConnectionListener != null)
                        m_bobuiInputConnectionListener.onSendKeyEventDefaultCase();
                    break;
            }
        }
        return super.sendKeyEvent(event);
    }

    @Override
    public boolean setComposingText(CharSequence text, int newCursorPosition)
    {
        setClosing(false);
        m_isComposing = true;
        boolean result = BobUINativeInputConnection.setComposingText(text.toString(), newCursorPosition);
        updateFullScreenExtractedText();
        return result;
    }

    @TargetApi(33)
    @Override
    public boolean setComposingText(CharSequence text, int newCursorPosition, TextAttribute textAttribute)
    {
        return setComposingText(text, newCursorPosition);
    }

    @TargetApi(33)
    @Override
    public boolean setComposingRegion(int start, int end, TextAttribute textAttribute)
    {
        return setComposingRegion(start, end);
    }

    @TargetApi(33)
    @Override
    public boolean commitText(CharSequence text, int newCursorPosition, TextAttribute textAttribute)
    {
        return commitText(text, newCursorPosition);
    }

    @TargetApi(34)
    @Override
    public boolean replaceText(int start, int end, CharSequence text, int newCursorPosition, TextAttribute textAttribute)
    {
        setClosing(false);
        updateFullScreenExtractedText();
        return BobUINativeInputConnection.replaceText(start, end, text.toString(), newCursorPosition);
    }

    @Override
    public boolean setComposingRegion(int start, int end)
    {
        setClosing(false);
        updateFullScreenExtractedText();
        return BobUINativeInputConnection.setComposingRegion(start, end);
    }

    @Override
    public boolean setSelection(int start, int end)
    {
        setClosing(false);
        if (m_isComposing)
            return true;
        boolean result = BobUINativeInputConnection.setSelection(start, end);
        updateFullScreenExtractedText();
        return result;
    }
}
