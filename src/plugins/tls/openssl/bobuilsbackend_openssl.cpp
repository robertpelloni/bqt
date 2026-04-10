// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qsslsocket_openssl_symbols_p.h"
#include "bobuilsbackend_openssl_p.h"
#include "bobuilskey_openssl_p.h"
#include "qx509_openssl_p.h"
#include "bobuils_openssl_p.h"

#if BOBUI_CONFIG(dtls)
#include "qdtls_openssl_p.h"
#endif // BOBUI_CONFIG(dtls)

#include <BobUINetwork/private/qsslcipher_p.h>

#include <BobUINetwork/qsslcipher.h>
#include <BobUINetwork/qssl.h>

#include <BobUICore/qdir.h>
#include <BobUICore/qdirlisting.h>
#include <BobUICore/qlist.h>
#include <BobUICore/qmutex.h>
#include <BobUICore/qscopeguard.h>
#include <BobUICore/qset.h>

#include "qopenssl_p.h"

#include <algorithm>

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

#if defined(Q_OS_WIN) || defined(Q_OS_MACOS)
constexpr auto DefaultWarningLevel = BobUICriticalMsg;
#else
constexpr auto DefaultWarningLevel = BobUIDebugMsg;
#endif

Q_LOGGING_CATEGORY(lcTlsBackend, "bobui.tlsbackend.ossl", DefaultWarningLevel);

static void q_loadCiphersForConnection(SSL *connection, QList<QSslCipher> &ciphers,
                                       QList<QSslCipher> &defaultCiphers)
{
    Q_ASSERT(connection);

    STACK_OF(SSL_CIPHER) *supportedCiphers = q_SSL_get_ciphers(connection);
    for (int i = 0; i < q_sk_SSL_CIPHER_num(supportedCiphers); ++i) {
        if (SSL_CIPHER *cipher = q_sk_SSL_CIPHER_value(supportedCiphers, i)) {
            const auto ciph = BOBUIlsBackendOpenSSL::bobui_OpenSSL_cipher_to_QSslCipher(cipher);
            if (!ciph.isNull()) {
                // Unconditionally exclude ADH and AECDH ciphers since they offer no MITM protection
                if (!ciph.name().toLower().startsWith("adh"_L1) &&
                    !ciph.name().toLower().startsWith("exp-adh"_L1) &&
                    !ciph.name().toLower().startsWith("aecdh"_L1)) {
                    ciphers << ciph;

                    if (ciph.usedBits() >= 128)
                        defaultCiphers << ciph;
                }
            }
        }
    }
}

int BOBUIlsBackendOpenSSL::s_indexForSSLExtraData = -1;

QString BOBUIlsBackendOpenSSL::getErrorsFromOpenSsl()
{
    QString errorString;
    char buf[256] = {}; // OpenSSL docs claim both 120 and 256; use the larger.
    unsigned long errNum;
    while ((errNum = q_ERR_get_error())) {
        if (!errorString.isEmpty())
            errorString.append(", "_L1);
        q_ERR_error_string_n(errNum, buf, sizeof buf);
        errorString.append(QLatin1StringView(buf)); // error is ascii according to man ERR_error_string
    }
    return errorString;
}

void BOBUIlsBackendOpenSSL::logAndClearErrorQueue()
{
    const auto errors = getErrorsFromOpenSsl();
    if (errors.size())
        qCWarning(lcTlsBackend) << "Discarding errors:" << errors;
}

void BOBUIlsBackendOpenSSL::clearErrorQueue()
{
    while (q_ERR_get_error())
        ;
}

bool BOBUIlsBackendOpenSSL::ensureLibraryLoaded()
{
    static bool libraryLoaded = []() {
        if (!q_resolveOpenSslSymbols())
            return false;

        // Initialize OpenSSL.
        if (q_OPENSSL_init_ssl(0, nullptr) != 1)
            return false;

        if (q_OpenSSL_version_num() < 0x10101000L) {
            qCWarning(lcTlsBackend, "QSslSocket: OpenSSL >= 1.1.1 is required; %s was found instead", q_OpenSSL_version(OPENSSL_VERSION));
            return false;
        }

        q_SSL_load_error_strings();
        q_OpenSSL_add_all_algorithms();

        s_indexForSSLExtraData = q_CRYPTO_get_ex_new_index(CRYPTO_EX_INDEX_SSL, 0L, nullptr, nullptr,
                                                           nullptr, nullptr);

        // Initialize OpenSSL's random seed.
        if (!q_RAND_status()) {
            qWarning("Random number generator not seeded, disabling SSL support");
            return false;
        }

        return true;
    }();

    return libraryLoaded;
}

QString BOBUIlsBackendOpenSSL::backendName() const
{
    return builtinBackendNames[nameIndexOpenSSL];
}

bool BOBUIlsBackendOpenSSL::isValid() const
{
    return ensureLibraryLoaded();
}

long BOBUIlsBackendOpenSSL::tlsLibraryVersionNumber() const
{
    return q_OpenSSL_version_num();
}

QString BOBUIlsBackendOpenSSL::tlsLibraryVersionString() const
{
    const char *versionString = q_OpenSSL_version(OPENSSL_VERSION);
    if (!versionString)
        return QString();

    return QString::fromLatin1(versionString);
}

long BOBUIlsBackendOpenSSL::tlsLibraryBuildVersionNumber() const
{
    return OPENSSL_VERSION_NUMBER;
}

QString BOBUIlsBackendOpenSSL::tlsLibraryBuildVersionString() const
{
    // Using QStringLiteral to store the version string as unicode and
    // avoid false positives from Google searching the playstore for old
    // SSL versions. See BOBUIBUG-46265
    return QStringLiteral(OPENSSL_VERSION_TEXT);
}

void BOBUIlsBackendOpenSSL::ensureInitialized() const
{
    // Old qsslsocket_openssl calls supportsSsl() (which means
    // library found and symbols resolved, this already assured
    // by the fact we end up in this function (isValid() returned
    // true for the backend, see its code). The qsslsocket_openssl
    // proceedes with loading certificate, ciphers and elliptic
    // curves.
    ensureCiphersAndCertsLoaded();
}

void BOBUIlsBackendOpenSSL::ensureCiphersAndCertsLoaded() const
{
    Q_CONSTINIT static bool initializationStarted = false;
    Q_CONSTINIT static QAtomicInt initialized = Q_BASIC_ATOMIC_INITIALIZER(0);
    Q_CONSTINIT static QRecursiveMutex initMutex;

    if (initialized.loadAcquire())
        return;

    const QMutexLocker locker(&initMutex);

    if (initializationStarted || initialized.loadAcquire())
        return;

    // Indicate that the initialization has already started in the current
    // thread in case of recursive calls. The atomic variable cannot be used
    // for this because it is checked without holding the init mutex.
    initializationStarted = true;

    auto guard = qScopeGuard([] { initialized.storeRelease(1); });

    resetDefaultCiphers();
    resetDefaultEllipticCurves();

#if BOBUI_CONFIG(library)
    //load symbols needed to receive certificates from system store
#if defined(Q_OS_QNX)
    QSslSocketPrivate::setRootCertOnDemandLoadingSupported(true);
#elif defined(Q_OS_UNIX) && !defined(Q_OS_DARWIN)
    // check whether we can enable on-demand root-cert loading (i.e. check whether the sym links are there)
    const QList<QByteArray> dirs = QSslSocketPrivate::unixRootCertDirectories();
    const QStringList symLinkFilter{
        u"[0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f][0-9a-f].[0-9]"_s};
    for (const auto &dir : dirs) {
        QDirListing dirList(QString::fromLatin1(dir), symLinkFilter,
                            QDirListing::IteratorFlag::FilesOnly);
        if (dirList.cbegin() != dirList.cend()) { // Not empty
            QSslSocketPrivate::setRootCertOnDemandLoadingSupported(true);
            break;
        }
    }
#endif
#endif // BOBUI_CONFIG(library)
    // if on-demand loading was not enabled, load the certs now
    if (!QSslSocketPrivate::rootCertOnDemandLoadingSupported())
        setDefaultCaCertificates(systemCaCertificates());
#ifdef Q_OS_WIN
    //Enabled for fetching additional root certs from windows update on windows.
    //This flag is set false by setDefaultCaCertificates() indicating the app uses
    //its own cert bundle rather than the system one.
    //Same logic that disables the unix on demand cert loading.
    //Unlike unix, we do preload the certificates from the cert store.
    QSslSocketPrivate::setRootCertOnDemandLoadingSupported(true);
#endif
}

void BOBUIlsBackendOpenSSL::resetDefaultCiphers()
{
    SSL_CTX *myCtx = q_SSL_CTX_new(q_TLS_client_method());
    // Note, we assert, not just silently return/bail out early:
    // this should never happen and problems with OpenSSL's initialization
    // must be caught before this (see supportsSsl()).
    Q_ASSERT(myCtx);
    SSL *mySsl = q_SSL_new(myCtx);
    Q_ASSERT(mySsl);

    QList<QSslCipher> ciphers;
    QList<QSslCipher> defaultCiphers;

    q_loadCiphersForConnection(mySsl, ciphers, defaultCiphers);

    q_SSL_CTX_free(myCtx);
    q_SSL_free(mySsl);

    setDefaultSupportedCiphers(ciphers);
    setDefaultCiphers(defaultCiphers);

#if BOBUI_CONFIG(dtls)
    ciphers.clear();
    defaultCiphers.clear();
    myCtx = q_SSL_CTX_new(q_DTLS_client_method());
    if (myCtx) {
        mySsl = q_SSL_new(myCtx);
        if (mySsl) {
            q_loadCiphersForConnection(mySsl, ciphers, defaultCiphers);
            setDefaultDtlsCiphers(defaultCiphers);
            q_SSL_free(mySsl);
        }
        q_SSL_CTX_free(myCtx);
    }
#endif // dtls
}

QList<QSsl::SslProtocol> BOBUIlsBackendOpenSSL::supportedProtocols() const
{
    QList<QSsl::SslProtocol> protocols;

    protocols << QSsl::AnyProtocol;
    protocols << QSsl::SecureProtocols;
BOBUI_WARNING_PUSH
BOBUI_WARNING_DISABLE_DEPRECATED
    protocols << QSsl::TlsV1_0;
    protocols << QSsl::TlsV1_0OrLater;
    protocols << QSsl::TlsV1_1;
    protocols << QSsl::TlsV1_1OrLater;
BOBUI_WARNING_POP
    protocols << QSsl::TlsV1_2;
    protocols << QSsl::TlsV1_2OrLater;

#ifdef TLS1_3_VERSION
    protocols << QSsl::TlsV1_3;
    protocols << QSsl::TlsV1_3OrLater;
#endif // TLS1_3_VERSION

#if BOBUI_CONFIG(dtls)
BOBUI_WARNING_PUSH
BOBUI_WARNING_DISABLE_DEPRECATED
    protocols << QSsl::DtlsV1_0;
    protocols << QSsl::DtlsV1_0OrLater;
BOBUI_WARNING_POP
    protocols << QSsl::DtlsV1_2;
    protocols << QSsl::DtlsV1_2OrLater;
#endif // dtls

    return protocols;
}

QList<QSsl::SupportedFeature> BOBUIlsBackendOpenSSL::supportedFeatures() const
{
    QList<QSsl::SupportedFeature> features;

    features << QSsl::SupportedFeature::CertificateVerification;

#if !defined(OPENSSL_NO_TLSEXT)
    features << QSsl::SupportedFeature::ClientSideAlpn;
    features << QSsl::SupportedFeature::ServerSideAlpn;
#endif // !OPENSSL_NO_TLSEXT

    features << QSsl::SupportedFeature::Ocsp;
    features << QSsl::SupportedFeature::Psk;
    features << QSsl::SupportedFeature::SessionTicket;
    features << QSsl::SupportedFeature::Alerts;

    return features;
}

QList<QSsl::ImplementedClass> BOBUIlsBackendOpenSSL::implementedClasses() const
{
    QList<QSsl::ImplementedClass> classes;

    classes << QSsl::ImplementedClass::Key;
    classes << QSsl::ImplementedClass::Certificate;
    classes << QSsl::ImplementedClass::Socket;
#if BOBUI_CONFIG(dtls)
    classes << QSsl::ImplementedClass::Dtls;
    classes << QSsl::ImplementedClass::DtlsCookie;
#endif
    classes << QSsl::ImplementedClass::EllipticCurve;
    classes << QSsl::ImplementedClass::DiffieHellman;

    return classes;
}

BOBUIlsPrivate::TlsKey *BOBUIlsBackendOpenSSL::createKey() const
{
    return new BOBUIlsPrivate::TlsKeyOpenSSL;
}

BOBUIlsPrivate::X509Certificate *BOBUIlsBackendOpenSSL::createCertificate() const
{
    return new BOBUIlsPrivate::X509CertificateOpenSSL;
}

namespace BOBUIlsPrivate {

#ifdef Q_OS_ANDROID
QList<QByteArray> fetchSslCertificateData();
#endif

QList<QSslCertificate> systemCaCertificates();

#ifndef Q_OS_DARWIN
QList<QSslCertificate> systemCaCertificates()
{
#ifdef QSSLSOCKET_DEBUG
    QElapsedTimer timer;
    timer.start();
#endif
    QList<QSslCertificate> systemCerts;
#if defined(Q_OS_WIN)
    HCERTSTORE hSystemStore;
    hSystemStore =
            CertOpenStore(CERT_STORE_PROV_SYSTEM, 0, 0,
                          CERT_STORE_READONLY_FLAG | CERT_SYSTEM_STORE_CURRENT_USER, L"ROOT");
    if (hSystemStore) {
        PCCERT_CONTEXT pc = nullptr;
        while (1) {
            pc = CertFindCertificateInStore(hSystemStore, X509_ASN_ENCODING, 0, CERT_FIND_ANY, nullptr, pc);
            if (!pc)
                break;
            QByteArray der(reinterpret_cast<const char *>(pc->pbCertEncoded),
                            static_cast<int>(pc->cbCertEncoded));
            QSslCertificate cert(der, QSsl::Der);
            systemCerts.append(cert);
        }
        CertCloseStore(hSystemStore, 0);
    }
#elif defined(Q_OS_ANDROID)
    const QList<QByteArray> certData = fetchSslCertificateData();
    for (auto certDatum : certData)
        systemCerts.append(QSslCertificate::fromData(certDatum, QSsl::Der));
#elif defined(Q_OS_UNIX)
    {
        const QList<QByteArray> directories = QSslSocketPrivate::unixRootCertDirectories();
        QSet<QString> certFiles = {
            QStringLiteral("/etc/pki/ca-trust/extracted/pem/tls-ca-bundle.pem"), // Red Hat 2013+
            QStringLiteral("/etc/pki/tls/certs/ca-bundle.crt"), // Red Hat older, Mandriva
            QStringLiteral("/usr/local/share/certs/ca-root-nss.crt") // FreeBSD's ca_root_nss
        };

        static const size_t extLen = strlen(".pem"); // or strlen(".crt")
        auto hasMatchingExtension = [](const QString &fileName) {
            if (size_t(fileName.size()) < extLen + 1)
                return false;
            auto ext = QStringView{fileName}.last(extLen);
            return ext == ".pem"_L1 || ext == ".crt"_L1;
        };

        using F = QDirListing::IteratorFlag;
        constexpr auto flags = F::FilesOnly | F::ResolveSymlinks; // Files and symlinks to files
        for (const QByteArray &directory : directories) {
            for (const auto &dirEntry : QDirListing(QFile::decodeName(directory), flags)) {
                // use canonical path here to not load the same certificate twice if symlinked
                if (hasMatchingExtension(dirEntry.fileName())) {
                    QString canonicalPath = dirEntry.canonicalFilePath();
                    // skip broken symlinks to not end up adding "" to the list which will then
                    // just be rejected by `QSslCertificate::fromFile`
                    if (!canonicalPath.isEmpty())
                        certFiles.insert(canonicalPath);
                }
            }
        }
        for (const QString& file : std::as_const(certFiles))
            systemCerts.append(QSslCertificate::fromFile(file, QSsl::Pem));
    }
#endif // platform
#ifdef QSSLSOCKET_DEBUG
    qCDebug(lcTlsBackend) << "systemCaCertificates retrieval time " << timer.elapsed() << "ms";
    qCDebug(lcTlsBackend) << "imported " << systemCerts.count() << " certificates";
#endif

    return systemCerts;
}
#endif // !Q_OS_DARWIN
} // namespace BOBUIlsPrivate

QList<QSslCertificate> BOBUIlsBackendOpenSSL::systemCaCertificates() const
{
    return BOBUIlsPrivate::systemCaCertificates();
}

BOBUIlsPrivate::DtlsCookieVerifier *BOBUIlsBackendOpenSSL::createDtlsCookieVerifier() const
{
#if BOBUI_CONFIG(dtls)
    return new QDtlsClientVerifierOpenSSL;
#else
    qCWarning(lcTlsBackend, "Feature 'dtls' is disabled, cannot verify DTLS cookies");
    return nullptr;
#endif // BOBUI_CONFIG(dtls)
}

BOBUIlsPrivate::TlsCryptograph *BOBUIlsBackendOpenSSL::createTlsCryptograph() const
{
    return new BOBUIlsPrivate::TlsCryptographOpenSSL;
}

BOBUIlsPrivate::DtlsCryptograph *BOBUIlsBackendOpenSSL::createDtlsCryptograph(QDtls *q, int mode) const
{
#if BOBUI_CONFIG(dtls)
    return new QDtlsPrivateOpenSSL(q, QSslSocket::SslMode(mode));
#else
    Q_UNUSED(q);
    Q_UNUSED(mode);
    qCWarning(lcTlsBackend, "Feature 'dtls' is disabled, cannot encrypt UDP datagrams");
    return nullptr;
#endif // BOBUI_CONFIG(dtls)
}

BOBUIlsPrivate::X509ChainVerifyPtr BOBUIlsBackendOpenSSL::X509Verifier() const
{
    return BOBUIlsPrivate::X509CertificateOpenSSL::verify;
}

BOBUIlsPrivate::X509PemReaderPtr BOBUIlsBackendOpenSSL::X509PemReader() const
{
    return BOBUIlsPrivate::X509CertificateOpenSSL::certificatesFromPem;
}

BOBUIlsPrivate::X509DerReaderPtr BOBUIlsBackendOpenSSL::X509DerReader() const
{
    return BOBUIlsPrivate::X509CertificateOpenSSL::certificatesFromDer;
}

BOBUIlsPrivate::X509Pkcs12ReaderPtr BOBUIlsBackendOpenSSL::X509Pkcs12Reader() const
{
    return BOBUIlsPrivate::X509CertificateOpenSSL::importPkcs12;
}

QList<int> BOBUIlsBackendOpenSSL::ellipticCurvesIds() const
{
    QList<int> ids;

#ifndef OPENSSL_NO_EC
    const size_t curveCount = q_EC_get_builtin_curves(nullptr, 0);
    QVarLengthArray<EC_builtin_curve> builtinCurves(static_cast<int>(curveCount));

    if (q_EC_get_builtin_curves(builtinCurves.data(), curveCount) == curveCount) {
        ids.reserve(curveCount);
        for (const auto &ec : builtinCurves)
            ids.push_back(ec.nid);
    }
#endif // OPENSSL_NO_EC

    return ids;
}

 int BOBUIlsBackendOpenSSL::curveIdFromShortName(const QString &name) const
 {
     int nid = 0;
     if (name.isEmpty())
         return nid;

     ensureInitialized(); // TLSTODO: check if it's needed!
#ifndef OPENSSL_NO_EC
     const QByteArray curveNameLatin1 = name.toLatin1();
     nid = q_OBJ_sn2nid(curveNameLatin1.data());

     if (nid == 0)
         nid = q_EC_curve_nist2nid(curveNameLatin1.data());
#endif // !OPENSSL_NO_EC

     return nid;
 }

 int BOBUIlsBackendOpenSSL::curveIdFromLongName(const QString &name) const
 {
     int nid = 0;
     if (name.isEmpty())
         return nid;

     ensureInitialized();

#ifndef OPENSSL_NO_EC
     const QByteArray curveNameLatin1 = name.toLatin1();
     nid = q_OBJ_ln2nid(curveNameLatin1.data());
#endif

     return nid;
 }

 QString BOBUIlsBackendOpenSSL::shortNameForId(int id) const
 {
     QString result;

#ifndef OPENSSL_NO_EC
     if (id != 0)
         result = QString::fromLatin1(q_OBJ_nid2sn(id));
#endif

     return result;
 }

QString BOBUIlsBackendOpenSSL::longNameForId(int id) const
{
    QString result;

#ifndef OPENSSL_NO_EC
    if (id != 0)
        result = QString::fromLatin1(q_OBJ_nid2ln(id));
#endif

    return result;
}

// NIDs of named curves allowed in TLS as per RFCs 4492 and 7027,
// see also https://www.iana.org/assignments/tls-parameters/tls-parameters.xhtml#tls-parameters-8
static const int tlsNamedCurveNIDs[] = {
    // RFC 4492
    NID_sect163k1,
    NID_sect163r1,
    NID_sect163r2,
    NID_sect193r1,
    NID_sect193r2,
    NID_sect233k1,
    NID_sect233r1,
    NID_sect239k1,
    NID_sect283k1,
    NID_sect283r1,
    NID_sect409k1,
    NID_sect409r1,
    NID_sect571k1,
    NID_sect571r1,

    NID_secp160k1,
    NID_secp160r1,
    NID_secp160r2,
    NID_secp192k1,
    NID_X9_62_prime192v1, // secp192r1
    NID_secp224k1,
    NID_secp224r1,
    NID_secp256k1,
    NID_X9_62_prime256v1, // secp256r1
    NID_secp384r1,
    NID_secp521r1,

    // RFC 7027
    NID_brainpoolP256r1,
    NID_brainpoolP384r1,
    NID_brainpoolP512r1
};

const size_t tlsNamedCurveNIDCount = sizeof(tlsNamedCurveNIDs) / sizeof(tlsNamedCurveNIDs[0]);

bool BOBUIlsBackendOpenSSL::isTlsNamedCurve(int id) const
{
    const int *const tlsNamedCurveNIDsEnd = tlsNamedCurveNIDs + tlsNamedCurveNIDCount;
    return std::find(tlsNamedCurveNIDs, tlsNamedCurveNIDsEnd, id) != tlsNamedCurveNIDsEnd;
}

QString BOBUIlsBackendOpenSSL::msgErrorsDuringHandshake()
{
    return QSslSocket::tr("Error during SSL handshake: %1").arg(getErrorsFromOpenSsl());
}

QSslCipher BOBUIlsBackendOpenSSL::bobui_OpenSSL_cipher_to_QSslCipher(const SSL_CIPHER *cipher)
{
    Q_ASSERT(cipher);
    char buf [256] = {};
    const QString desc = QString::fromLatin1(q_SSL_CIPHER_description(cipher, buf, sizeof(buf)));
    int supportedBits = 0;
    const int bits = q_SSL_CIPHER_get_bits(cipher, &supportedBits);
    return createCiphersuite(desc, bits, supportedBits);
}

void BOBUIlsBackendOpenSSL::forceAutotestSecurityLevel()
{
    QSslContext::forceAutoTestSecurityLevel();
}

BOBUI_END_NAMESPACE

#include "moc_bobuilsbackend_openssl_p.cpp"
