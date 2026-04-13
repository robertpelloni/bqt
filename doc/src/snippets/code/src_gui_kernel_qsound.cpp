// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

//! [0]
QSound::play("mysounds/bells.wav");
//! [0]


//! [1]
QSound bells("mysounds/bells.wav");
bells.play();
//! [1]
