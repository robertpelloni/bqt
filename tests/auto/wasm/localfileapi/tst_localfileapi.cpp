// Copyright (C) 2022 The BobUI Company Ltd.
// Copyright (C) 2016 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUIGui/private/qlocalfileapi_p.h>
#include <BOBUIest>
#include <emscripten/val.h>

class tst_LocalFileApi : public QObject
{
    Q_OBJECT

private:
    emscripten::val makeAccept(std::vector<emscripten::val> types)
    {
        auto accept = emscripten::val::object();
        accept.set("application/octet-stream",
            emscripten::val::array(std::move(types)));
        return accept;
    }

    emscripten::val makeType(QString description, std::vector<emscripten::val> acceptExtensions)
    {
        using namespace emscripten;

        auto type = val::object();
        type.set("description", description.toStdString());

        auto accept = val::object();
        accept.set("application/octet-stream",
            val::array(std::move(acceptExtensions)));
        type.set("accept", makeAccept(std::move(acceptExtensions)));

        return type;
    }

    emscripten::val makeOpenFileOptions(bool acceptMultiple, std::vector<emscripten::val> types) {
        using namespace emscripten;

        auto webFilter = val::object();
        webFilter.set("types", val::array(std::move(types)));
        if (!types.empty())
            webFilter.set("excludeAcceptAllOption", val(true));
        webFilter.set("multiple", val(acceptMultiple));

        return webFilter;
    }

    emscripten::val makeSaveFileOptions(QString suggestedName, std::vector<emscripten::val> types) {
        using namespace emscripten;

        auto webFilter = val::object();
        webFilter.set("suggestedName", val(suggestedName.toStdString()));
        webFilter.set("types", val::array(std::move(types)));

        return webFilter;
    }

private Q_SLOTS:
    void fileExtensionFilterTransformation_data();
    void fileExtensionFilterTransformation();
    void acceptTransformation_data();
    void acceptTransformation();
    void typeTransformation_data();
    void typeTransformation();
    void openFileOptions_data();
    void openFileOptions();
    void saveFileOptions_data();
    void saveFileOptions();
    void fileInputAccept_data();
    void fileInputAccept();
};

bool valDeepEquals(emscripten::val lhs, emscripten::val rhs)
{
    auto json = emscripten::val::global("JSON");
    auto lhsAsJsonString = json.call<emscripten::val>("stringify", lhs);
    auto rhsAsJsonString = json.call<emscripten::val>("stringify", rhs);

    return lhsAsJsonString.equals(rhsAsJsonString);
}

void tst_LocalFileApi::fileExtensionFilterTransformation_data()
{
    BOBUIest::addColumn<QString>("bobuiFileFilter");
    BOBUIest::addColumn<std::optional<QString>>("expectedWebExtensionFilter");

    BOBUIest::newRow("PNG extension with an asterisk")
            << QString("*.png") << std::make_optional<QString>(".png");
    BOBUIest::newRow("Long extension with an asterisk")
            << QString("*.someotherfile") << std::make_optional<QString>(".someotherfile");
    BOBUIest::newRow(".dat with no asterisk")
            << QString(".dat") << std::make_optional<QString>(".dat");
    BOBUIest::newRow("Multiple asterisks") << QString("*ot*.abc") << std::optional<QString>();
    BOBUIest::newRow("Filename") << QString("abcd.abc") << std::optional<QString>();
    BOBUIest::newRow("match all") << QString("*.*") << std::optional<QString>();
}

void tst_LocalFileApi::fileExtensionFilterTransformation()
{
    QFETCH(QString, bobuiFileFilter);
    QFETCH(std::optional<QString>, expectedWebExtensionFilter);

    auto result = LocalFileApi::Type::Accept::MimeType::Extension::fromBobUI(bobuiFileFilter);
    if (expectedWebExtensionFilter) {
        QCOMPARE_EQ(expectedWebExtensionFilter, result->value());
    } else {
        QVERIFY(!result.has_value());
    }
}

void tst_LocalFileApi::acceptTransformation_data()
{
    using namespace emscripten;

    BOBUIest::addColumn<QString>("bobuiFilterList");
    BOBUIest::addColumn<QStringList>("expectedExtensionList");

    BOBUIest::newRow("Multiple types")
            << QString("*.png *.other *.txt") << QStringList{ ".png", ".other", ".txt" };

    BOBUIest::newRow("Single type") << QString("*.png") << QStringList{ ".png" };

    BOBUIest::newRow("No filter when accepts all") << QString("*.*") << QStringList();

    BOBUIest::newRow("No filter when one filter accepts all") << QString("*.* *.jpg") << QStringList();

    BOBUIest::newRow("Weird spaces") << QString("    *.jpg     *.png *.icon  ")
                                  << QStringList{ ".jpg", ".png", ".icon" };
}

void tst_LocalFileApi::acceptTransformation()
{
    QFETCH(QString, bobuiFilterList);
    QFETCH(QStringList, expectedExtensionList);

    auto result = LocalFileApi::Type::Accept::fromBobUI(bobuiFilterList);
    if (expectedExtensionList.isEmpty()) {
        QVERIFY(!result.has_value());
    } else {
        QStringList transformed;
        std::transform(result->mimeType().extensions().begin(),
                       result->mimeType().extensions().end(), std::back_inserter(transformed),
                       [](const LocalFileApi::Type::Accept::MimeType::Extension &extension) {
                           return extension.value().toString();
                       });
        QCOMPARE_EQ(expectedExtensionList, transformed);
    }
}

void tst_LocalFileApi::typeTransformation_data()
{
    using namespace emscripten;

    BOBUIest::addColumn<QString>("bobuiFilterList");
    BOBUIest::addColumn<QString>("expectedDescription");
    BOBUIest::addColumn<QStringList>("expectedExtensions");

    BOBUIest::newRow("With description")
            << QString("Text files (*.txt)") << QString("Text files") << QStringList{ ".txt" };

    BOBUIest::newRow("No description") << QString("*.jpg") << QString("") << QStringList{ ".jpg" };
}

void tst_LocalFileApi::typeTransformation()
{
    QFETCH(QString, bobuiFilterList);
    QFETCH(QString, expectedDescription);
    QFETCH(QStringList, expectedExtensions);

    auto result = LocalFileApi::Type::fromBobUI(bobuiFilterList);
    QCOMPARE_EQ(result->description(), expectedDescription);

    QStringList transformed;
    std::transform(result->accept()->mimeType().extensions().begin(),
                   result->accept()->mimeType().extensions().end(), std::back_inserter(transformed),
                   [](const LocalFileApi::Type::Accept::MimeType::Extension &extension) {
                       return extension.value().toString();
                   });
    QCOMPARE_EQ(expectedExtensions, transformed);
}

void tst_LocalFileApi::openFileOptions_data()
{
    using namespace emscripten;

    BOBUIest::addColumn<QStringList>("bobuiFilterList");
    BOBUIest::addColumn<bool>("multiple");
    BOBUIest::addColumn<emscripten::val>("expectedWebType");

    BOBUIest::newRow("Multiple files") << QStringList({"Text files (*.txt)", "Images (*.jpg *.png)", "*.bat"})
        << true
        << makeOpenFileOptions(true, { makeType("Text files", { val(".txt")}), makeType("Images", { val(".jpg"), val(".png")}), makeType("", { val(".bat")})});
    BOBUIest::newRow("Single file") << QStringList({"Text files (*.txt)", "Images (*.jpg *.png)", "*.bat"})
        << false
        << makeOpenFileOptions(false, { makeType("Text files", { val(".txt")}), makeType("Images", { val(".jpg"), val(".png")}), makeType("", { val(".bat")})});
}

void tst_LocalFileApi::openFileOptions()
{
    QFETCH(QStringList, bobuiFilterList);
    QFETCH(bool, multiple);
    QFETCH(emscripten::val, expectedWebType);

    auto result = LocalFileApi::makeOpenFileOptions(bobuiFilterList, multiple);
    QVERIFY(valDeepEquals(result, expectedWebType));
}

void tst_LocalFileApi::saveFileOptions_data()
{
    using namespace emscripten;

    BOBUIest::addColumn<QStringList>("bobuiFilterList");
    BOBUIest::addColumn<QString>("suggestedName");
    BOBUIest::addColumn<emscripten::val>("expectedWebType");

    BOBUIest::newRow("Multiple files") << QStringList({"Text files (*.txt)", "Images (*.jpg *.png)", "*.bat"})
        << "someName1"
        << makeSaveFileOptions("someName1", { makeType("Text files", { val(".txt")}), makeType("Images", { val(".jpg"), val(".png")}), makeType("", { val(".bat")})});
    BOBUIest::newRow("Single file") << QStringList({"Text files (*.txt)", "Images (*.jpg *.png)", "*.bat"})
        << "some name 2"
        << makeSaveFileOptions("some name 2", { makeType("Text files", { val(".txt")}), makeType("Images", { val(".jpg"), val(".png")}), makeType("", { val(".bat")})});
}

void tst_LocalFileApi::saveFileOptions()
{
    QFETCH(QStringList, bobuiFilterList);
    QFETCH(QString, suggestedName);
    QFETCH(emscripten::val, expectedWebType);

    auto result = LocalFileApi::makeSaveFileOptions(bobuiFilterList, suggestedName.toStdString());
    QVERIFY(valDeepEquals(result, expectedWebType));
}

void tst_LocalFileApi::fileInputAccept_data()
{
    using namespace emscripten;

    BOBUIest::addColumn<QStringList>("bobuiFilterList");
    BOBUIest::addColumn<QString>("expectedAccept");

    BOBUIest::newRow("Multiple files")
            << QStringList{ "Text files (*.txt)", "Images (*.jpg *.png)", "*.bat" }
            << ".txt,.jpg,.png,.bat";
    BOBUIest::newRow("Spaces") << QStringList{ "   Documents (*.doc)", "Everything (*.*)",
                                            "   Stuff (  *.stf   *.tng)", "    *.exe" }
                            << ".doc,.stf,.tng,.exe";
}

void tst_LocalFileApi::fileInputAccept()
{
    QFETCH(QStringList, bobuiFilterList);
    QFETCH(QString, expectedAccept);

    auto result = LocalFileApi::makeFileInputAccept(bobuiFilterList);
    QCOMPARE_EQ(expectedAccept, QString::fromStdString(result));
}

BOBUIEST_APPLESS_MAIN(tst_LocalFileApi)
#include "tst_localfileapi.moc"
