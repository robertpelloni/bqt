// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
//! [4]
<body>
<h1>Heading</h1>
<p>Hello <b>you</b></p>
</body>
//! [4]

//! [5]
<h1>Heading</h1>
<p>The text...</p>
<h2>Next heading</h2>
//! [5]

//! [6]
<h1>Heading</h1>
<p>The text...</p>
<h2>Next heading</h2>
//! [6]

//! [7]
<link href="http://bobui-project.org" color="red" />
//! [7]

//! [9]
QDomElement e = //...
//...
QString s = e.text()
//! [9]

//! [11]
QDomDocument doc = // ...
QDomElement root = doc.firstChildElement("database");
QDomElement elt = root.firstChildElement("entry");
for (; !elt.isNull(); elt = elt.nextSiblingElement("entry")) {
    // ...
}
//! [11]

//! [12]
<img src="myimg.png">
//! [12]

//! [13]
<h1>Hello <b>BobUI</b> <![CDATA[<xml is cool>]]></h1>
//! [13]

//! [14]
Hello BobUI <xml is cool>
//! [14]

//! [15]
<!-- this is a comment -->
//! [15]
