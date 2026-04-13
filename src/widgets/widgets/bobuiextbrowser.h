// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef BOBUIEXTBROWSER_H
#define BOBUIEXTBROWSER_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUIWidgets/bobuiextedit.h>
#include <BobUICore/qurl.h>

BOBUI_REQUIRE_CONFIG(textbrowser);

BOBUI_BEGIN_NAMESPACE

class BOBUIextBrowserPrivate;

class Q_WIDGETS_EXPORT BOBUIextBrowser : public BOBUIextEdit
{
    Q_OBJECT

    Q_PROPERTY(QUrl source READ source WRITE setSource)
    Q_PROPERTY(BOBUIextDocument::ResourceType sourceType READ sourceType)
    Q_OVERRIDE(bool modified SCRIPTABLE false)
    Q_OVERRIDE(bool readOnly DESIGNABLE false SCRIPTABLE false)
    Q_OVERRIDE(bool undoRedoEnabled DESIGNABLE false SCRIPTABLE false)
    Q_PROPERTY(QStringList searchPaths READ searchPaths WRITE setSearchPaths)
    Q_PROPERTY(bool openExternalLinks READ openExternalLinks WRITE setOpenExternalLinks)
    Q_PROPERTY(bool openLinks READ openLinks WRITE setOpenLinks)

public:
    explicit BOBUIextBrowser(QWidget* parent = nullptr);
    virtual ~BOBUIextBrowser();

    QUrl source() const;
    BOBUIextDocument::ResourceType sourceType() const;

    QStringList searchPaths() const;
    void setSearchPaths(const QStringList &paths);

    virtual QVariant loadResource(int type, const QUrl &name) override;

    bool isBackwardAvailable() const;
    bool isForwardAvailable() const;
    void clearHistory();
    QString historyTitle(int) const;
    QUrl historyUrl(int) const;
    int backwardHistoryCount() const;
    int forwardHistoryCount() const;

    bool openExternalLinks() const;
    void setOpenExternalLinks(bool open);

    bool openLinks() const;
    void setOpenLinks(bool open);

public Q_SLOTS:
    void setSource(const QUrl &name, BOBUIextDocument::ResourceType type = BOBUIextDocument::UnknownResource);
    virtual void backward();
    virtual void forward();
    virtual void home();
    virtual void reload();

Q_SIGNALS:
    void backwardAvailable(bool);
    void forwardAvailable(bool);
    void historyChanged();
    void sourceChanged(const QUrl &);
    void highlighted(const QUrl &);
    void anchorClicked(const QUrl &);

protected:
    bool event(QEvent *e) override;
    virtual void keyPressEvent(QKeyEvent *ev) override;
    virtual void mouseMoveEvent(QMouseEvent *ev) override;
    virtual void mousePressEvent(QMouseEvent *ev) override;
    virtual void mouseReleaseEvent(QMouseEvent *ev) override;
    virtual void focusOutEvent(QFocusEvent *ev) override;
    virtual bool focusNextPrevChild(bool next) override;
    virtual void paintEvent(QPaintEvent *e) override;
    virtual void doSetSource(const QUrl &name, BOBUIextDocument::ResourceType type = BOBUIextDocument::UnknownResource);

private:
    Q_DISABLE_COPY(BOBUIextBrowser)
    Q_DECLARE_PRIVATE(BOBUIextBrowser)
};

BOBUI_END_NAMESPACE

#endif // BOBUIEXTBROWSER_H
