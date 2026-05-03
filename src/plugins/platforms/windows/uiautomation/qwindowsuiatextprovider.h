// Copyright (C) 2017 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWINDOWSUIATEXTPROVIDER_H
#define QWINDOWSUIATEXTPROVIDER_H

#include <BobUIGui/bobuiguiglobal.h>
#if BOBUI_CONFIG(accessibility)

#include "qwindowsuiabaseprovider.h"
#include "qwindowsuiatextrangeprovider.h"

BOBUI_BEGIN_NAMESPACE

namespace BobUIPrivate {

template <>
struct QComObjectTraits<ITextProvider2>
{
    static constexpr bool isGuidOf(REFIID riid) noexcept
    {
        return QComObjectTraits<ITextProvider2, ITextProvider>::isGuidOf(riid);
    }
};

} // namespace BobUIPrivate

// Implements the Text control pattern provider. Used for text controls.
class QWindowsUiaTextProvider : public QWindowsUiaBaseProvider, public QComObject<ITextProvider2>
{
    Q_DISABLE_COPY_MOVE(QWindowsUiaTextProvider)
public:
    explicit QWindowsUiaTextProvider(QAccessible::Id id);
    ~QWindowsUiaTextProvider();

    // ITextProvider
    HRESULT STDMETHODCALLTYPE GetSelection(SAFEARRAY **pRetVal) override;
    HRESULT STDMETHODCALLTYPE GetVisibleRanges(SAFEARRAY **pRetVal) override;
    HRESULT STDMETHODCALLTYPE RangeFromChild(IRawElementProviderSimple *childElement, ITextRangeProvider **pRetVal) override;
    HRESULT STDMETHODCALLTYPE RangeFromPoint(UiaPoint point, ITextRangeProvider **pRetVal) override;
    HRESULT STDMETHODCALLTYPE get_DocumentRange(ITextRangeProvider **pRetVal) override;
    HRESULT STDMETHODCALLTYPE get_SupportedTextSelection(SupportedTextSelection *pRetVal) override;

    // ITextProvider2
    HRESULT STDMETHODCALLTYPE RangeFromAnnotation(IRawElementProviderSimple *annotationElement, ITextRangeProvider **pRetVal) override;
    HRESULT STDMETHODCALLTYPE GetCaretRange(BOOL *isActive, ITextRangeProvider **pRetVal) override;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_CONFIG(accessibility)

#endif // QWINDOWSUIATEXTPROVIDER_H
