// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QCOCOADRAG_H
#define QCOCOADRAG_H

#include <qpa/qplatformdrag.h>
#include <BobUIGui/private/qsimpledrag_p.h>
#include <BobUIGui/private/qinternalmimedata_p.h>

#include <BobUICore/private/qcore_mac_p.h>


Q_FORWARD_DECLARE_OBJC_CLASS(NSView);
Q_FORWARD_DECLARE_OBJC_CLASS(NSEvent);
Q_FORWARD_DECLARE_OBJC_CLASS(NSPasteboard);

BOBUI_BEGIN_NAMESPACE

class QDrag;
class QEventLoop;
class QMimeData;

class QCocoaDrag : public QPlatformDrag
{
public:
    QCocoaDrag();
    ~QCocoaDrag();

    QMimeData *dragMimeData();
    BobUI::DropAction drag(QDrag *m_drag) override;

    BobUI::DropAction defaultAction(BobUI::DropActions possibleActions,
                                 BobUI::KeyboardModifiers modifiers) const override;

    /**
    * to meet NSView dragImage:at guarantees, we need to record the original
    * event and view when handling an event in QNSView
    */
    void setLastInputEvent(NSEvent *event, NSView *view);

    void setAcceptedAction(BobUI::DropAction act);
    void exitDragLoop();
private:
    QDrag *m_drag;
    NSEvent *m_lastEvent;
    QObjCWeakPointer<NSView> m_lastView;
    BobUI::DropAction m_executed_drop_action;
    QEventLoop *m_internalDragLoop = nullptr;

    bool maybeDragMultipleItems();

    QPixmap dragPixmap(QDrag *drag, QPoint &hotSpot) const;
};

class QCocoaDropData : public QInternalMimeData
{
public:
    QCocoaDropData(NSPasteboard *pasteboard);
    ~QCocoaDropData();
protected:
    bool hasFormat_sys(const QString &mimeType) const;
    QStringList formats_sys() const;
    QVariant retrieveData_sys(const QString &mimeType, QMetaType type) const;
public:
    CFStringRef dropPasteboard;
};


BOBUI_END_NAMESPACE

#endif
