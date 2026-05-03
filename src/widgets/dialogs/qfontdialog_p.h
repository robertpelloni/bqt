// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QFONTDIALOG_P_H
#define QFONTDIALOG_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//
//

#include <BobUIWidgets/private/bobuiwidgetsglobal_p.h>
#include "private/qdialog_p.h"
#include "qfontdatabase.h"
#include "qfontdialog.h"
#include <qpa/qplatformdialoghelper.h>

#include <BobUICore/qpointer.h>
#include "qsharedpointer.h"

BOBUI_REQUIRE_CONFIG(fontdialog);

BOBUI_BEGIN_NAMESPACE

class QBoxLayout;
class QCheckBox;
class QComboBox;
class QDialogButtonBox;
class QFontListView;
class QGroupBox;
class QLabel;
class QLineEdit;

class Q_AUTOTEST_EXPORT QFontDialogPrivate : public QDialogPrivate
{
    Q_DECLARE_PUBLIC(QFontDialog)

public:
    QFontDialogPrivate();
    ~QFontDialogPrivate();

    QPlatformFontDialogHelper *platformFontDialogHelper() const
        { return static_cast<QPlatformFontDialogHelper *>(platformHelper()); }

    void updateFamilies();
    void updateStyles();
    void updateSizes();

    static QFont getFont(bool *ok, const QFont &initial, QWidget *parent,
                         const QString &title, QFontDialog::FontDialogOptions options);

    void init();
    void sizeChanged(const QString &);
    void familyHighlighted(int);
    void writingSystemHighlighted(int);
    void styleHighlighted(int);
    void sizeHighlighted(int);
    void updateSample();
    void updateSampleFont(const QFont &newFont);
    void retranslateStrings();

    QLabel *familyAccel;
    QLineEdit *familyEdit;
    QFontListView *familyList;

    QLabel *styleAccel;
    QLineEdit *styleEdit;
    QFontListView *styleList;

    QLabel *sizeAccel;
    QLineEdit *sizeEdit;
    QFontListView *sizeList;

    QGroupBox *effects;
    QCheckBox *strikeout;
    QCheckBox *underline;
    QComboBox *color;

    QGroupBox *sample;
    QLineEdit *sampleEdit;

    QLabel *writingSystemAccel;
    QComboBox *writingSystemCombo;

    QBoxLayout *buttonLayout;
    QBoxLayout *effectsLayout;
    QBoxLayout *sampleLayout;
    QBoxLayout *sampleEditLayout;

    QDialogButtonBox *buttonBox;

    QString family;
    QFontDatabase::WritingSystem writingSystem;
    QString style;
    int size;
    bool smoothScalable;
    QFont selectedFont;
    QSharedPointer<QFontDialogOptions> options;
    QPointer<QObject> receiverToDisconnectOnClose;
    QByteArray memberToDisconnectOnClose;

    bool canBeNativeDialog() const override;
    void setVisible(bool visible) override;
    void _q_runNativeAppModalPanel();

private:
    virtual void initHelper(QPlatformDialogHelper *) override;
    virtual void helperPrepareShow(QPlatformDialogHelper *) override;
};

BOBUI_END_NAMESPACE

#endif // QFONTDIALOG_P_H
