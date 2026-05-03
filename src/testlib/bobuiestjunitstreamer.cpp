// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <BobUITest/private/bobuiestjunitstreamer_p.h>
#include <BobUITest/private/qjunittestlogger_p.h>
#include <BobUITest/private/bobuiestelement_p.h>
#include <BobUITest/private/bobuiestelementattribute_p.h>
#include <BobUITest/bobuiestassert.h>
#include <BobUITest/private/bobuiestlog_p.h>
#include <BobUITest/private/bobuiestresult_p.h>
#include <BobUITest/private/qxmltestlogger_p.h>

BOBUI_BEGIN_NAMESPACE

BOBUIestJUnitStreamer::BOBUIestJUnitStreamer(QJUnitTestLogger *logger)
    : testLogger(logger)
{
    BOBUIEST_ASSERT(testLogger);
}

BOBUIestJUnitStreamer::~BOBUIestJUnitStreamer() = default;

void BOBUIestJUnitStreamer::indentForElement(const BOBUIestElement* element, char* buf, int size)
{
    if (size == 0) return;

    buf[0] = 0;

    if (!element) return;

    char* endbuf = buf + size;
    element = element->parentElement();
    while (element && buf+2 < endbuf) {
        *(buf++) = ' ';
        *(buf++) = ' ';
        *buf = 0;
        element = element->parentElement();
    }
}

void BOBUIestJUnitStreamer::formatStart(const BOBUIestElement *element, BOBUIestCharBuffer *formatted) const
{
    if (!element || !formatted )
        return;

    char indent[20];
    indentForElement(element, indent, sizeof(indent));

    if (element->elementType() == BOBUIest::LET_Text) {
        BOBUIest::bobui_asprintf(formatted, "%s<![CDATA[", indent);
        return;
    }

    BOBUIest::bobui_asprintf(formatted, "%s<%s", indent, element->elementName());
}

void BOBUIestJUnitStreamer::formatEnd(const BOBUIestElement *element, BOBUIestCharBuffer *formatted) const
{
    if (!element || !formatted )
        return;

    if (element->childElements().empty()) {
        formatted->data()[0] = '\0';
        return;
    }

    char indent[20];
    indentForElement(element, indent, sizeof(indent));

    BOBUIest::bobui_asprintf(formatted, "%s</%s>\n", indent, element->elementName());
}

bool BOBUIestJUnitStreamer::formatAttributes(const BOBUIestElement* element,
                                          const BOBUIestElementAttribute *attribute,
                                          BOBUIestCharBuffer *formatted) const
{
    if (!attribute || !formatted )
        return false;

    BOBUIest::AttributeIndex attrindex = attribute->index();

    if (element && element->elementType() == BOBUIest::LET_Text) {
        BOBUIEST_ASSERT(attrindex == BOBUIest::AI_Value);
        return QXmlTestLogger::xmlCdata(formatted, attribute->value());
    }

    BOBUIestCharBuffer quotedValue;
    if (QXmlTestLogger::xmlQuote(&quotedValue, attribute->value())) {
        return BOBUIest::bobui_asprintf(formatted, " %s=\"%s\"",
                                  attribute->name(), quotedValue.constData()) != 0;
    }
    return false;
}

void BOBUIestJUnitStreamer::formatAfterAttributes(const BOBUIestElement *element, BOBUIestCharBuffer *formatted) const
{
    if (!element || !formatted )
        return;

    if (element->elementType() == BOBUIest::LET_Text) {
        BOBUIest::bobui_asprintf(formatted, "]]>\n");
        return;
    }

    if (element->childElements().empty())
        BOBUIest::bobui_asprintf(formatted, "/>\n");
    else
        BOBUIest::bobui_asprintf(formatted, ">\n");
}

void BOBUIestJUnitStreamer::output(BOBUIestElement *element) const
{
    BOBUIEST_ASSERT(element);

    if (!element->parentElement())
        outputString("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");

    BOBUIestCharBuffer buf;

    formatStart(element, &buf);
    outputString(buf.data());

    outputElementAttributes(element, element->attributes());

    formatAfterAttributes(element, &buf);
    outputString(buf.data());

    if (!element->childElements().empty())
        outputElements(element->childElements());

    formatEnd(element, &buf);
    outputString(buf.data());
}

void BOBUIestJUnitStreamer::outputElements(const std::vector<BOBUIestElement*> &elements) const
{
    for (auto *element : elements)
        output(element);
}

void BOBUIestJUnitStreamer::outputElementAttributes(const BOBUIestElement* element, const std::vector<BOBUIestElementAttribute*> &attributes) const
{
    BOBUIestCharBuffer buf;

    for (auto *attribute : attributes) {
        if (formatAttributes(element, attribute, &buf))
            outputString(buf.data());
    }
}

void BOBUIestJUnitStreamer::outputString(const char *msg) const
{
    testLogger->outputString(msg);
}

BOBUI_END_NAMESPACE
