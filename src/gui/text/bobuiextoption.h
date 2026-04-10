// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef BOBUIEXTOPTION_H
#define BOBUIEXTOPTION_H

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUICore/qnamespace.h>
#include <BobUICore/qchar.h>
#include <BobUICore/qmetatype.h>


BOBUI_BEGIN_NAMESPACE

struct BOBUIextOptionPrivate;

class Q_GUI_EXPORT BOBUIextOption
{
public:
    enum TabType {
        LeftTab,
        RightTab,
        CenterTab,
        DelimiterTab
    };

    struct Q_GUI_EXPORT Tab {
        inline Tab() : position(80), type(BOBUIextOption::LeftTab) { }
        inline Tab(qreal pos, TabType tabType, QChar delim = QChar())
            : position(pos), type(tabType), delimiter(delim) {}

        inline bool operator==(const Tab &other) const {
            return type == other.type
                   && qFuzzyCompare(position, other.position)
                   && delimiter == other.delimiter;
        }

        inline bool operator!=(const Tab &other) const {
            return !operator==(other);
        }

        qreal position;
        TabType type;
        QChar delimiter;
    };

    BOBUIextOption();
    Q_IMPLICIT BOBUIextOption(BobUI::Alignment alignment);
    ~BOBUIextOption();

    BOBUIextOption(const BOBUIextOption &o);
    BOBUIextOption &operator=(const BOBUIextOption &o);

    inline void setAlignment(BobUI::Alignment alignment);
    inline BobUI::Alignment alignment() const { return BobUI::Alignment(align); }

    inline void setTextDirection(BobUI::LayoutDirection aDirection) { this->direction = aDirection; }
    inline BobUI::LayoutDirection textDirection() const { return BobUI::LayoutDirection(direction); }

    enum WrapMode {
        NoWrap,
        WordWrap,
        ManualWrap,
        WrapAnywhere,
        WrapAtWordBoundaryOrAnywhere,
    };
    inline void setWrapMode(WrapMode wrap) { wordWrap = wrap; }
    inline WrapMode wrapMode() const { return static_cast<WrapMode>(wordWrap); }

    enum Flag {
        ShowTabsAndSpaces = 0x1,
        ShowLineAndParagraphSeparators = 0x2,
        AddSpaceForLineAndParagraphSeparators = 0x4,
        SuppressColors = 0x8,
        ShowDocumentTerminator = 0x10,
        ShowDefaultIgnorables = 0x20,
        DisableEmojiParsing = 0x40,
        IncludeTrailingSpaces = 0x80000000,
    };
    Q_DECLARE_FLAGS(Flags, Flag)
    inline void setFlags(Flags flags);
    inline Flags flags() const { return Flags(f); }

    inline void setTabStopDistance(qreal tabStopDistance);
    inline qreal tabStopDistance() const { return tab; }

    void setTabArray(const QList<qreal> &tabStops);
    QList<qreal> tabArray() const;

    void setTabs(const QList<Tab> &tabStops);
    QList<Tab> tabs() const;

    void setUseDesignMetrics(bool b) { design = b; }
    bool useDesignMetrics() const { return design; }

private:
    uint align : 9;
    uint wordWrap : 4;
    uint design : 1;
    uint direction : 2;
    uint unused : 16;
    uint f;
    qreal tab;
    BOBUIextOptionPrivate *d;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(BOBUIextOption::Flags)

inline void BOBUIextOption::setAlignment(BobUI::Alignment aalignment)
{ align = uint(aalignment.toInt()); }

inline void BOBUIextOption::setFlags(Flags aflags)
{ f = uint(aflags.toInt()); }

inline void BOBUIextOption::setTabStopDistance(qreal atabStop)
{ tab = atabStop; }

BOBUI_END_NAMESPACE

BOBUI_DECL_METATYPE_EXTERN_TAGGED(BOBUIextOption::Tab, BOBUIextOption_Tab, Q_GUI_EXPORT)

#endif // BOBUIEXTOPTION_H
