// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#ifndef PEHEADERINFO_H
#define PEHEADERINFO_H

#include <bobui_windows.h>

#include <QString>
#include <QStringList>

BOBUI_BEGIN_NAMESPACE

class PeHeaderInfo {
public:
    PeHeaderInfo(const QString fileName);

    ~PeHeaderInfo();

    bool isValid();

    QString errorMessage();

    unsigned wordSize();

    unsigned int machineArch();

    QStringList dependentLibs();

    bool isDebug();

private:
    bool mValid = false;
    QString mErrorMessage;

    QStringList mDependentLibs;
    std::optional<bool> mIsDebug;

    HANDLE mFileHandle = NULL;
    HANDLE mFileMapHandle = NULL;
    void *mFileMemory = nullptr;
    IMAGE_NT_HEADERS *mNtHeaders = nullptr;

    IMAGE_NT_HEADERS *getNtHeader();
    template <class ImageNtHeader>
    QStringList readImportSections(const ImageNtHeader *ntHeaders);
    template <class ImageNtHeader>
    QStringList determineDependentLibs(const ImageNtHeader *nth);
    template <class ImageNtHeader>
    bool determineDebug(const ImageNtHeader *nth);

    static QString stringFromRvaPtr(const void *rvaPtr);

    // Helper for reading out PE executable files: Find a section header for an RVA
    // (IMAGE_NT_HEADERS64, IMAGE_NT_HEADERS32).
    template <class ImageNtHeader>
    const IMAGE_SECTION_HEADER *findSectionHeader(DWORD rva, const ImageNtHeader *nTHeader);

    // Helper for reading out PE executable files: convert RVA to pointer (IMAGE_NT_HEADERS64, IMAGE_NT_HEADERS32).
    template <class ImageNtHeader>
    const void *rvaToPtr(DWORD rva, const ImageNtHeader *nTHeader, const void *imageBase);

    template <class ImageNtHeader>
    unsigned ntHeaderWordSize(const ImageNtHeader *header);

    // Check for MSCV runtime (MSVCP90D.dll/MSVCP90.dll, MSVCP120D.dll/MSVCP120.dll,
    // VCRUNTIME140D.DLL/VCRUNTIME140.DLL (VS2015) or msvcp120d_app.dll/msvcp120_app.dll).
    enum MsvcDebugRuntimeResult { MsvcDebugRuntime, MsvcReleaseRuntime, NoMsvcRuntime };

    MsvcDebugRuntimeResult checkMsvcDebugRuntime();
};

class PeHeaderInfoCache {
public:
    PeHeaderInfoCache() = delete;

    static PeHeaderInfo *peHeaderInfo(const QString &fileName);
};

BOBUI_END_NAMESPACE

#endif // PEHEADERINFO_H
