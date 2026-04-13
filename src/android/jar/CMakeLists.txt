# Copyright (C) 2023 The BobUI Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

set(java_sources
    src/org/bobuiproject/bobui/android/BobUIAccessibilityDelegate.java
    src/org/bobuiproject/bobui/android/BobUINativeAccessibility.java
    src/org/bobuiproject/bobui/android/CursorHandle.java
    src/org/bobuiproject/bobui/android/EditContextView.java
    src/org/bobuiproject/bobui/android/EditPopupMenu.java
    src/org/bobuiproject/bobui/android/ExtractStyle.java
    src/org/bobuiproject/bobui/android/BobUIApplicationBase.java
    src/org/bobuiproject/bobui/android/BobUIActivityBase.java
    src/org/bobuiproject/bobui/android/BobUIServiceBase.java
    src/org/bobuiproject/bobui/android/BobUIActivityDelegate.java
    src/org/bobuiproject/bobui/android/BobUIInputDelegate.java
    src/org/bobuiproject/bobui/android/BobUILoader.java
    src/org/bobuiproject/bobui/android/BobUIActivityLoader.java
    src/org/bobuiproject/bobui/android/BobUIServiceLoader.java
    src/org/bobuiproject/bobui/android/BobUIEditText.java
    src/org/bobuiproject/bobui/android/BobUIInputConnection.java
    src/org/bobuiproject/bobui/android/BobUILayout.java
    src/org/bobuiproject/bobui/android/BobUIMessageDialogHelper.java
    src/org/bobuiproject/bobui/android/BobUINative.java
    src/org/bobuiproject/bobui/android/BobUISurfaceInterface.java
    src/org/bobuiproject/bobui/android/BobUISurface.java
    src/org/bobuiproject/bobui/android/BobUITextureView.java
    src/org/bobuiproject/bobui/android/BobUIThread.java
    src/org/bobuiproject/bobui/android/extras/BobUIAndroidBinder.java
    src/org/bobuiproject/bobui/android/extras/BobUIAndroidServiceConnection.java
    src/org/bobuiproject/bobui/android/extras/BobUINative.java
    src/org/bobuiproject/bobui/android/BobUIClipboardManager.java
    src/org/bobuiproject/bobui/android/BobUIDisplayManager.java
    src/org/bobuiproject/bobui/android/UsedFromNativeCode.java
    src/org/bobuiproject/bobui/android/BobUIRootLayout.java
    src/org/bobuiproject/bobui/android/BobUIWindow.java
    src/org/bobuiproject/bobui/android/BobUIActivityDelegateBase.java
    src/org/bobuiproject/bobui/android/BobUIEmbeddedDelegate.java
    src/org/bobuiproject/bobui/android/BobUIEmbeddedViewInterfaceFactory.java
    src/org/bobuiproject/bobui/android/BobUIEmbeddedLoader.java
    src/org/bobuiproject/bobui/android/BobUIView.java
    src/org/bobuiproject/bobui/android/BobUIEmbeddedViewInterface.java
    src/org/bobuiproject/bobui/android/BobUIServiceEmbeddedDelegate.java
    src/org/bobuiproject/bobui/android/BackendRegister.java
    src/org/bobuiproject/bobui/android/BobUIWindowInterface.java
    src/org/bobuiproject/bobui/android/BobUIAccessibilityInterface.java
    src/org/bobuiproject/bobui/android/BobUIMenuInterface.java
    src/org/bobuiproject/bobui/android/BobUIInputInterface.java
    src/org/bobuiproject/bobui/android/BobUIAbstractItemModel.java
    src/org/bobuiproject/bobui/android/BobUIAbstractItemModelProxy.java
    src/org/bobuiproject/bobui/android/BobUIModelIndex.java
    src/org/bobuiproject/bobui/android/BobUIAbstractListModel.java
    src/org/bobuiproject/bobui/android/BobUISignalListener.java
    src/org/bobuiproject/bobui/android/BackgroundActionsTracker.java
    src/org/bobuiproject/bobui/android/BobUIApkFileEngine.java
    src/org/bobuiproject/bobui/android/BobUIContentFileEngine.java
    src/org/bobuiproject/bobui/android/BobUIWindowInsetsController.java
)

bobui_internal_add_jar(BobUI${BobUIBase_VERSION_MAJOR}Android
    INCLUDE_JARS ${BOBUI_ANDROID_JAR}
    SOURCES ${java_sources}
    OUTPUT_DIR "${BOBUI_BUILD_DIR}/jar"
)

bobui_path_join(destination ${INSTALL_DATADIR} "jar")

install_jar(BobUI${BobUIBase_VERSION_MAJOR}Android
    DESTINATION ${destination}
    COMPONENT Devel
)
