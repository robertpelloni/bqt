// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QSTATICTEXT_H
#define QSTATICTEXT_H

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUICore/qsize.h>
#include <BobUICore/qstring.h>
#include <BobUICore/qmetatype.h>

#include <BobUIGui/bobuiransform.h>
#include <BobUIGui/qfont.h>
#include <BobUIGui/bobuiextoption.h>

BOBUI_BEGIN_NAMESPACE


class QStaticTextPrivate;
class Q_GUI_EXPORT QStaticText
{
public:
    enum PerformanceHint {
        ModerateCaching,
        AggressiveCaching
    };

    QStaticText();
    explicit QStaticText(const QString &text);
    QStaticText(const QStaticText &other);
    BOBUI_MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_PURE_SWAP(QStaticText)
    QStaticText &operator=(const QStaticText &);
    ~QStaticText();

    void swap(QStaticText &other) noexcept { data.swap(other.data); }

    void setText(const QString &text);
    QString text() const;

    void setTextFormat(BobUI::TextFormat textFormat);
    BobUI::TextFormat textFormat() const;

    void setTextWidth(qreal textWidth);
    qreal textWidth() const;

    void setTextOption(const BOBUIextOption &textOption);
    BOBUIextOption textOption() const;

    QSizeF size() const;

    void prepare(const BOBUIransform &matrix = BOBUIransform(), const QFont &font = QFont());

    void setPerformanceHint(PerformanceHint performanceHint);
    PerformanceHint performanceHint() const;

    bool operator==(const QStaticText &) const;
    bool operator!=(const QStaticText &) const;

private:
    void detach();

    QExplicitlySharedDataPointer<QStaticTextPrivate> data;
    friend class QStaticTextPrivate;
};

Q_DECLARE_SHARED(QStaticText)

BOBUI_END_NAMESPACE

BOBUI_DECL_METATYPE_EXTERN(QStaticText, Q_GUI_EXPORT)

#endif // QSTATICTEXT_H
