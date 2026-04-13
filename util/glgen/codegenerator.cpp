// Copyright (C) 2013 Klaralvdalens Datakonsult AB (KDAB)
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

#include "codegenerator.h"

#include <QDebug>
#include <QFile>
#include <QSettings>
#include <BOBUIextStream>

static const QString extensionRegistryFileName = QStringLiteral("qopengl-extension-registry.ini");
static const QString extensionIdGroupName = QStringLiteral("ExtensionIds");

CodeGenerator::CodeGenerator()
    : m_parser(0)
{
}

void CodeGenerator::generateCoreClasses(const QString &baseFileName) const
{
    // Output header and implementation files for the backend and base class
    writeCoreHelperClasses(baseFileName + QStringLiteral(".h"), Declaration);
    writeCoreHelperClasses(baseFileName + QStringLiteral(".cpp"), Definition);

    // Output the per-version and profile public classes
    writeCoreClasses(baseFileName);

    // We also need to generate a factory class that can be used by
    // QOpenGLContext to actually create version function objects
    writeCoreFactoryHeader(baseFileName + QStringLiteral("factory_p.h"));
    writeCoreFactoryImplementation(baseFileName + QStringLiteral("factory.cpp"));
}

void CodeGenerator::generateExtensionClasses(const QString &baseFileName) const
{
    writeExtensionHeader(baseFileName + QStringLiteral(".h"));
    writeExtensionImplementation(baseFileName + QStringLiteral(".cpp"));
}

bool CodeGenerator::isLegacyVersion(Version v) const
{
    return (v.major < 3 || (v.major == 3 && v.minor == 0));
}

bool CodeGenerator::versionHasProfiles(Version v) const
{
    VersionProfile vp;
    vp.version = v;
    return vp.hasProfiles();
}

void CodeGenerator::writeCoreHelperClasses(const QString &fileName, ClassComponent component) const
{
    if (!m_parser)
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    BOBUIextStream stream(&file);

    // Write the preamble
    writePreamble(fileName, stream);

    // Iterate over each OpenGL version. For each version output a private class for
    // core functions and a private class for deprecated functions.
    const QString privateRootClass = QStringLiteral("QOpenGLVersionFunctionsBackend");
    Q_FOREACH (const VersionProfile &versionProfile, m_parser->versionProfiles()) {
        switch (component) {
        case Declaration:
            writeBackendClassDeclaration(stream, versionProfile, privateRootClass);
            break;

        case Definition:
            writeBackendClassImplementation(stream, versionProfile, privateRootClass);
            break;
        }
    }

    // Write the postamble
    writePostamble(fileName, stream);
}

void CodeGenerator::writeCoreClasses(const QString &baseFileName) const
{
    // Iterate over each OpenGL version. For each version output a public class (for legacy
    // versions or two public classes (for modern versions with profiles). Each public class
    // is given pointers to private classes containing the actual entry points. For example,
    // the class for OpenGL 1.1 will have pointers to the private classes for 1.0 core, 1.1
    // core, 1.0 deprecated and 1.1 deprecated. Whereas the class for OpenGL 3.2 Core profile
    // will have pointers to the private classes for 1.0 core, 1.1 core, ..., 3.2 core but
    // not to any of the deprecated private classes
    QList<ClassComponent> components = (QList<ClassComponent>() << Declaration << Definition);
    Q_FOREACH (const ClassComponent &component, components) {
        const QString rootClass = QStringLiteral("QAbstractOpenGLFunctions");
        Q_FOREACH (const Version &classVersion, m_parser->versions()) {
            VersionProfile v;
            v.version = classVersion;
            v.profile = VersionProfile::CompatibilityProfile;

            if (isLegacyVersion(classVersion)) {
                switch (component) {
                case Declaration:
                    writePublicClassDeclaration(baseFileName, v, rootClass);
                    break;

                case Definition:
                    writePublicClassImplementation(baseFileName, v, rootClass);
                    break;
                }
            } else {
                switch (component) {
                case Declaration:
                    writePublicClassDeclaration(baseFileName, v, rootClass);
                    v.profile = VersionProfile::CoreProfile;
                    writePublicClassDeclaration(baseFileName, v, rootClass);
                    break;

                case Definition:
                    writePublicClassImplementation(baseFileName, v, rootClass);
                    v.profile = VersionProfile::CoreProfile;
                    writePublicClassImplementation(baseFileName, v, rootClass);
                    break;
                }
            }
        }
    }
}

void CodeGenerator::writeCoreFactoryHeader(const QString &fileName) const
{
    if (!m_parser)
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    BOBUIextStream stream(&file);

    // Write the preamble
    writePreamble(fileName, stream);

    // Write the postamble
    writePostamble(fileName, stream);
}

void CodeGenerator::writeCoreFactoryImplementation(const QString &fileName) const
{
    if (!m_parser)
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    BOBUIextStream stream(&file);

    // Write the preamble
    writePreamble(fileName, stream);

    // Get the set of version functions classes we need to create
    QList<Version> versions = m_parser->versions();
    std::sort(versions.begin(), versions.end(), std::greater<Version>());

    // Outout the #include statements
    stream << QStringLiteral("#if !BOBUI_CONFIG(opengles2)") << BobUI::endl;
    Q_FOREACH (const Version &classVersion, versions) {
        if (!versionHasProfiles(classVersion)) {
            stream << QString(QStringLiteral("#include \"qopenglfunctions_%1_%2.h\""))
                      .arg(classVersion.major)
                      .arg(classVersion.minor) << BobUI::endl;
        } else {
            const QList<VersionProfile::OpenGLProfile> profiles = (QList<VersionProfile::OpenGLProfile>()
                << VersionProfile::CoreProfile << VersionProfile::CompatibilityProfile);

            Q_FOREACH (const VersionProfile::OpenGLProfile profile, profiles) {
                const QString profileSuffix = profile == VersionProfile::CoreProfile
                                            ? QStringLiteral("core")
                                            : QStringLiteral("compatibility");
                stream << QString(QStringLiteral("#include \"qopenglfunctions_%1_%2_%3.h\""))
                          .arg(classVersion.major)
                          .arg(classVersion.minor)
                          .arg(profileSuffix) << BobUI::endl;
            }
        }
    }
    stream << QStringLiteral("#else") << BobUI::endl;
    stream << QStringLiteral("#include \"qopenglfunctions_es2.h\"") << BobUI::endl;
    stream << QStringLiteral("#endif") << BobUI::endl;

    stream << BobUI::endl;

    stream << QStringLiteral("BOBUI_BEGIN_NAMESPACE") << BobUI::endl << BobUI::endl;
    stream << QStringLiteral("QAbstractOpenGLFunctions *QOpenGLVersionFunctionsFactory::create(const QOpenGLVersionProfile &versionProfile)") << BobUI::endl;
    stream << QStringLiteral("{") << BobUI::endl;
    stream << QStringLiteral("#if !BOBUI_CONFIG(opengles2)") << BobUI::endl;
    stream << QStringLiteral("    const int major = versionProfile.version().first;") << BobUI::endl;
    stream << QStringLiteral("    const int minor = versionProfile.version().second;") << BobUI::endl << BobUI::endl;

    // Iterate over classes with profiles
    stream << QStringLiteral("    if (versionProfile.hasProfiles()) {") << BobUI::endl;
    stream << QStringLiteral("        switch (versionProfile.profile()) {") << BobUI::endl;
    const QList<VersionProfile::OpenGLProfile> profiles = (QList<VersionProfile::OpenGLProfile>()
        << VersionProfile::CoreProfile << VersionProfile::CompatibilityProfile);
    Q_FOREACH (const VersionProfile::OpenGLProfile profile, profiles) {
        const QString caseLabel = profile == VersionProfile::CoreProfile
                                ? QStringLiteral("QSurfaceFormat::CoreProfile")
                                : QStringLiteral("QSurfaceFormat::CompatibilityProfile");
        stream << QString(QStringLiteral("        case %1:")).arg(caseLabel) << BobUI::endl;

        int i = 0;
        Q_FOREACH (const Version &classVersion, versions) {
            if (!versionHasProfiles(classVersion))
                continue;

            const QString ifString = (i++ == 0) ? QStringLiteral("if") : QStringLiteral("else if");
            stream << QString(QStringLiteral("            %1 (major == %2 && minor == %3)"))
                      .arg(ifString)
                      .arg(classVersion.major)
                      .arg(classVersion.minor) << BobUI::endl;

            VersionProfile v;
            v.version = classVersion;
            v.profile = profile;
            stream << QString(QStringLiteral("                return new %1;"))
                      .arg(generateClassName(v)) << BobUI::endl;
        }

        stream << QStringLiteral("            break;") << BobUI::endl << BobUI::endl;
    }

    stream << QStringLiteral("        case QSurfaceFormat::NoProfile:") << BobUI::endl;
    stream << QStringLiteral("        default:") << BobUI::endl;
    stream << QStringLiteral("            break;") << BobUI::endl;
    stream << QStringLiteral("        };") << BobUI::endl;
    stream << QStringLiteral("    } else {") << BobUI::endl;

    // Iterate over the legacy classes (no profiles)
    int i = 0;
    Q_FOREACH (const Version &classVersion, versions) {
        if (versionHasProfiles(classVersion))
            continue;

        const QString ifString = (i++ == 0) ? QStringLiteral("if") : QStringLiteral("else if");
        stream << QString(QStringLiteral("        %1 (major == %2 && minor == %3)"))
                  .arg(ifString)
                  .arg(classVersion.major)
                  .arg(classVersion.minor) << BobUI::endl;

        VersionProfile v;
        v.version = classVersion;
        stream << QString(QStringLiteral("            return new %1;"))
                  .arg(generateClassName(v)) << BobUI::endl;
    }

    stream << QStringLiteral("    }") << BobUI::endl;
    stream << QStringLiteral("    return 0;") << BobUI::endl;

    stream << QStringLiteral("#else") << BobUI::endl;
    stream << QStringLiteral("    Q_UNUSED(versionProfile);") << BobUI::endl;
    stream << QStringLiteral("    return new QOpenGLFunctions_ES2;") << BobUI::endl;
    stream << QStringLiteral("#endif") << BobUI::endl;
    stream << QStringLiteral("}") << BobUI::endl;

    // Write the postamble
    writePostamble(fileName, stream);
}

/**
  \returns all functions to be included in the class defined by \a classVersionProfile
 */
FunctionCollection CodeGenerator::functionCollection( const VersionProfile& classVersionProfile ) const
{
    const Version classVersion = classVersionProfile.version;
    FunctionCollection functionSet;
    QList<Version> versions = m_parser->versions();

    // Populate these based upon the class version and profile
    Version minVersion;
    minVersion.major = 1;
    minVersion.minor = 0;
    Version maxVersion = classVersion;
    QList<VersionProfile::OpenGLProfile> profiles;
    profiles << VersionProfile::CoreProfile; // Always need core functions

    if (isLegacyVersion(classVersion)
        || (classVersionProfile.hasProfiles()
         && classVersionProfile.profile == VersionProfile::CompatibilityProfile)) {
        // For versions < 3.1 and Compatibility profile we include both core and deprecated functions
        profiles << VersionProfile::CompatibilityProfile;
    }

    Q_FOREACH (const Version &v, versions) {
        // Only include functions from versions in the range
        if (v < minVersion)
            continue;
        if (v > maxVersion)
            break;

        Q_FOREACH (VersionProfile::OpenGLProfile profile, profiles) {
            // Combine version and profile for this subset of functions
            VersionProfile version;
            version.version = v;
            version.profile = profile;

            // Fetch the functions and add to collection for this class
            QList<Function> functions = m_parser->functionsForVersion(version);
            functionSet.insert(version, functions);
        }
    }

    return functionSet;
}

void CodeGenerator::writePreamble(const QString &baseFileName, BOBUIextStream &stream, const QString replacement) const
{
    const QString fileName = baseFileName + QStringLiteral(".header");
    if (!QFile::exists(fileName))
        return;

    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        BOBUIextStream preambleStream(&file);
        QString preamble = preambleStream.readAll();
        if (!replacement.isEmpty())
            preamble.replace(QStringLiteral("__VERSION__"), replacement, BobUI::CaseSensitive);
        stream << preamble;
    }
}

void CodeGenerator::writePostamble(const QString &baseFileName, BOBUIextStream &stream) const
{
    const QString fileName = baseFileName + QStringLiteral(".footer");
    if (!QFile::exists(fileName))
        return;

    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        BOBUIextStream postambleStream(&file);
        QString postamble = postambleStream.readAll();
        stream << postamble;
    }
}

QString CodeGenerator::passByType(const Argument &arg) const
{
    QString passBy;
    switch (arg.mode) {
    case Argument::Reference:
    case Argument::Array:
        passBy = QStringLiteral("*");
        break;

    default:
    case Argument::Value:
        passBy = QString();
    }
    return passBy;
}

QString CodeGenerator::safeArgumentName(const QString& arg) const
{
    if (arg == QLatin1String("near")) // MS Windows defines near and far
        return QStringLiteral("nearVal");
    else if (arg == QLatin1String("far"))
        return QStringLiteral("farVal");
    else if (arg == QLatin1String("d"))
        return QStringLiteral("dd"); // Don't shadow d pointer
    else
        return arg;
}

QString CodeGenerator::generateClassName(const VersionProfile &classVersion, ClassVisibility visibility) const
{
    QString className;
    switch ( visibility ) {
    case Public: {
        // Class name and base class
        QString profileSuffix;
        if (classVersion.hasProfiles())
            profileSuffix = (classVersion.profile == VersionProfile::CoreProfile ? QStringLiteral("_Core") : QStringLiteral("_Compatibility"));

        className = QString(QStringLiteral("QOpenGLFunctions_%1_%2%3"))
                    .arg(classVersion.version.major)
                    .arg(classVersion.version.minor)
                    .arg(profileSuffix);
        break;
    }
    case Private: {
        QString statusSuffix = (classVersion.profile == VersionProfile::CoreProfile ? QStringLiteral("_Core") : QStringLiteral("_Deprecated"));

        className = QString(QStringLiteral("QOpenGLFunctions_%1_%2%3Private"))
                    .arg(classVersion.version.major)
                    .arg(classVersion.version.minor)
                    .arg(statusSuffix);
        break;
        }
    }

    return className;
}

void CodeGenerator::writeBackendClassDeclaration(BOBUIextStream &stream,
                                                 const VersionProfile &versionProfile,
                                                 const QString &baseClass) const
{
    const QString className = backendClassName(versionProfile);
    stream << QString(QStringLiteral("class %1 : public %2"))
              .arg(className)
              .arg(baseClass)
           << BobUI::endl;
    stream << QStringLiteral("{") << BobUI::endl;
    stream << QStringLiteral("public:") << BobUI::endl;
    stream << QString( QStringLiteral("    %1(QOpenGLContext *context);") ).arg(className) << BobUI::endl << BobUI::endl;

    // Output function used for generating key used in QOpenGLContextPrivate
    stream << QStringLiteral("    static QOpenGLVersionStatus versionStatus();") << BobUI::endl << BobUI::endl;

    // Get the functions needed for this class
    FunctionList functions = m_parser->functionsForVersion(versionProfile);
    FunctionCollection functionSet;
    functionSet.insert(versionProfile, functions);

    // Declare the functions
    writeClassFunctionDeclarations(stream, functionSet, Private);

    stream << QStringLiteral("};") << BobUI::endl;
    stream << BobUI::endl;
}

void CodeGenerator::writeBackendClassImplementation(BOBUIextStream &stream,
                                                    const VersionProfile &versionProfile,
                                                    const QString &baseClass) const
{
    const QString className = backendClassName(versionProfile);
    stream << QString(QStringLiteral("%1::%1(QOpenGLContext *context)")).arg(className) << BobUI::endl;
    stream << QString(QStringLiteral("    : %1(context)")).arg(baseClass) << BobUI::endl
           << QStringLiteral("{") << BobUI::endl;

    // Resolve the entry points for this set of functions
    // Get the functions needed for this class
    FunctionList functions = m_parser->functionsForVersion(versionProfile);
    FunctionCollection functionSet;
    functionSet.insert(versionProfile, functions);
    writeEntryPointResolutionCode(stream, functionSet);

    stream << QStringLiteral("}") << BobUI::endl << BobUI::endl;

    stream << QString(QStringLiteral("QOpenGLVersionStatus %1::versionStatus()")).arg(className) << BobUI::endl;
    stream << QStringLiteral("{") << BobUI::endl;
    const QString status = versionProfile.profile == VersionProfile::CoreProfile
                         ? QStringLiteral("QOpenGLVersionStatus::CoreStatus")
                         : QStringLiteral("QOpenGLVersionStatus::DeprecatedStatus");
    stream << QString(QStringLiteral("    return QOpenGLVersionStatus(%1, %2, %3);"))
              .arg(versionProfile.version.major)
              .arg(versionProfile.version.minor)
              .arg(status) << BobUI::endl;
    stream << QStringLiteral("}") << BobUI::endl << BobUI::endl;
}

QString CodeGenerator::coreClassFileName(const VersionProfile &versionProfile,
                                         const QString& fileExtension) const
{
    QString profileSuffix;
    if (versionProfile.hasProfiles())
        profileSuffix = (versionProfile.profile == VersionProfile::CoreProfile ? QStringLiteral("_core") : QStringLiteral("_compatibility"));

    const QString fileName = QString(QStringLiteral("qopenglfunctions_%1_%2%3.%4"))
                             .arg(versionProfile.version.major)
                             .arg(versionProfile.version.minor)
                             .arg(profileSuffix)
                             .arg(fileExtension);
    return fileName;
}

void CodeGenerator::writePublicClassDeclaration(const QString &baseFileName,
                                                const VersionProfile &versionProfile,
                                                const QString &baseClass) const
{
    const QString fileName = coreClassFileName(versionProfile, QStringLiteral("h"));
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    BOBUIextStream stream(&file);

    // Write the preamble
    const QString templateFileName = QString(QStringLiteral("%1__VERSION__.h"))
                                     .arg(baseFileName);
    QString profileSuffix;
    if (versionProfile.hasProfiles())
        profileSuffix = (versionProfile.profile == VersionProfile::CoreProfile ? QStringLiteral("_CORE") : QStringLiteral("_COMPATIBILITY"));

    const QString versionProfileString = QString(QStringLiteral("_%1_%2%3"))
                                         .arg(versionProfile.version.major)
                                         .arg(versionProfile.version.minor)
                                         .arg(profileSuffix);
    writePreamble(templateFileName, stream, versionProfileString);

    // Ctor, dtor, and initialize function;
    const QString className = generateClassName(versionProfile, Public);
    stream << QString(QStringLiteral("class Q_GUI_EXPORT %1 : public %2"))
              .arg(className)
              .arg(baseClass)
           << BobUI::endl;
    stream << QStringLiteral("{") << BobUI::endl;
    stream << QStringLiteral("public:") << BobUI::endl;
    stream << QString(QStringLiteral("    %1();")).arg(className) << BobUI::endl;
    stream << QString(QStringLiteral("    ~%1();")).arg(className) << BobUI::endl << BobUI::endl;
    stream << QStringLiteral("    bool initializeOpenGLFunctions() override;") << BobUI::endl << BobUI::endl;

    // Get the functions needed for this class and declare them
    FunctionCollection functionSet = functionCollection(versionProfile);
    writeClassFunctionDeclarations(stream, functionSet, Public);

    // isCompatible function and backend variables
    stream << QStringLiteral("private:") << BobUI::endl;
    stream << QStringLiteral("    friend class QOpenGLContext;") << BobUI::endl << BobUI::endl;
    stream << QStringLiteral("    static bool isContextCompatible(QOpenGLContext *context);") << BobUI::endl;
    stream << QStringLiteral("    static QOpenGLVersionProfile versionProfile();") << BobUI::endl << BobUI::endl;
    writeBackendVariableDeclarations(stream, backendsForFunctionCollection(functionSet));

    stream << QStringLiteral("};") << BobUI::endl << BobUI::endl;

    // Output the inline functions that forward OpenGL calls to the backends' entry points
    writeClassInlineFunctions(stream, className, functionSet);

    // Write the postamble
    writePostamble(templateFileName, stream);
}

void CodeGenerator::writePublicClassImplementation(const QString &baseFileName,
                                                   const VersionProfile &versionProfile,
                                                   const QString& baseClass) const
{
    const QString fileName = coreClassFileName(versionProfile, QStringLiteral("cpp"));
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    BOBUIextStream stream(&file);

    // Write the preamble
    const QString templateFileName = QString(QStringLiteral("%1__VERSION__.cpp"))
                                     .arg(baseFileName);
    QString profileSuffix;
    if (versionProfile.hasProfiles())
        profileSuffix = (versionProfile.profile == VersionProfile::CoreProfile ? QStringLiteral("_core") : QStringLiteral("_compatibility"));

    const QString versionProfileString = QString(QStringLiteral("_%1_%2%3"))
                                         .arg(versionProfile.version.major)
                                         .arg(versionProfile.version.minor)
                                         .arg(profileSuffix);
    writePreamble(templateFileName, stream, versionProfileString);

    const QString className = generateClassName(versionProfile, Public);
    stream << QStringLiteral("/*!") << BobUI::endl
           << QStringLiteral("    \\class ") << className << BobUI::endl
           << QStringLiteral("    \\inmodule BobUIGui") << BobUI::endl
           << QStringLiteral("    \\since 5.1") << BobUI::endl
           << QStringLiteral("    \\wrapper") << BobUI::endl
           << QStringLiteral("    \\brief The ") << className
           << QString(QStringLiteral(" class provides all functions for OpenGL %1.%2 "))
                .arg(versionProfile.version.major)
                .arg(versionProfile.version.minor);

    if (!profileSuffix.isEmpty()) {
        profileSuffix.remove(0, 1);
        profileSuffix.append(QStringLiteral(" profile"));
    } else {
        profileSuffix = "specification";
    }

    stream << profileSuffix << QStringLiteral(".") << BobUI::endl << BobUI::endl
           << QStringLiteral("    This class is a wrapper for functions from ")
           << QString(QStringLiteral("OpenGL %1.%2 "))
                .arg(versionProfile.version.major)
                .arg(versionProfile.version.minor)
           << profileSuffix << QStringLiteral(".") << BobUI::endl
           << QStringLiteral("    See reference pages on \\l {http://www.opengl.org/sdk/docs/}{opengl.org}") << BobUI::endl
           << QStringLiteral("    for function documentation.") << BobUI::endl << BobUI::endl
           << QStringLiteral("    \\sa QAbstractOpenGLFunctions") << BobUI::endl
           << QStringLiteral("*/") << BobUI::endl << BobUI::endl;

    // Get the data we'll need for this class implementation
    FunctionCollection functionSet = functionCollection(versionProfile);
    QList<VersionProfile> backends = backendsForFunctionCollection(functionSet);

    // Output default constructor
    stream << className << QStringLiteral("::") << className << QStringLiteral("()") << BobUI::endl;
    stream << QStringLiteral(" : ") << baseClass << QStringLiteral("()");
    Q_FOREACH (const VersionProfile &v, backends)
        stream << BobUI::endl << QString(QStringLiteral(" , %1(0)")).arg(backendVariableName(v));
    stream << BobUI::endl << QStringLiteral("{") << BobUI::endl << QStringLiteral("}") << BobUI::endl << BobUI::endl;

    // Output the destructor
    stream << className << QStringLiteral("::~") << className << QStringLiteral("()") << BobUI::endl;
    stream << QStringLiteral("{") << BobUI::endl;
    Q_FOREACH (const VersionProfile &v, backends) {
        const QString backendVar = backendVariableName(v);
        const QString backendClass = backendClassName(v);
        stream << QString(QStringLiteral("    if (%1 && !%1->refs.deref()) {")).arg(backendVar) << BobUI::endl;
        stream << QString(QStringLiteral("        QAbstractOpenGLFunctionsPrivate::removeFunctionsBackend(%1->context, %2::versionStatus());"))
                  .arg(backendVar)
                  .arg(backendClass) << BobUI::endl;
        stream << QString(QStringLiteral("        delete %1;")).arg(backendVar) << BobUI::endl;
        stream << QStringLiteral("    }") << BobUI::endl;
    }
    stream << QStringLiteral("}") << BobUI::endl << BobUI::endl;

    // Output the initialize function that creates the backend objects
    stream << QString(QStringLiteral("bool %1::initializeOpenGLFunctions()")).arg(className) << BobUI::endl;
    stream << QStringLiteral("{") << BobUI::endl;

    stream << QStringLiteral("    if ( isInitialized() )") << BobUI::endl;
    stream << QStringLiteral("        return true;") << BobUI::endl << BobUI::endl;
    stream << QStringLiteral("    QOpenGLContext* context = QOpenGLContext::currentContext();") << BobUI::endl << BobUI::endl;
    stream << QStringLiteral("    // If owned by a context object make sure it is current.") << BobUI::endl;
    stream << QStringLiteral("    // Also check that current context is capable of resolving all needed functions") << BobUI::endl;
    stream << QStringLiteral("    if (((owningContext() && owningContext() == context) || !owningContext())") << BobUI::endl;
    stream << QString(QStringLiteral("        && %1::isContextCompatible(context))")).arg(className) << BobUI::endl;
    stream << QStringLiteral("    {") << BobUI::endl;
    stream << QStringLiteral("        // Associate with private implementation, creating if necessary") << BobUI::endl;
    stream << QStringLiteral("        // Function pointers in the backends are resolved at creation time") << BobUI::endl;
    stream << QStringLiteral("        QOpenGLVersionFunctionsBackend* d = 0;") << BobUI::endl;

    Q_FOREACH (const VersionProfile &v, backends) {
        const QString backendClass = backendClassName(v);
        const QString backendVar = backendVariableName(v);
        stream << QString(QStringLiteral("        d = QAbstractOpenGLFunctionsPrivate::functionsBackend(context, %1::versionStatus());"))
                  .arg(backendClass) << BobUI::endl;
        stream << QStringLiteral("        if (!d) {") << BobUI::endl;
        stream << QString(QStringLiteral("            d = new %1(context);")).arg(backendClass) << BobUI::endl;
        stream << QString(QStringLiteral("            QAbstractOpenGLFunctionsPrivate::insertFunctionsBackend(context, %1::versionStatus(), d);"))
                  .arg(backendClass) << BobUI::endl;
        stream << QStringLiteral("        }") << BobUI::endl;
        stream << QString(QStringLiteral("        %1 = static_cast<%2*>(d);")).arg(backendVar).arg(backendClass) << BobUI::endl;
        stream << QStringLiteral("        d->refs.ref();") << BobUI::endl << BobUI::endl;
    }

    stream << QStringLiteral("        QAbstractOpenGLFunctions::initializeOpenGLFunctions();") << BobUI::endl;
    stream << QStringLiteral("    }") << BobUI::endl;

    stream << QStringLiteral("    return isInitialized();") << BobUI::endl;
    stream << QStringLiteral("}") << BobUI::endl << BobUI::endl;

    // Output the context compatibility check function
    stream << QString(QStringLiteral("bool %1::isContextCompatible(QOpenGLContext *context)")).arg(className) << BobUI::endl;
    stream << QStringLiteral("{") << BobUI::endl;
    stream << QStringLiteral("    Q_ASSERT(context);") << BobUI::endl;
    stream << QStringLiteral("    QSurfaceFormat f = context->format();") << BobUI::endl;
    stream << QStringLiteral("    const auto v = std::pair(f.majorVersion(), f.minorVersion());") << BobUI::endl;
    stream << QString(QStringLiteral("    if (v < std::pair(%1, %2))"))
              .arg(versionProfile.version.major)
              .arg(versionProfile.version.minor) << BobUI::endl;
    stream << QStringLiteral("        return false;") << BobUI::endl << BobUI::endl;

    // If generating a legacy or compatibility profile class we need to ensure that
    // the context does not expose only core functions
    if (versionProfile.profile != VersionProfile::CoreProfile) {
        stream << QStringLiteral("    if (f.profile() == QSurfaceFormat::CoreProfile)") << BobUI::endl;
        stream << QStringLiteral("        return false;") << BobUI::endl << BobUI::endl;
    }

    stream << QStringLiteral("    return true;") << BobUI::endl;
    stream << QStringLiteral("}") << BobUI::endl << BobUI::endl;

    // Output static function used as helper in template versionFunctions() function
    // in QOpenGLContext
    stream << QString(QStringLiteral("QOpenGLVersionProfile %1::versionProfile()")).arg(className) << BobUI::endl;
    stream << QStringLiteral("{") << BobUI::endl;
    stream << QStringLiteral("    QOpenGLVersionProfile v;") << BobUI::endl;
    stream << QString(QStringLiteral("    v.setVersion(%1, %2);"))
              .arg(versionProfile.version.major)
              .arg(versionProfile.version.minor) << BobUI::endl;
    if (versionProfile.hasProfiles()) {
        const QString profileName = versionProfile.profile == VersionProfile::CoreProfile
                                    ? QStringLiteral("QSurfaceFormat::CoreProfile")
                                    : QStringLiteral("QSurfaceFormat::CompatibilityProfile");
        stream << QString(QStringLiteral("    v.setProfile(%1);")).arg(profileName) << BobUI::endl;
    }
    stream << QStringLiteral("    return v;") << BobUI::endl;
    stream << QStringLiteral("}") << BobUI::endl;

    // Write the postamble
    writePostamble(templateFileName, stream);
}

void CodeGenerator::writeClassFunctionDeclarations(BOBUIextStream &stream,
                                                   const FunctionCollection &functionSet,
                                                   ClassVisibility visibility) const
{
    Q_FOREACH (const VersionProfile &version, functionSet.keys()) {
        // Add a comment to the header
        stream << QString(QStringLiteral("    // OpenGL %1.%2 %3 functions"))
                  .arg(version.version.major)
                  .arg(version.version.minor)
                  .arg((version.profile == VersionProfile::CoreProfile) ? QStringLiteral("core") : QStringLiteral("deprecated"))
               << BobUI::endl;

        // Output function declarations
        FunctionList functions = functionSet.value(version);
        Q_FOREACH (const Function &f, functions)
            writeFunctionDeclaration(stream, f, visibility);
        stream << BobUI::endl;
    } // version and profile
}

void CodeGenerator::writeFunctionDeclaration(BOBUIextStream &stream, const Function &f, ClassVisibility visibility) const
{
    QStringList argList;
    Q_FOREACH (const Argument &arg, f.arguments) {
        QString a = QString(QStringLiteral("%1%2 %3%4"))
                    .arg((arg.direction == Argument::In && arg.mode != Argument::Value) ? QStringLiteral("const ") : QString())
                    .arg(arg.type)
                    .arg(passByType(arg))
                    .arg(safeArgumentName(arg.name));
        argList.append(a);
    }
    QString args = argList.join(QStringLiteral(", "));

    QString signature;
    switch (visibility) {
    case Public:
        signature = QString(QStringLiteral("    %1 gl%2(%3);")).arg(f.returnType).arg(f.name).arg(args);
        break;

    case Private:
    default:
        signature = QString(QStringLiteral("    %1 (QOPENGLF_APIENTRYP %2)(%3);")).arg(f.returnType).arg(f.name).arg(args);
    }
    stream << signature << BobUI::endl;
}

void CodeGenerator::writeClassInlineFunctions(BOBUIextStream &stream,
                                              const QString &className,
                                              const FunctionCollection &functionSet) const
{
    Q_FOREACH (const VersionProfile &version, functionSet.keys()) {

        // Add a comment to the header
        stream << QString(QStringLiteral("// OpenGL %1.%2 %3 functions"))
                  .arg(version.version.major)
                  .arg(version.version.minor)
                  .arg((version.profile == VersionProfile::CoreProfile) ? QStringLiteral("core") : QStringLiteral("deprecated"))
               << BobUI::endl;

        // Output function declarations
        const QString backendVar = backendVariableName(version);
        FunctionList functions = functionSet.value(version);
        Q_FOREACH (const Function &f, functions)
            writeInlineFunction(stream, className, backendVar, f);

        stream << BobUI::endl;

    } // version and profile
}

void CodeGenerator::writeInlineFunction(BOBUIextStream &stream, const QString &className,
                                        const QString &backendVar, const Function &f) const
{
    QStringList argList;
    Q_FOREACH (const Argument &arg, f.arguments) {
        QString a = QString(QStringLiteral("%1%2 %3%4"))
                    .arg((arg.direction == Argument::In && arg.mode != Argument::Value) ? QStringLiteral("const ") : QString())
                    .arg(arg.type)
                    .arg(passByType(arg))
                    .arg(safeArgumentName(arg.name));
        argList.append(a);
    }
    QString args = argList.join(", ");


    QString signature = QString(QStringLiteral("inline %1 %2::gl%3(%4)"))
                        .arg(f.returnType)
                        .arg(className)
                        .arg(f.name)
                        .arg(args);
    stream << signature << BobUI::endl << QStringLiteral("{") << BobUI::endl;

    QStringList argumentNames;
    Q_FOREACH (const Argument &arg, f.arguments)
        argumentNames.append(safeArgumentName(arg.name));
    QString argNames = argumentNames.join(", ");

    if (f.returnType == QLatin1String("void"))
        stream << QString(QStringLiteral("    %1->%2(%3);")).arg(backendVar).arg(f.name).arg(argNames) << BobUI::endl;
    else
        stream << QString(QStringLiteral("    return %1->%2(%3);")).arg(backendVar).arg(f.name).arg(argNames) << BobUI::endl;
    stream << QStringLiteral("}") << BobUI::endl << BobUI::endl;
}

void CodeGenerator::writeEntryPointResolutionCode(BOBUIextStream &stream,
                                                  const FunctionCollection &functionSet) const
{
    bool hasModuleHandle = false;
    Q_FOREACH (const VersionProfile &version, functionSet.keys()) {

        // Add a comment to the header
        stream << QString(QStringLiteral("    // OpenGL %1.%2 %3 functions"))
                  .arg(version.version.major)
                  .arg(version.version.minor)
                  .arg((version.profile == VersionProfile::CoreProfile) ? QStringLiteral("core") : QStringLiteral("deprecated"))
               << BobUI::endl;

        // Output function declarations
        FunctionList functions = functionSet.value(version);

        bool useGetProcAddress = (version.version.major == 1 && (version.version.minor == 0 || version.version.minor == 1));
        if (useGetProcAddress) {
            stream << "#if defined(Q_OS_WIN)" << BobUI::endl;
            if (!hasModuleHandle) {
                stream << "    HMODULE handle = GetModuleHandleA(\"opengl32.dll\");" << BobUI::endl;
                hasModuleHandle = true;
            }

            Q_FOREACH (const Function &f, functions)
                writeEntryPointResolutionStatement(stream, f, QString(), useGetProcAddress);

            stream << "#else" << BobUI::endl;
        }

        Q_FOREACH (const Function &f, functions)
            writeEntryPointResolutionStatement(stream, f);

        if (useGetProcAddress)
            stream << "#endif" << BobUI::endl;

        stream << BobUI::endl;

    } // version and profile
}

void CodeGenerator::writeEntryPointResolutionStatement(BOBUIextStream &stream, const Function &f,
                                                       const QString &prefix, bool useGetProcAddress) const
{
    QStringList argList;
    Q_FOREACH (const Argument &arg, f.arguments) {
        QString a = QString("%1%2 %3")
                    .arg((arg.direction == Argument::In && arg.mode != Argument::Value) ? QStringLiteral("const ") : QString())
                    .arg(arg.type)
                    .arg(passByType(arg));
        argList.append(a);
    }
    QString args = argList.join(QStringLiteral(", "));

    QString signature;
    if (!useGetProcAddress) {
        signature = QString(QStringLiteral("    %4%3 = reinterpret_cast<%1 (QOPENGLF_APIENTRYP)(%2)>(context->getProcAddress(\"gl%3\"));"))
                    .arg(f.returnType)
                    .arg(args)
                    .arg(f.name)
                    .arg(prefix);
    } else {
        signature = QString(QStringLiteral("    %4%3 = reinterpret_cast<%1 (QOPENGLF_APIENTRYP)(%2)>(GetProcAddress(handle, \"gl%3\"));"))
                    .arg(f.returnType)
                    .arg(args)
                    .arg(f.name)
                    .arg(prefix);
    }
    stream << signature << BobUI::endl;
}

QList<VersionProfile> CodeGenerator::backendsForFunctionCollection(const FunctionCollection &functionSet) const
{
    QList<VersionProfile> backends;
    Q_FOREACH (const VersionProfile &versionProfile, functionSet.keys()) {
        if (m_parser->versionProfiles().contains(versionProfile))
            backends.append(versionProfile);
    }
    return backends;
}

QString CodeGenerator::backendClassName(const VersionProfile &v) const
{
    QString statusSuffix = v.profile == VersionProfile::CoreProfile
                         ? QStringLiteral("_Core")
                         : QStringLiteral("_Deprecated");
    const QString className = QString(QStringLiteral("QOpenGLFunctions_%1_%2%3Backend"))
                              .arg(v.version.major)
                              .arg(v.version.minor)
                              .arg(statusSuffix);
    return className;
}

QString CodeGenerator::backendVariableName(const VersionProfile &v) const
{
    const QString status = (v.profile == VersionProfile::CoreProfile)
                         ? QStringLiteral("Core")
                         : QStringLiteral("Deprecated");
    const QString varName = QString(QStringLiteral("d_%1_%2_%3"))
                            .arg(v.version.major)
                            .arg(v.version.minor)
                            .arg(status);
    return varName;
}

void CodeGenerator::writeBackendVariableDeclarations(BOBUIextStream &stream, const QList<VersionProfile> &backends) const
{
    // We need a private class for each version and profile (status: core or deprecated)
    Q_FOREACH (const VersionProfile &v, backends) {
        const QString className = backendClassName(v);
        const QString varName = backendVariableName(v);
        stream << QString(QStringLiteral("    %1* %2;")).arg(className).arg(varName) << BobUI::endl;
    }
}

void CodeGenerator::writeExtensionHeader(const QString &fileName) const
{
    if (!m_parser)
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    BOBUIextStream stream(&file);

    // Write the preamble
    writePreamble(fileName, stream);

    // Iterate through the list of extensions and create one class per extension
    QStringList extensions = m_parser->extensions();
    Q_FOREACH (const QString &extension, extensions) {
        writeExtensionClassDeclaration(stream, extension, Private);
        writeExtensionClassDeclaration(stream, extension, Public);
    }

    // Write the postamble
    writePostamble(fileName, stream);
}

void CodeGenerator::writeExtensionImplementation(const QString &fileName) const
{
    if (!m_parser)
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    BOBUIextStream stream(&file);

    // Write the preamble
    writePreamble(fileName, stream);

    // Iterate through the list of extensions and create one class per extension
    QStringList extensions = m_parser->extensions();
    Q_FOREACH (const QString &extension, extensions)
        writeExtensionClassImplementation(stream, extension);

    // Write the postamble
    writePostamble(fileName, stream);
}

void CodeGenerator::writeExtensionClassDeclaration(BOBUIextStream &stream, const QString &extension, ClassVisibility visibility) const
{
    const QString className = generateExtensionClassName(extension, visibility);

    QString baseClass = (visibility == Public) ? QStringLiteral("QAbstractOpenGLExtension") : QStringLiteral("QAbstractOpenGLExtensionPrivate");

    stream << QString(QStringLiteral("class %2 : public %3"))
              .arg(className)
              .arg(baseClass)
           << BobUI::endl << "{" << BobUI::endl << "public:" << BobUI::endl;

    if (visibility == Public) {
        // Default constructor
        stream << QStringLiteral("    ") << className << QStringLiteral("();") << BobUI::endl << BobUI::endl;

        // Base class virtual function(s)
        QString resolveFunction = QStringLiteral("    bool initializeOpenGLFunctions() final;");
        stream << resolveFunction << BobUI::endl << BobUI::endl;
    }

    // Output the functions provided by this extension
    QList<Function> functions = m_parser->functionsForExtension(extension);
    Q_FOREACH (const Function &f, functions)
        writeFunctionDeclaration(stream, f, visibility);

    if (visibility == Public) {
        // Write out the protected ctor
        stream << BobUI::endl << QStringLiteral("protected:") << BobUI::endl;
        stream << QStringLiteral("    Q_DECLARE_PRIVATE(") << className << QStringLiteral(")") << BobUI::endl;
    }

    // End the class declaration
    stream << QStringLiteral("};") << BobUI::endl << BobUI::endl;

    // Output the inline functions for public class
    if (visibility == Public) {
        Q_FOREACH (const Function &f, functions)
            writeExtensionInlineFunction(stream, className, f);
    }
}

void CodeGenerator::writeExtensionInlineFunction(BOBUIextStream &stream, const QString &className, const Function &f) const
{
    QStringList argList;
    Q_FOREACH (const Argument &arg, f.arguments) {
        QString a = QString(QStringLiteral("%1%2 %3%4"))
                    .arg((arg.direction == Argument::In && arg.mode != Argument::Value) ? QStringLiteral("const ") : QString())
                    .arg(arg.type)
                    .arg(passByType(arg))
                    .arg(safeArgumentName(arg.name));
        argList.append(a);
    }
    QString args = argList.join(", ");


    QString signature = QString(QStringLiteral("inline %1 %2::gl%3(%4)"))
                        .arg(f.returnType)
                        .arg(className)
                        .arg(f.name)
                        .arg(args);
    stream << signature << BobUI::endl << QStringLiteral("{") << BobUI::endl;

    stream << QString(QStringLiteral("    Q_D(%1);")).arg(className) << BobUI::endl;

    QStringList argumentNames;
    Q_FOREACH (const Argument &arg, f.arguments)
        argumentNames.append(safeArgumentName(arg.name));
    QString argNames = argumentNames.join(", ");

    if (f.returnType == QStringLiteral("void"))
        stream << QString(QStringLiteral("    d->%1(%2);")).arg(f.name).arg(argNames) << BobUI::endl;
    else
        stream << QString(QStringLiteral("    return d->%1(%2);")).arg(f.name).arg(argNames) << BobUI::endl;
    stream << QStringLiteral("}") << BobUI::endl << BobUI::endl;
}

void CodeGenerator::writeExtensionClassImplementation(BOBUIextStream &stream, const QString &extension) const
{
    const QString className = generateExtensionClassName(extension);
    const QString privateClassName = generateExtensionClassName(extension, Private);

    // Output default constructor
    stream << className << QStringLiteral("::") << className << QStringLiteral("()") << BobUI::endl;
    stream << QStringLiteral(" : QAbstractOpenGLExtension(*(new ") << privateClassName << QStringLiteral("))") << BobUI::endl;
    stream << QStringLiteral("{") << BobUI::endl << QStringLiteral("}") << BobUI::endl << BobUI::endl;


    // Output function to initialize this class
    stream << QStringLiteral("bool ") << className
           << QStringLiteral("::initializeOpenGLFunctions()") << BobUI::endl
           << QStringLiteral("{") << BobUI::endl;

    stream << QStringLiteral("    if (isInitialized())") << BobUI::endl;
    stream << QStringLiteral("        return true;") << BobUI::endl << BobUI::endl;

    stream << QStringLiteral("    QOpenGLContext *context = QOpenGLContext::currentContext();") << BobUI::endl;
    stream << QStringLiteral("    if (!context) {") << BobUI::endl;
    stream << QStringLiteral("        qWarning(\"A current OpenGL context is required to resolve OpenGL extension functions\");")
           << BobUI::endl;
    stream << QStringLiteral("        return false;") << BobUI::endl;
    stream << QStringLiteral("    }") << BobUI::endl << BobUI::endl;

    // Output code to resolve entry points for this class
    stream << QStringLiteral("    // Resolve the functions") << BobUI::endl;
    stream << QStringLiteral("    Q_D(") << className << QStringLiteral(");") << BobUI::endl;
    stream << BobUI::endl;

    // Output function declarations
    QList<Function> functions = m_parser->functionsForExtension(extension);
    Q_FOREACH (const Function &f, functions)
        writeEntryPointResolutionStatement(stream, f, QStringLiteral("d->"));

    // Call the base class implementation
    stream << QStringLiteral("    QAbstractOpenGLExtension::initializeOpenGLFunctions();") << BobUI::endl;

    // Finish off
    stream << QStringLiteral("    return true;") << BobUI::endl;
    stream << QStringLiteral("}") << BobUI::endl << BobUI::endl;
}

QString CodeGenerator::generateExtensionClassName(const QString &extension, ClassVisibility visibility) const
{
    QString visibilitySuffix;
    if (visibility == Private)
        visibilitySuffix = QStringLiteral("Private");

    return QString(QStringLiteral("QOpenGLExtension_%1%2"))
            .arg(extension)
            .arg(visibilitySuffix);
}
