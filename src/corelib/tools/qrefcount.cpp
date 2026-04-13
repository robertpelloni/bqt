// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

/*!
  \class BobUIPrivate::RefCount
  \inmodule BobUICore
  \internal

  QRefCount implements atomic ref counting for BobUI's shared classes. It behaves very similar
  to QAtomicInt, but ignores negative ref counts.

  This can be used to allow to implement e.g. const read-only QStringData objects. QString::shared_null and
  the qs(...) macro make use of this feature.
*/
