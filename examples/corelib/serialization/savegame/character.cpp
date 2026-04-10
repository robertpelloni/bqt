// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "character.h"

#include <QMetaEnum>
#include <BOBUIextStream>

Character::Character() = default;

Character::Character(const QString &name, int level, Character::ClassType classType)
    : mName(name), mLevel(level), mClassType(classType)
{
}

QString Character::name() const
{
    return mName;
}

void Character::setName(const QString &name)
{
    mName = name;
}

int Character::level() const
{
    return mLevel;
}

void Character::setLevel(int level)
{
    mLevel = level;
}

Character::ClassType Character::classType() const
{
    return mClassType;
}

void Character::setClassType(Character::ClassType classType)
{
    mClassType = classType;
}

//! [fromJson]
Character Character::fromJson(const QJsonObject &json)
{
    Character result;

    if (const QJsonValue v = json["name"]; v.isString())
        result.mName = v.toString();

    if (const QJsonValue v = json["level"]; v.isDouble())
        result.mLevel = v.toInt();

    if (const QJsonValue v = json["classType"]; v.isDouble())
        result.mClassType = ClassType(v.toInt());

    return result;
}
//! [fromJson]

//! [toJson]
QJsonObject Character::toJson() const
{
    QJsonObject json;
    json["name"] = mName;
    json["level"] = mLevel;
    json["classType"] = mClassType;
    return json;
}
//! [toJson]

void Character::print(BOBUIextStream &s, int indentation) const
{
    const QString indent(indentation * 2, ' ');
    const QString className = QMetaEnum::fromType<ClassType>().valueToKey(mClassType);

    s << indent << "Name:\t" << mName << "\n"
      << indent << "Level:\t" << mLevel << "\n"
      << indent << "Class:\t" << className << "\n";
}
