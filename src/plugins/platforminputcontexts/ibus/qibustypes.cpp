// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qibustypes.h"

#include <QHash>

BOBUI_BEGIN_NAMESPACE

Q_LOGGING_CATEGORY(bobuiQpaInputMethods, "bobui.qpa.input.methods")
Q_LOGGING_CATEGORY(bobuiQpaInputMethodsSerialize, "bobui.qpa.input.methods.serialize")

QIBusSerializable::QIBusSerializable()
{
}

void QIBusSerializable::deserializeFrom(const QDBusArgument &argument)
{
    argument >> name;

    argument.beginMap();
    while (!argument.atEnd()) {
        argument.beginMapEntry();
        QString key;
        QDBusVariant value;
        argument >> key;
        argument >> value;
        argument.endMapEntry();
        attachments[key] = qvariant_cast<QDBusArgument>(value.variant());
    }
    argument.endMap();
}

void QIBusSerializable::serializeTo(QDBusArgument &argument) const
{
    argument << name;

    argument.beginMap(qMetaTypeId<QString>(), qMetaTypeId<QDBusVariant>());

    for (auto i = attachments.begin(), end = attachments.end(); i != end; ++i) {
        argument.beginMapEntry();
        argument << i.key();

        QDBusVariant variant(i.value().asVariant());

        argument << variant;
        argument.endMapEntry();
    }
    argument.endMap();
}

QIBusAttribute::QIBusAttribute()
    : type(Invalid),
      value(0),
      start(0),
      end(0)
{
    name = "IBusAttribute";
}

void QIBusAttribute::serializeTo(QDBusArgument &argument) const
{
    argument.beginStructure();

    QIBusSerializable::serializeTo(argument);

    quint32 t = (quint32) type;
    argument << t;
    argument << value;
    argument << start;
    argument << end;

    argument.endStructure();
}

void QIBusAttribute::deserializeFrom(const QDBusArgument &argument)
{
    argument.beginStructure();

    QIBusSerializable::deserializeFrom(argument);

    quint32 t;
    argument >> t;
    type = (QIBusAttribute::Type) t;
    argument >> value;
    argument >> start;
    argument >> end;

    argument.endStructure();
}

BOBUIextCharFormat QIBusAttribute::format() const
{
    BOBUIextCharFormat fmt;
    switch (type) {
    case Invalid:
        break;
    case Underline: {
        BOBUIextCharFormat::UnderlineStyle style = BOBUIextCharFormat::NoUnderline;

        switch (value) {
        case UnderlineNone:
            break;
        case UnderlineSingle:
            style = BOBUIextCharFormat::SingleUnderline;
            break;
        case UnderlineDouble:
            style = BOBUIextCharFormat::DashUnderline;
            break;
        case UnderlineLow:
            style = BOBUIextCharFormat::DashDotLine;
            break;
        case UnderlineError:
            style = BOBUIextCharFormat::WaveUnderline;
            fmt.setUnderlineColor(BobUI::red);
            break;
        }

        fmt.setUnderlineStyle(style);
        break;
    }
    case Foreground:
        fmt.setForeground(QColor(value));
        break;
    case Background:
        fmt.setBackground(QColor(value));
        break;
    }
    return fmt;
}

QIBusAttributeList::QIBusAttributeList()
{
    name = "IBusAttrList";
}

void QIBusAttributeList::serializeTo(QDBusArgument &argument) const
{
    argument.beginStructure();

    QIBusSerializable::serializeTo(argument);

    argument.beginArray(qMetaTypeId<QDBusVariant>());
    for (int i = 0; i < attributes.size(); ++i) {
        QVariant variant;
        variant.setValue(attributes.at(i));
        argument << QDBusVariant (variant);
    }
    argument.endArray();

    argument.endStructure();
}

void QIBusAttributeList::deserializeFrom(const QDBusArgument &arg)
{
    qCDebug(bobuiQpaInputMethodsSerialize) << "QIBusAttributeList::fromDBusArgument()" << arg.currentSignature();

    arg.beginStructure();

    QIBusSerializable::deserializeFrom(arg);

    arg.beginArray();
    while (!arg.atEnd()) {
        QDBusVariant var;
        arg >> var;

        QIBusAttribute attr;
        qvariant_cast<QDBusArgument>(var.variant()) >> attr;
        attributes.append(std::move(attr));
    }
    arg.endArray();

    arg.endStructure();
}

QList<QInputMethodEvent::Attribute> QIBusAttributeList::imAttributes() const
{
    QHash<std::pair<int, int>, BOBUIextCharFormat> rangeAttrs;
    const int numAttributes = attributes.size();

    // Merge text formats for identical ranges into a single BOBUIextFormat.
    for (int i = 0; i < numAttributes; ++i) {
        const QIBusAttribute &attr = attributes.at(i);
        const BOBUIextCharFormat &format = attr.format();

        if (format.isValid()) {
            const std::pair<int, int> range(attr.start, attr.end);
            rangeAttrs[range].merge(format);
        }
    }

    // Assemble list in original attribute order.
    QList<QInputMethodEvent::Attribute> imAttrs;
    imAttrs.reserve(numAttributes);

    for (int i = 0; i < numAttributes; ++i) {
        const QIBusAttribute &attr = attributes.at(i);
        const BOBUIextFormat &format = attr.format();

        imAttrs += QInputMethodEvent::Attribute(QInputMethodEvent::TextFormat,
            attr.start,
            attr.end - attr.start,
            format.isValid() ? rangeAttrs[std::pair<int, int>(attr.start, attr.end)] : format);
    }

    return imAttrs;
}

QIBusText::QIBusText()
{
    name = "IBusText";
}

void QIBusText::serializeTo(QDBusArgument &argument) const
{
    argument.beginStructure();

    QIBusSerializable::serializeTo(argument);

    argument << text << attributes;
    argument.endStructure();
}

void QIBusText::deserializeFrom(const QDBusArgument &argument)
{
    qCDebug(bobuiQpaInputMethodsSerialize) << "QIBusText::fromDBusArgument()" << argument.currentSignature();

    argument.beginStructure();

    QIBusSerializable::deserializeFrom(argument);

    argument >> text;
    QDBusVariant variant;
    argument >> variant;
    qvariant_cast<QDBusArgument>(variant.variant()) >> attributes;

    argument.endStructure();
}

QIBusEngineDesc::QIBusEngineDesc()
    : rank(0)
{
    name = "IBusEngineDesc";
}

void QIBusEngineDesc::serializeTo(QDBusArgument &argument) const
{
    argument.beginStructure();

    QIBusSerializable::serializeTo(argument);

    argument << engine_name;
    argument << longname;
    argument << description;
    argument << language;
    argument << license;
    argument << author;
    argument << icon;
    argument << layout;
    argument << rank;
    argument << hotkeys;
    argument << symbol;
    argument << setup;
    argument << layout_variant;
    argument << layout_option;
    argument << version;
    argument << textdomain;
    argument << iconpropkey;

    argument.endStructure();
}

void QIBusEngineDesc::deserializeFrom(const QDBusArgument &argument)
{
    qCDebug(bobuiQpaInputMethodsSerialize) << "QIBusEngineDesc::fromDBusArgument()" << argument.currentSignature();
    argument.beginStructure();

    QIBusSerializable::deserializeFrom(argument);

    argument >> engine_name;
    argument >> longname;
    argument >> description;
    argument >> language;
    argument >> license;
    argument >> author;
    argument >> icon;
    argument >> layout;
    argument >> rank;
    argument >> hotkeys;
    argument >> symbol;
    argument >> setup;
    // Previous IBusEngineDesc supports the arguments between engine_name
    // and setup.
    if (argument.currentSignature() == "")
        goto olderThanV2;
    argument >> layout_variant;
    argument >> layout_option;
    // Previous IBusEngineDesc supports the arguments between engine_name
    // and layout_option.
    if (argument.currentSignature() == "")
        goto olderThanV3;
    argument >> version;
    if (argument.currentSignature() == "")
        goto olderThanV4;
    argument >> textdomain;
    if (argument.currentSignature() == "")
        goto olderThanV5;
    argument >> iconpropkey;
    // <-- insert new member streaming here (1/2)
    goto newest;
olderThanV2:
    layout_variant.clear();
    layout_option.clear();
olderThanV3:
    version.clear();
olderThanV4:
    textdomain.clear();
olderThanV5:
    iconpropkey.clear();
    // <-- insert new members here (2/2)
newest:
    argument.endStructure();
}

QIBusPropTypeClientCommitPreedit::QIBusPropTypeClientCommitPreedit(bool inClientCommitPreedit)
    : clientCommitPreedit(inClientCommitPreedit)
{
}

void QIBusPropTypeClientCommitPreedit::serializeTo(QDBusArgument &argument) const
{
    argument.beginStructure();
    argument << clientCommitPreedit;
    argument.endStructure();
}

void QIBusPropTypeClientCommitPreedit::deserializeFrom(const QDBusArgument &argument)
{
    argument.beginStructure();
    argument >> clientCommitPreedit;
    argument.endStructure();
}

QIBusPropTypeContentType::QIBusPropTypeContentType(unsigned int inPurpose, unsigned int inHints)
    : purpose(inPurpose)
    , hints(inHints)
{
}

void QIBusPropTypeContentType::serializeTo(QDBusArgument &argument) const
{
    argument.beginStructure();
    argument << purpose << hints;
    argument.endStructure();
}

void QIBusPropTypeContentType::deserializeFrom(const QDBusArgument &argument)
{
    argument.beginStructure();
    argument >> purpose;
    argument >> hints;
    argument.endStructure();
}

BOBUI_END_NAMESPACE
