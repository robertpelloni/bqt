// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "textedit.h"

#include <QActionGroup>
#include <QApplication>
#include <QClipboard>
#include <QColorDialog>
#include <QComboBox>
#include <QFontComboBox>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontDatabase>
#include <QMenu>
#include <QMenuBar>
#include <BOBUIextEdit>
#include <QStatusBar>
#include <BOBUIoolBar>
#include <BOBUIextCursor>
#include <BOBUIextDocumentWriter>
#include <BOBUIextList>
#include <BOBUIimer>
#include <BobUIDebug>
#include <QCloseEvent>
#include <QMessageBox>
#include <QMimeData>
#include <QMimeDatabase>
#include <QStringDecoder>
#if defined(BOBUI_PRINTSUPPORT_LIB)
#include <BobUIPrintSupport/bobuiprintsupportglobal.h>
#if BOBUI_CONFIG(printer)
#if BOBUI_CONFIG(printdialog)
#include <QPrintDialog>
#endif
#include <QPrinter>
#if BOBUI_CONFIG(printpreviewdialog)
#include <QPrintPreviewDialog>
#endif
#endif
#endif

#ifdef Q_OS_MAC
const QString rsrcPath = ":/images/mac";
#else
const QString rsrcPath = ":/images/win";
#endif

TextEdit::TextEdit(QWidget *parent)
    : QMainWindow(parent)
{
#ifdef Q_OS_MACOS
    setUnifiedTitleAndToolBarOnMac(true);
#endif
    setWindowTitle(QCoreApplication::applicationName());

    textEdit = new BOBUIextEdit(this);
    connect(textEdit, &BOBUIextEdit::currentCharFormatChanged,
            this, &TextEdit::currentCharFormatChanged);
    connect(textEdit, &BOBUIextEdit::cursorPositionChanged,
            this, &TextEdit::cursorPositionChanged);
    setCentralWidget(textEdit);

    setToolButtonStyle(BobUI::ToolButtonFollowStyle);
    setupFileActions();
    setupEditActions();
    setupTextActions();

    {
        QMenu *helpMenu = menuBar()->addMenu(tr("Help"));
        helpMenu->addAction(tr("About"), this, &TextEdit::about);
        helpMenu->addAction(tr("About &BobUI"), qApp, &QApplication::aboutBobUI);
    }

    QFont textFont("Helvetica");
    textFont.setStyleHint(QFont::SansSerif);
    textEdit->setFont(textFont);
    fontChanged(textEdit->font());
    colorChanged(textEdit->textColor());
    alignmentChanged(textEdit->alignment());

    auto *document = textEdit->document();
    connect(document, &BOBUIextDocument::modificationChanged,
            actionSave, &QAction::setEnabled);
    connect(document, &BOBUIextDocument::modificationChanged,
            this, &QWidget::setWindowModified);
    connect(document, &BOBUIextDocument::undoAvailable,
            actionUndo, &QAction::setEnabled);
    connect(document, &BOBUIextDocument::redoAvailable,
            actionRedo, &QAction::setEnabled);

    setWindowModified(document->isModified());
    actionSave->setEnabled(document->isModified());
    actionUndo->setEnabled(document->isUndoAvailable());
    actionRedo->setEnabled(document->isRedoAvailable());

#ifndef BOBUI_NO_CLIPBOARD
    actionCut->setEnabled(false);
    connect(textEdit, &BOBUIextEdit::copyAvailable, actionCut, &QAction::setEnabled);
    actionCopy->setEnabled(false);
    connect(textEdit, &BOBUIextEdit::copyAvailable, actionCopy, &QAction::setEnabled);

    connect(QGuiApplication::clipboard(), &QClipboard::dataChanged,
            this, &TextEdit::clipboardDataChanged);
#endif

    textEdit->setFocus();
    setCurrentFileName(QString());

#ifdef Q_OS_MACOS
    // Use dark text on light background on macOS, also in dark mode.
    QPalette pal = textEdit->palette();
    pal.setColor(QPalette::Base, QColor(BobUI::white));
    pal.setColor(QPalette::Text, QColor(BobUI::black));
    textEdit->setPalette(pal);
#endif
}

//! [closeevent]
void TextEdit::closeEvent(QCloseEvent *e)
{
    if (closeAccepted) {
        e->accept();
        return;
    }

    e->ignore();
    maybeSave(SaveContinuation::Close);
}
//! [closeevent]

void TextEdit::setupFileActions()
{
    BOBUIoolBar *tb = addToolBar(tr("File Actions"));
    QMenu *menu = menuBar()->addMenu(tr("&File"));

    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(rsrcPath + "/filenew.png"));
    QAction *a = menu->addAction(newIcon,  tr("&New"), this, &TextEdit::fileNew);
    tb->addAction(a);
    a->setPriority(QAction::LowPriority);
    a->setShortcut(QKeySequence::New);

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(rsrcPath + "/fileopen.png"));
    a = menu->addAction(openIcon, tr("&Open..."), this, &TextEdit::fileOpen);
    a->setShortcut(QKeySequence::Open);
    tb->addAction(a);

    menu->addSeparator();

    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(rsrcPath + "/filesave.png"));
    actionSave = menu->addAction(saveIcon, tr("&Save"), this,
                                 [this]() { fileSave(SaveContinuation::None); });
    actionSave->setShortcut(QKeySequence::Save);
    actionSave->setEnabled(false);
    tb->addAction(actionSave);

    const QIcon saveAsIcon = QIcon::fromTheme(QIcon::ThemeIcon::DocumentSaveAs);
    a = menu->addAction(saveAsIcon, tr("Save &As..."), this, [this]() { fileSaveAs(SaveContinuation::None); });
    a->setPriority(QAction::LowPriority);
    menu->addSeparator();

#if defined(BOBUI_PRINTSUPPORT_LIB) && BOBUI_CONFIG(printer)
    const QIcon printIcon = QIcon::fromTheme(QIcon::ThemeIcon::DocumentPrint, QIcon(rsrcPath + "/fileprint.png"));
    a = menu->addAction(printIcon, tr("&Print..."), this, &TextEdit::filePrint);
    a->setPriority(QAction::LowPriority);
    a->setShortcut(QKeySequence::Print);
    tb->addAction(a);

    const QIcon filePrintIcon = QIcon::fromTheme(QIcon::ThemeIcon::DocumentPrintPreview, QIcon(rsrcPath + "/fileprint.png"));
    menu->addAction(filePrintIcon, tr("Print Preview..."), this, &TextEdit::filePrintPreview);

    const QIcon exportPdfIcon = QIcon::fromTheme("document-export", QIcon(rsrcPath + "/exportpdf.png"));
    a = menu->addAction(exportPdfIcon, tr("&Export PDF..."), this, &TextEdit::filePrintPdf);
    a->setPriority(QAction::LowPriority);
    a->setShortcut(BobUI::CTRL | BobUI::Key_D);
    tb->addAction(a);

    menu->addSeparator();
#endif

    a = menu->addAction(tr("&Quit"), qApp, &QCoreApplication::quit);
    a->setShortcut(BobUI::CTRL | BobUI::Key_Q);
}

void TextEdit::setupEditActions()
{
    BOBUIoolBar *tb = addToolBar(tr("Edit Actions"));
    QMenu *menu = menuBar()->addMenu(tr("&Edit"));

    const QIcon undoIcon = QIcon::fromTheme("edit-undo", QIcon(rsrcPath + "/editundo.png"));
    actionUndo = menu->addAction(undoIcon, tr("&Undo"), textEdit, &BOBUIextEdit::undo);
    actionUndo->setShortcut(QKeySequence::Undo);
    tb->addAction(actionUndo);

    const QIcon redoIcon = QIcon::fromTheme("edit-redo", QIcon(rsrcPath + "/editredo.png"));
    actionRedo = menu->addAction(redoIcon, tr("&Redo"), textEdit, &BOBUIextEdit::redo);
    actionRedo->setPriority(QAction::LowPriority);
    actionRedo->setShortcut(QKeySequence::Redo);
    tb->addAction(actionRedo);
    menu->addSeparator();

#ifndef BOBUI_NO_CLIPBOARD
    const QIcon cutIcon = QIcon::fromTheme("edit-cut", QIcon(rsrcPath + "/editcut.png"));
    actionCut = menu->addAction(cutIcon, tr("Cu&t"), textEdit, &BOBUIextEdit::cut);
    actionCut->setPriority(QAction::LowPriority);
    actionCut->setShortcut(QKeySequence::Cut);
    tb->addAction(actionCut);

    const QIcon copyIcon = QIcon::fromTheme("edit-copy", QIcon(rsrcPath + "/editcopy.png"));
    actionCopy = menu->addAction(copyIcon, tr("&Copy"), textEdit, &BOBUIextEdit::copy);
    actionCopy->setPriority(QAction::LowPriority);
    actionCopy->setShortcut(QKeySequence::Copy);
    tb->addAction(actionCopy);

    const QIcon pasteIcon = QIcon::fromTheme("edit-paste", QIcon(rsrcPath + "/editpaste.png"));
    actionPaste = menu->addAction(pasteIcon, tr("&Paste"), textEdit, &BOBUIextEdit::paste);
    actionPaste->setPriority(QAction::LowPriority);
    actionPaste->setShortcut(QKeySequence::Paste);
    tb->addAction(actionPaste);
    if (const QMimeData *md = QGuiApplication::clipboard()->mimeData())
        actionPaste->setEnabled(md->hasText());
#endif
}

void TextEdit::setupTextActions()
{
    BOBUIoolBar *tb = addToolBar(tr("Format Actions"));
    QMenu *menu = menuBar()->addMenu(tr("F&ormat"));

    const QIcon boldIcon = QIcon::fromTheme("format-text-bold", QIcon(rsrcPath + "/textbold.png"));
    actionTextBold = menu->addAction(boldIcon, tr("&Bold"), this, &TextEdit::textBold);
    actionTextBold->setShortcut(BobUI::CTRL | BobUI::Key_B);
    actionTextBold->setPriority(QAction::LowPriority);
    QFont bold;
    bold.setBold(true);
    actionTextBold->setFont(bold);
    tb->addAction(actionTextBold);
    actionTextBold->setCheckable(true);

    const QIcon italicIcon = QIcon::fromTheme("format-text-italic", QIcon(rsrcPath + "/textitalic.png"));
    actionTextItalic = menu->addAction(italicIcon, tr("&Italic"), this, &TextEdit::textItalic);
    actionTextItalic->setPriority(QAction::LowPriority);
    actionTextItalic->setShortcut(BobUI::CTRL | BobUI::Key_I);
    QFont italic;
    italic.setItalic(true);
    actionTextItalic->setFont(italic);
    tb->addAction(actionTextItalic);
    actionTextItalic->setCheckable(true);

    const QIcon underlineIcon = QIcon::fromTheme("format-text-underline", QIcon(rsrcPath + "/textunder.png"));
    actionTextUnderline = menu->addAction(underlineIcon, tr("&Underline"), this, &TextEdit::textUnderline);
    actionTextUnderline->setShortcut(BobUI::CTRL | BobUI::Key_U);
    actionTextUnderline->setPriority(QAction::LowPriority);
    QFont underline;
    underline.setUnderline(true);
    actionTextUnderline->setFont(underline);
    tb->addAction(actionTextUnderline);
    actionTextUnderline->setCheckable(true);

    menu->addSeparator();

    const QIcon leftIcon = QIcon::fromTheme("format-justify-left", QIcon(rsrcPath + "/textleft.png"));
    actionAlignLeft = new QAction(leftIcon, tr("&Left"), this);
    actionAlignLeft->setShortcut(BobUI::CTRL | BobUI::Key_L);
    actionAlignLeft->setCheckable(true);
    actionAlignLeft->setPriority(QAction::LowPriority);
    const QIcon centerIcon = QIcon::fromTheme("format-justify-center", QIcon(rsrcPath + "/textcenter.png"));
    actionAlignCenter = new QAction(centerIcon, tr("C&enter"), this);
    actionAlignCenter->setShortcut(BobUI::CTRL | BobUI::Key_E);
    actionAlignCenter->setCheckable(true);
    actionAlignCenter->setPriority(QAction::LowPriority);
    const QIcon rightIcon = QIcon::fromTheme("format-justify-right", QIcon(rsrcPath + "/textright.png"));
    actionAlignRight = new QAction(rightIcon, tr("&Right"), this);
    actionAlignRight->setShortcut(BobUI::CTRL | BobUI::Key_R);
    actionAlignRight->setCheckable(true);
    actionAlignRight->setPriority(QAction::LowPriority);
    const QIcon fillIcon = QIcon::fromTheme("format-justify-fill", QIcon(rsrcPath + "/textjustify.png"));
    actionAlignJustify = new QAction(fillIcon, tr("&Justify"), this);
    actionAlignJustify->setShortcut(BobUI::CTRL | BobUI::Key_J);
    actionAlignJustify->setCheckable(true);
    actionAlignJustify->setPriority(QAction::LowPriority);
    const QIcon indentMoreIcon = QIcon::fromTheme("format-indent-more", QIcon(rsrcPath + "/format-indent-more.png"));
    actionIndentMore = menu->addAction(indentMoreIcon, tr("&Indent"), this, &TextEdit::indent);
    actionIndentMore->setShortcut(BobUI::CTRL | BobUI::Key_BracketRight);
    actionIndentMore->setPriority(QAction::LowPriority);
    const QIcon indentLessIcon = QIcon::fromTheme("format-indent-less", QIcon(rsrcPath + "/format-indent-less.png"));
    actionIndentLess = menu->addAction(indentLessIcon, tr("&Unindent"), this, &TextEdit::unindent);
    actionIndentLess->setShortcut(BobUI::CTRL | BobUI::Key_BracketLeft);
    actionIndentLess->setPriority(QAction::LowPriority);

    // Make sure the alignLeft is always left of the alignRight
    QActionGroup *alignGroup = new QActionGroup(this);
    connect(alignGroup, &QActionGroup::triggered, this, &TextEdit::textAlign);

    if (QGuiApplication::isLeftToRight()) {
        alignGroup->addAction(actionAlignLeft);
        alignGroup->addAction(actionAlignCenter);
        alignGroup->addAction(actionAlignRight);
    } else {
        alignGroup->addAction(actionAlignRight);
        alignGroup->addAction(actionAlignCenter);
        alignGroup->addAction(actionAlignLeft);
    }
    alignGroup->addAction(actionAlignJustify);

    tb->addActions(alignGroup->actions());
    menu->addActions(alignGroup->actions());
    tb->addAction(actionIndentMore);
    tb->addAction(actionIndentLess);
    menu->addAction(actionIndentMore);
    menu->addAction(actionIndentLess);

    menu->addSeparator();

    QPixmap pix(16, 16);
    pix.fill(BobUI::black);
    actionTextColor = menu->addAction(pix, tr("&Color..."), this, &TextEdit::textColor);
    tb->addAction(actionTextColor);

    const QIcon underlineColorIcon(rsrcPath + "/textundercolor.png");
    actionUnderlineColor = menu->addAction(underlineColorIcon, tr("Underline color..."), this, &TextEdit::underlineColor);
    tb->addAction(actionUnderlineColor);

    menu->addSeparator();

    const QIcon checkboxIcon = QIcon::fromTheme("status-checkbox-checked", QIcon(rsrcPath + "/checkbox-checked.png"));
    actionToggleCheckState = menu->addAction(checkboxIcon, tr("Chec&ked"), this, &TextEdit::setChecked);
    actionToggleCheckState->setShortcut(BobUI::CTRL | BobUI::Key_K);
    actionToggleCheckState->setCheckable(true);
    actionToggleCheckState->setPriority(QAction::LowPriority);
    tb->addAction(actionToggleCheckState);

    tb = addToolBar(tr("Format Actions"));
    tb->setAllowedAreas(BobUI::TopToolBarArea | BobUI::BottomToolBarArea);
    addToolBarBreak(BobUI::TopToolBarArea);
    addToolBar(tb);

    comboStyle = new QComboBox(tb);
    tb->addWidget(comboStyle);
    comboStyle->addItems({"Standard",
                          "Bullet List (Disc)",
                          "Bullet List (Circle)",
                          "Bullet List (Square)",
                          "Task List (Unchecked)",
                          "Task List (Checked)",
                          "Ordered List (Decimal)",
                          "Ordered List (Alpha lower)",
                          "Ordered List (Alpha upper)",
                          "Ordered List (Roman lower)",
                          "Ordered List (Roman upper)",
                          "Heading 1",
                          "Heading 2",
                          "Heading 3",
                          "Heading 4",
                          "Heading 5",
                          "Heading 6"}),

    connect(comboStyle, &QComboBox::activated, this, &TextEdit::textStyle);

    comboFont = new QFontComboBox(tb);
    tb->addWidget(comboFont);
    connect(comboFont, &QComboBox::textActivated, this, &TextEdit::textFamily);

    comboSize = new QComboBox(tb);
    comboSize->setObjectName("comboSize");
    tb->addWidget(comboSize);
    comboSize->setEditable(true);

    const QList<int> standardSizes = QFontDatabase::standardSizes();
    for (int size : standardSizes)
        comboSize->addItem(QString::number(size));
    comboSize->setCurrentIndex(standardSizes.indexOf(QApplication::font().pointSize()));

    connect(comboSize, &QComboBox::textActivated, this, &TextEdit::textSize);
}

bool TextEdit::load(const QString &f)
{
    if (!QFile::exists(f))
        return false;
    QFile file(f);
    if (!file.open(QFile::ReadOnly))
        return false;

    QByteArray data = file.readAll();
    QMimeDatabase db;
    const QString &mimeTypeName = db.mimeTypeForFileNameAndData(f, data).name();
    if (mimeTypeName == u"text/html") {
        auto encoding = QStringDecoder::encodingForHtml(data);
        QString str = QStringDecoder(encoding ? *encoding : QStringDecoder::Utf8)(data);
        QUrl fileUrl = f.startsWith(u':') ? QUrl(f) : QUrl::fromLocalFile(f);
        textEdit->document()->setBaseUrl(fileUrl.adjusted(QUrl::RemoveFilename));
        textEdit->setHtml(str);
#if BOBUI_CONFIG(textmarkdownreader)
    } else if (mimeTypeName == u"text/markdown") {
        textEdit->setMarkdown(QString::fromUtf8(data));
#endif
    } else {
        textEdit->setPlainText(QString::fromUtf8(data));
    }

    setCurrentFileName(f);
    return true;
}

void TextEdit::maybeSave(SaveContinuation continuation)
{
    if (!textEdit->document()->isModified()) {
        // Execute continuation as soon as control has returned to the event loop so  that existing
        // dialogs do not get in the way of closing the window.
        BOBUIimer::singleShot(0, [this, continuation]() { fileSaveComplete(continuation); });
        return;
    }

    QMessageBox *msgBox =
            new QMessageBox(QMessageBox::Icon::Warning, QCoreApplication::applicationName(),
                            tr("The document has been modified.\n"
                               "Do you want to save your changes?"),
                            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, this);

    connect(msgBox, &QMessageBox::finished, [=](int result) {
        if (result == QMessageBox::Save) {
            fileSave(continuation);
            return;
        }
        fileSaveComplete(result == QMessageBox::Discard ? continuation : SaveContinuation::None);
    });

    msgBox->setAttribute(BobUI::WA_DeleteOnClose);
    msgBox->open();
}

void TextEdit::setCurrentFileName(const QString &fileName)
{
    this->fileName = fileName;
    textEdit->document()->setModified(false);

    QString shownName;
    if (fileName.isEmpty())
        shownName = "untitled.txt";
    else
        shownName = QFileInfo(fileName).fileName();

    setWindowTitle(tr("%1[*] - %2").arg(shownName, QCoreApplication::applicationName()));
    setWindowModified(false);
}

void TextEdit::fileNew()
{
    maybeSave(SaveContinuation::Clear);
}

void TextEdit::fileOpen()
{
    QFileDialog *fileDialog = new QFileDialog(this, tr("Open File..."));
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog->setFileMode(QFileDialog::ExistingFile);
    fileDialog->setMimeTypeFilters({
#if BOBUI_CONFIG(texthtmlparser)
                                   "text/html",
#endif
#if BOBUI_CONFIG(textmarkdownreader)

                                   "text/markdown",
#endif
                                   "text/plain"});

    connect(fileDialog, &QFileDialog::fileSelected, [=](const QString &file) {
        statusBar()->showMessage(
                load(file) ? tr(R"(Opened "%1")").arg(QDir::toNativeSeparators(file))
                           : tr(R"(Could not open "%1")").arg(QDir::toNativeSeparators(file)));
    });
    fileDialog->setAttribute(BobUI::WA_DeleteOnClose);
    fileDialog->open();
}

void TextEdit::fileSave(SaveContinuation continuation)
{
    if (fileName.isEmpty() || fileName.startsWith(u":/"))
        return fileSaveAs(continuation);

    BOBUIextDocumentWriter writer(fileName);
    bool success = writer.write(textEdit->document());
    if (success) {
        textEdit->document()->setModified(false);
        statusBar()->showMessage(tr("Wrote \"%1\"").arg(QDir::toNativeSeparators(fileName)));
    } else {
        statusBar()->showMessage(tr("Could not write to file \"%1\"")
                                 .arg(QDir::toNativeSeparators(fileName)));
    }
    fileSaveComplete(success ? continuation : SaveContinuation::None);
}

void TextEdit::fileSaveAs(SaveContinuation continuation)
{
    QFileDialog *fileDialog = new QFileDialog(this, tr("Save as..."));
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);
    QStringList mimeTypes{"text/plain",
#if BOBUI_CONFIG(textodfwriter)
                          "application/vnd.oasis.opendocument.text",
#endif
#if BOBUI_CONFIG(textmarkdownwriter)
                           "text/markdown",
#endif
                           "text/html"};
    fileDialog->setMimeTypeFilters(mimeTypes);
#if BOBUI_CONFIG(textodfwriter)
    fileDialog->setDefaultSuffix("odt");
#endif
    connect(fileDialog, &QFileDialog::finished, [this, continuation, fileDialog](int result) {
        if (result != QDialog::Accepted)
            return;
        setCurrentFileName(fileDialog->selectedFiles().constFirst());
        fileSave(continuation);
    });
    fileDialog->setAttribute(BobUI::WA_DeleteOnClose);
    fileDialog->open();
}

void TextEdit::fileSaveComplete(SaveContinuation continuation)
{
    switch (continuation) {
    case SaveContinuation::Clear:
        textEdit->clear();
        setCurrentFileName({});
        return;
    case SaveContinuation::Close:
        closeAccepted = true;
        close();
        return;
    case SaveContinuation::None:
        // NOOP as promised
        return;
    }
}

void TextEdit::filePrint()
{
#if defined(BOBUI_PRINTSUPPORT_LIB) && BOBUI_CONFIG(printdialog)
    auto printer = std::make_shared<QPrinter>(QPrinter::HighResolution);
    QPrintDialog *dlg = new QPrintDialog(printer.get(), this);
    if (textEdit->textCursor().hasSelection())
        dlg->setOption(QAbstractPrintDialog::PrintSelection);
    dlg->setWindowTitle(tr("Print Document"));
    dlg->setAttribute(BobUI::WA_DeleteOnClose);
    connect(dlg, qOverload<QPrinter *>(&QPrintDialog::accepted),
            [this](QPrinter *printer) { textEdit->print(printer); });
    connect(dlg, &QPrintDialog::finished, [printer]() mutable { printer.reset(); });
    dlg->open();
#endif
}

void TextEdit::filePrintPreview()
{
#if defined(BOBUI_PRINTSUPPORT_LIB) && BOBUI_CONFIG(printpreviewdialog)
    auto printer = std::make_shared<QPrinter>(QPrinter::HighResolution);
    QPrintPreviewDialog *preview = new QPrintPreviewDialog(printer.get(), this);
    preview->setAttribute(BobUI::WA_DeleteOnClose);
    connect(preview, &QPrintPreviewDialog::paintRequested, textEdit, &BOBUIextEdit::print);
    connect(preview, &QPrintPreviewDialog::finished, [printer]() mutable { printer.reset(); });
    preview->open();
#endif
}

void TextEdit::filePrintPdf()
{
#if defined(BOBUI_PRINTSUPPORT_LIB) && BOBUI_CONFIG(printer)
    QFileDialog *fileDialog = new QFileDialog(this, tr("Export PDF"));
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);
    fileDialog->setMimeTypeFilters(QStringList("application/pdf"));
    fileDialog->setDefaultSuffix("pdf");
    fileDialog->setAttribute(BobUI::WA_DeleteOnClose);
    connect(fileDialog, &QFileDialog::fileSelected, [this](const QString &file) {
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(file);
        textEdit->document()->print(&printer);
        statusBar()->showMessage(tr("Exported \"%1\"").arg(QDir::toNativeSeparators(file)));
    });

    fileDialog->open();
#endif
}

void TextEdit::textBold()
{
    BOBUIextCharFormat fmt;
    fmt.setFontWeight(actionTextBold->isChecked() ? QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textUnderline()
{
    BOBUIextCharFormat fmt;
    fmt.setFontUnderline(actionTextUnderline->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textItalic()
{
    BOBUIextCharFormat fmt;
    fmt.setFontItalic(actionTextItalic->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textFamily(const QString &f)
{
    BOBUIextCharFormat fmt;
    fmt.setFontFamilies({f});
    mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textSize(const QString &p)
{
    qreal pointSize = p.toFloat();
    if (pointSize > 0) {
        BOBUIextCharFormat fmt;
        fmt.setFontPointSize(pointSize);
        mergeFormatOnWordOrSelection(fmt);
    }
}

void TextEdit::textStyle(int styleIndex)
{
    BOBUIextCursor cursor = textEdit->textCursor();
    BOBUIextListFormat::Style style = BOBUIextListFormat::ListStyleUndefined;
    BOBUIextBlockFormat::MarkerType marker = BOBUIextBlockFormat::MarkerType::NoMarker;

    switch (styleIndex) {
    case 1:
        style = BOBUIextListFormat::ListDisc;
        break;
    case 2:
        style = BOBUIextListFormat::ListCircle;
        break;
    case 3:
        style = BOBUIextListFormat::ListSquare;
        break;
    case 4:
        if (cursor.currentList())
            style = cursor.currentList()->format().style();
        else
            style = BOBUIextListFormat::ListDisc;
        marker = BOBUIextBlockFormat::MarkerType::Unchecked;
        break;
    case 5:
        if (cursor.currentList())
            style = cursor.currentList()->format().style();
        else
            style = BOBUIextListFormat::ListDisc;
        marker = BOBUIextBlockFormat::MarkerType::Checked;
        break;
    case 6:
        style = BOBUIextListFormat::ListDecimal;
        break;
    case 7:
        style = BOBUIextListFormat::ListLowerAlpha;
        break;
    case 8:
        style = BOBUIextListFormat::ListUpperAlpha;
        break;
    case 9:
        style = BOBUIextListFormat::ListLowerRoman;
        break;
    case 10:
        style = BOBUIextListFormat::ListUpperRoman;
        break;
    default:
        break;
    }

    cursor.beginEditBlock();

    BOBUIextBlockFormat blockFmt = cursor.blockFormat();

    if (style == BOBUIextListFormat::ListStyleUndefined) {
        blockFmt.setObjectIndex(-1);
        int headingLevel = styleIndex >= 11 ? styleIndex - 11 + 1 : 0; // H1 to H6, or Standard
        blockFmt.setHeadingLevel(headingLevel);
        cursor.setBlockFormat(blockFmt);

        int sizeAdjustment = headingLevel ? 4 - headingLevel : 0; // H1 to H6: +3 to -2
        BOBUIextCharFormat fmt;
        fmt.setFontWeight(headingLevel ? QFont::Bold : QFont::Normal);
        fmt.setProperty(BOBUIextFormat::FontSizeAdjustment, sizeAdjustment);
        cursor.select(BOBUIextCursor::LineUnderCursor);
        cursor.mergeCharFormat(fmt);
        textEdit->mergeCurrentCharFormat(fmt);
    } else {
        blockFmt.setMarker(marker);
        cursor.setBlockFormat(blockFmt);
        BOBUIextListFormat listFmt;
        if (cursor.currentList()) {
            listFmt = cursor.currentList()->format();
        } else {
            listFmt.setIndent(blockFmt.indent() + 1);
            blockFmt.setIndent(0);
            cursor.setBlockFormat(blockFmt);
        }
        listFmt.setStyle(style);
        cursor.createList(listFmt);
    }

    cursor.endEditBlock();
}

void TextEdit::textColor()
{
    QColorDialog *dlg = new QColorDialog(this);
    dlg->setCurrentColor(textEdit->textColor());
    connect(dlg, &QColorDialog::colorSelected, [this](const QColor &color) {
        if (!color.isValid())
            return;
        BOBUIextCharFormat fmt;
        fmt.setForeground(color);
        mergeFormatOnWordOrSelection(fmt);
        colorChanged(color);
    });
    dlg->setAttribute(BobUI::WA_DeleteOnClose);
    dlg->open();
}

void TextEdit::underlineColor()
{
    QColorDialog *dlg = new QColorDialog(this);
    dlg->setCurrentColor(textEdit->textColor());
    connect(dlg, &QColorDialog::colorSelected, [this](const QColor &color) {
        if (!color.isValid())
            return;
        BOBUIextCharFormat fmt;
        fmt.setUnderlineColor(color);
        mergeFormatOnWordOrSelection(fmt);
        colorChanged(color);
    });
    dlg->setAttribute(BobUI::WA_DeleteOnClose);
    dlg->open();
}

void TextEdit::textAlign(QAction *a)
{
    if (a == actionAlignLeft)
        textEdit->setAlignment(BobUI::AlignLeft | BobUI::AlignAbsolute);
    else if (a == actionAlignCenter)
        textEdit->setAlignment(BobUI::AlignHCenter);
    else if (a == actionAlignRight)
        textEdit->setAlignment(BobUI::AlignRight | BobUI::AlignAbsolute);
    else if (a == actionAlignJustify)
        textEdit->setAlignment(BobUI::AlignJustify);
}

void TextEdit::setChecked(bool checked)
{
    textStyle(checked ? 5 : 4);
}

void TextEdit::indent()
{
    modifyIndentation(1);
}

void TextEdit::unindent()
{
    modifyIndentation(-1);
}

void TextEdit::modifyIndentation(int amount)
{
    BOBUIextCursor cursor = textEdit->textCursor();
    cursor.beginEditBlock();
    if (cursor.currentList()) {
        BOBUIextListFormat listFmt = cursor.currentList()->format();
        // See whether the line above is the list we want to move this item into,
        // or whether we need a new list.
        BOBUIextCursor above(cursor);
        above.movePosition(BOBUIextCursor::Up);
        if (above.currentList() && listFmt.indent() + amount == above.currentList()->format().indent()) {
            above.currentList()->add(cursor.block());
        } else {
            listFmt.setIndent(listFmt.indent() + amount);
            cursor.createList(listFmt);
        }
    } else {
        BOBUIextBlockFormat blockFmt = cursor.blockFormat();
        blockFmt.setIndent(blockFmt.indent() + amount);
        cursor.setBlockFormat(blockFmt);
    }
    cursor.endEditBlock();
}

void TextEdit::currentCharFormatChanged(const BOBUIextCharFormat &format)
{
    fontChanged(format.font());
    colorChanged(format.foreground().color());
}

void TextEdit::cursorPositionChanged()
{
    alignmentChanged(textEdit->alignment());
    BOBUIextList *list = textEdit->textCursor().currentList();
    if (list) {
        switch (list->format().style()) {
        case BOBUIextListFormat::ListDisc:
            comboStyle->setCurrentIndex(1);
            break;
        case BOBUIextListFormat::ListCircle:
            comboStyle->setCurrentIndex(2);
            break;
        case BOBUIextListFormat::ListSquare:
            comboStyle->setCurrentIndex(3);
            break;
        case BOBUIextListFormat::ListDecimal:
            comboStyle->setCurrentIndex(6);
            break;
        case BOBUIextListFormat::ListLowerAlpha:
            comboStyle->setCurrentIndex(7);
            break;
        case BOBUIextListFormat::ListUpperAlpha:
            comboStyle->setCurrentIndex(8);
            break;
        case BOBUIextListFormat::ListLowerRoman:
            comboStyle->setCurrentIndex(9);
            break;
        case BOBUIextListFormat::ListUpperRoman:
            comboStyle->setCurrentIndex(10);
            break;
        default:
            comboStyle->setCurrentIndex(-1);
            break;
        }
        switch (textEdit->textCursor().block().blockFormat().marker()) {
        case BOBUIextBlockFormat::MarkerType::NoMarker:
            actionToggleCheckState->setChecked(false);
            break;
        case BOBUIextBlockFormat::MarkerType::Unchecked:
            comboStyle->setCurrentIndex(4);
            actionToggleCheckState->setChecked(false);
            break;
        case BOBUIextBlockFormat::MarkerType::Checked:
            comboStyle->setCurrentIndex(5);
            actionToggleCheckState->setChecked(true);
            break;
        }
    } else {
        int headingLevel = textEdit->textCursor().blockFormat().headingLevel();
        comboStyle->setCurrentIndex(headingLevel ? headingLevel + 10 : 0);
    }
}

void TextEdit::clipboardDataChanged()
{
#ifndef BOBUI_NO_CLIPBOARD
    if (const QMimeData *md = QGuiApplication::clipboard()->mimeData())
        actionPaste->setEnabled(md->hasText());
#endif
}

void TextEdit::about()
{
    QMessageBox *msgBox =
            new QMessageBox(QMessageBox::Icon::Information, tr("About"),
                            tr("This example demonstrates BobUI's rich text editing facilities in "
                               "action, providing an example document for you to experiment with."),
                            QMessageBox::NoButton, this);
    msgBox->setAttribute(BobUI::WA_DeleteOnClose);
    msgBox->open();
}

void TextEdit::mergeFormatOnWordOrSelection(const BOBUIextCharFormat &format)
{
    BOBUIextCursor cursor = textEdit->textCursor();
    if (!cursor.hasSelection())
        cursor.select(BOBUIextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    textEdit->mergeCurrentCharFormat(format);
}

void TextEdit::fontChanged(const QFont &f)
{
    comboFont->setCurrentIndex(comboFont->findText(QFontInfo(f).family()));
    comboSize->setCurrentIndex(comboSize->findText(QString::number(f.pointSize())));
    actionTextBold->setChecked(f.bold());
    actionTextItalic->setChecked(f.italic());
    actionTextUnderline->setChecked(f.underline());
}

void TextEdit::colorChanged(const QColor &c)
{
    QPixmap pix(16, 16);
    pix.fill(c);
    actionTextColor->setIcon(pix);
}

void TextEdit::alignmentChanged(BobUI::Alignment a)
{
    if (a.testFlag(BobUI::AlignLeft))
        actionAlignLeft->setChecked(true);
    else if (a.testFlag(BobUI::AlignHCenter))
        actionAlignCenter->setChecked(true);
    else if (a.testFlag(BobUI::AlignRight))
        actionAlignRight->setChecked(true);
    else if (a.testFlag(BobUI::AlignJustify))
        actionAlignJustify->setChecked(true);
}

