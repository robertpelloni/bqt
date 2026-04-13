// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

package org.bobuiproject.bobui.android.testdatapackage;

public class BobUIJniEnvironmentTestClass
{
    private static native void callbackFromJava(String message);
    private static native void namedCallbackFromJava(String message);
    private static native void memberCallbackFromJava(String message);
    private static native void namedMemberCallbackFromJava(String message);
    private static native void namespaceCallbackFromJava(String message);
    private static native void intCallbackFromJava(int value);

    public final int INT_FIELD = 123;
    public static final int S_INT_FIELD = 321;

    BobUIJniEnvironmentTestClass() {}

    public static void appendJavaToString(String message)
    {
        callbackFromJava("From Java: " + message);
    }

    public static void namedAppendJavaToString(String message)
    {
        namedCallbackFromJava("From Java (named): " + message);
    }

    public static void memberAppendJavaToString(String message)
    {
        memberCallbackFromJava("From Java (member): " + message);
    }

    public static void namedMemberAppendJavaToString(String message)
    {
        namedMemberCallbackFromJava("From Java (named member): " + message);
    }

    public static void namespaceAppendJavaToString(String message)
    {
        namespaceCallbackFromJava("From Java (namespace): " + message);
    }

    public static void convertToInt(String message)
    {
        intCallbackFromJava(Integer.parseInt(message));
    }
}

class BobUIJniEnvironmentTestClassNoCtor
{
    private static native void callbackFromJavaNoCtor(String message);

    public static void appendJavaToString(String message)
    {
        callbackFromJavaNoCtor("From Java (no ctor): " + message);
    }
}

