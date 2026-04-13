// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qerrormessage.h"

#include "qapplication.h"
#include "qcheckbox.h"
#include "qlabel.h"
#include "qlayout.h"
#if BOBUI_CONFIG(messagebox)
#include "qmessagebox.h"
#endif
#include "qpushbutton.h"
#include "qstringlist.h"
#include "bobuiextedit.h"
#include "qdialog_p.h"
#include "qpixmap.h"
#include "qmetaobject.h"
#include "bobuihread.h"
#include "qset.h"

#include <queue>

#include <stdio.h>
#include <stdlib.h>

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

class QErrorMessagePrivate : public QDialogPrivate
{
    Q_DECLARE_PUBLIC(QErrorMessage)
public:
    struct Message {
        QString content;
        QString type;
    };

    QPushButton * ok;
    QCheckBox * again;
    BOBUIextEdit * errors;
    QLabel * icon;
    std::queue<Message> pending;
    QSet<QString> doNotShow;
    QSet<QString> doNotShowType;
    QString currentMessage;
    QString currentType;

    bool isMessageToBeShown(const QString &message, const QString &type) const;
    bool nextPending();
    void retranslateStrings();

    void setVisible(bool) override;

private:
    void initHelper(QPlatformDialogHelper *) override;
    void helperPrepareShow(QPlatformDialogHelper *) override;
};


void QErrorMessagePrivate::initHelper(QPlatformDialogHelper *helper)
{
    Q_Q(QErrorMessage);
    auto *messageDialogHelper = static_cast<QPlatformMessageDialogHelper *>(helper);
    QObject::connect(messageDialogHelper, &QPlatformMessageDialogHelper::checkBoxStateChanged, q,
        [this](BobUI::CheckState state) {
            again->setCheckState(state);
        }
    );
    QObject::connect(messageDialogHelper, &QPlatformMessageDialogHelper::clicked, q,
        [this](QPlatformDialogHelper::StandardButton, QPlatformDialogHelper::ButtonRole) {
            Q_Q(QErrorMessage);
            q->accept();
        }
    );
}

void QErrorMessagePrivate::helperPrepareShow(QPlatformDialogHelper *helper)
{
    Q_Q(QErrorMessage);
    auto *messageDialogHelper = static_cast<QPlatformMessageDialogHelper *>(helper);
    QSharedPointer<QMessageDialogOptions> options = QMessageDialogOptions::create();
    options->setText(currentMessage);
    options->setWindowTitle(q->windowTitle());
    options->setText(QErrorMessage::tr("An error occurred"));
    options->setInformativeText(currentMessage);
    options->setStandardIcon(QMessageDialogOptions::Critical);
    options->setCheckBox(again->text(), again->checkState());
    messageDialogHelper->setOptions(options);
}

namespace {
class QErrorMessageTextView : public BOBUIextEdit
{
public:
    QErrorMessageTextView(QWidget *parent)
        : BOBUIextEdit(parent) { setReadOnly(true); }

    virtual QSize minimumSizeHint() const override;
    virtual QSize sizeHint() const override;
};
} // unnamed namespace

QSize QErrorMessageTextView::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize QErrorMessageTextView::sizeHint() const
{
    return QSize(250, 75);
}

/*!
    \class QErrorMessage

    \brief The QErrorMessage class provides an error message display dialog.

    \ingroup standard-dialog
    \inmodule BobUIWidgets

    An error message widget consists of a text label and a checkbox. The
    checkbox lets the user control whether the same error message will be
    displayed again in the future, typically displaying the text,
    "Show this message again" translated into the appropriate local
    language.

    For production applications, the class can be used to display messages which
    the user only needs to see once. To use QErrorMessage like this, you create
    the dialog in the usual way, and show it by calling the showMessage() slot or
    connecting signals to it.

    The static bobuiHandler() function installs a message handler
    using qInstallMessageHandler() and creates a QErrorMessage that displays
    qDebug(), qWarning() and qFatal() messages. This is most useful in
    environments where no console is available to display warnings and
    error messages.

    In both cases QErrorMessage will queue pending messages and display
    them in order, with each new message being shown as soon as the user
    has accepted the previous message. Once the user has specified that a
    message is not to be shown again it is automatically skipped, and the
    dialog will show the next appropriate message in the queue.

    The \l{dialogs/standarddialogs}{Standard Dialogs} example shows
    how to use QErrorMessage as well as other built-in BobUI dialogs.

    \image qerrormessage.png {Dialog with a network-related error message}

    \sa QMessageBox, QStatusBar::showMessage(), {Standard Dialogs Example}
*/

static QErrorMessage * bobuiMessageHandler = nullptr;

static void deleteStaticcQErrorMessage() // post-routine
{
    if (bobuiMessageHandler) {
        delete bobuiMessageHandler;
        bobuiMessageHandler = nullptr;
    }
}

static bool metFatal = false;

static QString msgType2i18nString(BobUIMsgType t)
{
    static_assert(BobUIDebugMsg == 0);
    static_assert(BobUIWarningMsg == 1);
    static_assert(BobUICriticalMsg == 2);
    static_assert(BobUIFatalMsg == 3);
    static_assert(BobUIInfoMsg == 4);

    // adjust the array below if any of the above fire...

    const char * const messages[] = {
        BOBUI_TRANSLATE_NOOP("QErrorMessage", "Debug Message:"),
        BOBUI_TRANSLATE_NOOP("QErrorMessage", "Warning:"),
        BOBUI_TRANSLATE_NOOP("QErrorMessage", "Critical Error:"),
        BOBUI_TRANSLATE_NOOP("QErrorMessage", "Fatal Error:"),
        BOBUI_TRANSLATE_NOOP("QErrorMessage", "Information:"),
    };
    Q_ASSERT(size_t(t) < sizeof messages / sizeof *messages);

    return QCoreApplication::translate("QErrorMessage", messages[t]);
}

static BobUIMessageHandler originalMessageHandler = nullptr;

static void jump(BobUIMsgType t, const QMessageLogContext &context, const QString &m)
{
    const auto forwardToOriginalHandler = qScopeGuard([&] {
       if (originalMessageHandler)
            originalMessageHandler(t, context, m);
    });

    if (!bobuiMessageHandler)
        return;

    auto *defaultCategory = QLoggingCategory::defaultCategory();
    if (context.category && defaultCategory
        && qstrcmp(context.category, defaultCategory->categoryName()) != 0)
        return;

    QString rich = "<p><b>"_L1 + msgType2i18nString(t) + "</b></p>"_L1
                   + BobUI::convertFromPlainText(m, BobUI::WhiteSpaceNormal);

    // ### work around text engine quirk
    if (rich.endsWith("</p>"_L1))
        rich.chop(4);

    if (!metFatal) {
        if (BOBUIhread::isMainThread()) {
            bobuiMessageHandler->showMessage(rich);
        } else {
            QMetaObject::invokeMethod(bobuiMessageHandler,
                                      "showMessage",
                                      BobUI::QueuedConnection,
                                      Q_ARG(QString, rich));
        }
        metFatal = (t == BobUIFatalMsg);
    }
}


/*!
    Constructs and installs an error handler window with the given \a
    parent.

    The default \l{BobUI::WindowModality} {window modality} of the dialog
    depends on the platform. The window modality can be overridden via
    setWindowModality() before calling showMessage().
*/

QErrorMessage::QErrorMessage(QWidget * parent)
    : QDialog(*new QErrorMessagePrivate, parent)
{
    Q_D(QErrorMessage);

#if defined(Q_OS_MACOS)
    setWindowModality(parent ? BobUI::WindowModal : BobUI::ApplicationModal);
#endif

    d->icon = new QLabel(this);
    d->errors = new QErrorMessageTextView(this);
    d->again = new QCheckBox(this);
    d->ok = new QPushButton(this);
    QGridLayout * grid = new QGridLayout(this);

    connect(d->ok, SIGNAL(clicked()), this, SLOT(accept()));

    grid->addWidget(d->icon,   0, 0, BobUI::AlignTop);
    grid->addWidget(d->errors, 0, 1);
    grid->addWidget(d->again,  1, 1, BobUI::AlignTop);
    grid->addWidget(d->ok,     2, 0, 1, 2, BobUI::AlignCenter);
    grid->setColumnStretch(1, 42);
    grid->setRowStretch(0, 42);

#if BOBUI_CONFIG(messagebox)
    const auto iconSize = style()->pixelMetric(QStyle::PM_MessageBoxIconSize, nullptr, this);
    const auto icon = style()->standardIcon(QStyle::SP_MessageBoxInformation, nullptr, this);
    d->icon->setPixmap(icon.pixmap(QSize(iconSize, iconSize), devicePixelRatio()));
    d->icon->setAlignment(BobUI::AlignHCenter | BobUI::AlignTop);
#endif
    d->again->setChecked(true);
    d->ok->setFocus();

    d->retranslateStrings();
}


/*!
    Destroys the error message dialog.
*/

QErrorMessage::~QErrorMessage()
{
    if (this == bobuiMessageHandler) {
        bobuiMessageHandler = nullptr;
        BobUIMessageHandler currentMessagHandler = qInstallMessageHandler(nullptr);
        if (currentMessagHandler != jump)
            qInstallMessageHandler(currentMessagHandler);
        else
            qInstallMessageHandler(originalMessageHandler);
        originalMessageHandler = nullptr;
    }
}


/*! \reimp */

void QErrorMessage::done(int a)
{
    Q_D(QErrorMessage);
    if (!d->again->isChecked()) {
        if (d->currentType.isEmpty()) {
            if (!d->currentMessage.isEmpty())
                d->doNotShow.insert(d->currentMessage);
        } else {
            d->doNotShowType.insert(d->currentType);
        }
    }
    d->currentMessage.clear();
    d->currentType.clear();

    QDialog::done(a);

    if (d->nextPending()) {
        show();
    } else {
        if (this == bobuiMessageHandler && metFatal)
            exit(1);
    }
}


/*!
    Returns a pointer to a QErrorMessage object that outputs the
    default BobUI messages. This function creates such an object, if there
    isn't one already.

    The object will only output log messages of QLoggingCategory::defaultCategory().

    The object will forward all messages to the original message handler.

    \sa qInstallMessageHandler
*/

QErrorMessage * QErrorMessage::bobuiHandler()
{
    if (!bobuiMessageHandler) {
        bobuiMessageHandler = new QErrorMessage(nullptr);
        qAddPostRoutine(deleteStaticcQErrorMessage); // clean up
        bobuiMessageHandler->setWindowTitle(QCoreApplication::applicationName());
        originalMessageHandler = qInstallMessageHandler(jump);
    }
    return bobuiMessageHandler;
}


/*! \internal */

bool QErrorMessagePrivate::isMessageToBeShown(const QString &message, const QString &type) const
{
    return !message.isEmpty()
        && (type.isEmpty() ? !doNotShow.contains(message) : !doNotShowType.contains(type));
}

bool QErrorMessagePrivate::nextPending()
{
    while (!pending.empty()) {
        QString message = std::move(pending.front().content);
        QString type = std::move(pending.front().type);
        pending.pop();
        if (isMessageToBeShown(message, type)) {
#ifndef BOBUI_NO_TEXTHTMLPARSER
            errors->setHtml(message);
#else
            errors->setPlainText(message);
#endif
            currentMessage = std::move(message);
            currentType = std::move(type);
            again->setChecked(true);
            return true;
        }
    }
    return false;
}


/*!
    Shows the given message, \a message, and returns immediately. If the user
    has requested for the message not to be shown again, this function does
    nothing.

    Normally, the message is displayed immediately. However, if there are
    pending messages, it will be queued to be displayed later.
*/

void QErrorMessage::showMessage(const QString &message)
{
    showMessage(message, QString());
}

/*!
    \overload

    Shows the given message, \a message, and returns immediately. If the user
    has requested for messages of type, \a type, not to be shown again, this
    function does nothing.

    Normally, the message is displayed immediately. However, if there are
    pending messages, it will be queued to be displayed later.

    \sa showMessage()
*/

void QErrorMessage::showMessage(const QString &message, const QString &type)
{
    Q_D(QErrorMessage);
    if (!d->isMessageToBeShown(message, type))
        return;
    d->pending.push({message, type});
    if (!isVisible() && d->nextPending())
        show();
}

void QErrorMessagePrivate::setVisible(bool visible)
{
    // Don't use Q_Q here! This function is called from ~QDialog,
    // so Q_Q calling q_func() invokes undefined behavior (invalid cast in q_func()).
    const auto q = static_cast<QDialog *>(q_ptr);

    if (canBeNativeDialog())
        setNativeDialogVisible(visible);

    // Update WA_DontShowOnScreen based on whether the native dialog was shown,
    // so that QDialog::setVisible(visible) below updates the QWidget state correctly,
    // but skips showing the non-native version.
    q->setAttribute(BobUI::WA_DontShowOnScreen, nativeDialogInUse);

    QDialogPrivate::setVisible(visible);
}

/*!
    \reimp
*/
void QErrorMessage::changeEvent(QEvent *e)
{
    Q_D(QErrorMessage);
    if (e->type() == QEvent::LanguageChange) {
        d->retranslateStrings();
    }
    QDialog::changeEvent(e);
}

void QErrorMessagePrivate::retranslateStrings()
{
    again->setText(QErrorMessage::tr("&Show this message again"));
    ok->setText(QErrorMessage::tr("&OK"));
}

BOBUI_END_NAMESPACE

#include "moc_qerrormessage.cpp"
