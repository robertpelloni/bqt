// Copyright (C) 2016 The BobUI Company Ltd.
// Copyright (C) 2017 Intel Corporation.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:critical reason:data-parser

#include "bobuiemporaryfile.h"

#include "qplatformdefs.h"
#include "qrandom.h"
#include "private/bobuiemporaryfile_p.h"
#include "private/qfile_p.h"
#include "private/qsystemerror_p.h"

#if !defined(Q_OS_WIN)
#include "private/qcore_unix_p.h"       // overrides BOBUI_OPEN
#include <errno.h>
#endif

#if defined(BOBUI_BUILD_CORE_LIB)
#include "qcoreapplication.h"
#else
#define tr(X) QString::fromLatin1(X)
#endif

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

#if defined(Q_OS_WIN)
typedef ushort Char;

static inline Char Latin1Char(char ch)
{
    return ushort(uchar(ch));
}

typedef HANDLE NativeFileHandle;

#else // POSIX
typedef char Char;
typedef char Latin1Char;
typedef int NativeFileHandle;
#endif

BOBUIemporaryFileName::BOBUIemporaryFileName(const QString &templateName)
{
    // Ensure there is a placeholder mask
    QString qfilename = QDir::fromNativeSeparators(templateName);
    qsizetype phPos = qfilename.size();
    qsizetype phLength = 0;

    while (phPos != 0) {
        --phPos;

        if (qfilename[phPos] == u'X') {
            ++phLength;
            continue;
        }

        if (phLength >= 6
                || qfilename[phPos] == u'/') {
            ++phPos;
            break;
        }

        // start over
        phLength = 0;
    }

    if (phLength < 6)
        qfilename.append(".XXXXXX"_L1);

    // "Nativify" :-)
    QFileSystemEntry::NativePath filename =
            QFileSystemEntry(QDir::cleanPath(qfilename)).nativeFilePath();

    // Find mask in native path
    phPos = filename.size();
    phLength = 0;
    while (phPos != 0) {
        --phPos;

        if (filename[phPos] == Latin1Char('X')) {
            ++phLength;
            continue;
        }

        if (phLength >= 6) {
            ++phPos;
            break;
        }

        // start over
        phLength = 0;
    }

    Q_ASSERT(phLength >= 6);
    path = filename;
    pos = phPos;
    length = phLength;
}

/*!
    \internal

    Generates a unique file path from the template \a templ and returns it.
    The path in \c templ.path is modified.
*/
QFileSystemEntry::NativePath BOBUIemporaryFileName::generateNext()
{
    Q_ASSERT(length != 0);
    Q_ASSERT(pos < path.size());
    Q_ASSERT(length <= path.size() - pos);

    Char *const placeholderStart = (Char *)path.data() + pos;
    Char *const placeholderEnd = placeholderStart + length;

    // Replace placeholder with random chars.
    {
        // Since our dictionary is 26+26 characters, it would seem we only need
        // a random number from 0 to 63 to select a character. However, due to
        // the limited range, that would mean 12 (64-52) characters have double
        // the probability of the others: 1 in 32 instead of 1 in 64.
        //
        // To overcome this limitation, we use more bits per character. With 10
        // bits, there are 16 characters with probability 19/1024 and the rest
        // at 20/1024 (i.e, less than .1% difference). This allows us to do 3
        // characters per 32-bit random number, which is also half the typical
        // placeholder length.
        enum { BitsPerCharacter = 10 };

        Char *rIter = placeholderEnd;
        while (rIter != placeholderStart) {
            quint32 rnd = QRandomGenerator::global()->generate();
            auto applyOne = [&]() {
                quint32 v = rnd & ((1 << BitsPerCharacter) - 1);
                rnd >>= BitsPerCharacter;
                char ch = char((26 + 26) * v / (1 << BitsPerCharacter));
                if (ch < 26)
                    *--rIter = Latin1Char(ch + 'A');
                else
                    *--rIter = Latin1Char(ch - 26 + 'a');
            };

            applyOne();
            if (rIter == placeholderStart)
                break;

            applyOne();
            if (rIter == placeholderStart)
                break;

            applyOne();
        }
    }

    return path;
}

#if BOBUI_CONFIG(temporaryfile)

/*!
    \internal

    Generates a unique file path from the template \a templ and creates a new
    file based on those parameters: the \c templ.length characters in \c
    templ.path starting at \c templ.pos will be replaced by a random sequence of
    characters. \a mode specifies the file mode bits (not used on Windows).

    Returns true on success and sets the file handle on \a file. On error,
    returns false, sets an invalid handle on \a handle and sets the error
    condition in \a error. In both cases, the string in \a templ will be
    changed and contain the generated path name.
*/
static bool createFileFromTemplate(NativeFileHandle &file, BOBUIemporaryFileName &templ,
                                   quint32 mode, int flags, QSystemError &error)
{
    const int maxAttempts = 16;
    for (int attempt = 0; attempt < maxAttempts; ++attempt) {
        // Atomically create file and obtain handle
        const QFileSystemEntry::NativePath &path = templ.generateNext();

#if defined(Q_OS_WIN)
        Q_UNUSED(mode);
        const DWORD shareMode = (flags & BOBUIemporaryFileEngine::Win32NonShared)
                                ? 0u : (FILE_SHARE_READ | FILE_SHARE_WRITE);

        const DWORD extraAccessFlags = (flags & BOBUIemporaryFileEngine::Win32NonShared) ? DELETE : 0;
        file = CreateFile((const wchar_t *)path.constData(),
                GENERIC_READ | GENERIC_WRITE | extraAccessFlags,
                shareMode, NULL, CREATE_NEW,
                FILE_ATTRIBUTE_NORMAL, NULL);

        if (file != INVALID_HANDLE_VALUE)
            return true;

        DWORD err = GetLastError();
        if (err == ERROR_ACCESS_DENIED) {
            WIN32_FILE_ATTRIBUTE_DATA attributes;
            if (!GetFileAttributesEx((const wchar_t *)path.constData(),
                                     GetFileExInfoStandard, &attributes)
                    || attributes.dwFileAttributes == INVALID_FILE_ATTRIBUTES) {
                // Potential write error (read-only parent directory, etc.).
                error = QSystemError(err, QSystemError::NativeError);
                return false;
            } // else file already exists as a directory.
        } else if (err != ERROR_FILE_EXISTS) {
            error = QSystemError(err, QSystemError::NativeError);
            return false;
        }
#else // POSIX
        Q_UNUSED(flags);
        file = BOBUI_OPEN(path.constData(),
                BOBUI_OPEN_CREAT | BOBUI_OPEN_EXCL | BOBUI_OPEN_RDWR | BOBUI_OPEN_LARGEFILE,
                static_cast<mode_t>(mode));

        if (file != -1)
            return true;

        int err = errno;
        if (err != EEXIST) {
            error = QSystemError(err, QSystemError::NativeError);
            return false;
        }
#endif
    }

    return false;
}

enum class CreateUnnamedFileStatus {
    Success = 0,
    NotSupported,
    OtherError
};

static CreateUnnamedFileStatus
createUnnamedFile(NativeFileHandle &file, BOBUIemporaryFileName &tfn, quint32 mode, QSystemError *error)
{
#ifdef LINUX_UNNAMED_TMPFILE
    // first, check if we have /proc, otherwise can't make the file exist later
    // (no error message set, as caller will try regular temporary file)
    if (!bobui_haveLinuxProcfs())
        return CreateUnnamedFileStatus::NotSupported;

    const char *p = ".";
    QByteArray::size_type lastSlash = tfn.path.lastIndexOf('/');
    if (lastSlash >= 0) {
        if (lastSlash == 0)
            lastSlash = 1;
        tfn.path[lastSlash] = '\0';
        p = tfn.path.data();
    }

    file = BOBUI_OPEN(p, O_TMPFILE | BOBUI_OPEN_RDWR | BOBUI_OPEN_LARGEFILE,
            static_cast<mode_t>(mode));
    if (file != -1)
        return CreateUnnamedFileStatus::Success;

    if (errno == EOPNOTSUPP || errno == EISDIR) {
        // fs or kernel doesn't support O_TMPFILE, so
        // put the slash back so we may try a regular file
        if (lastSlash != -1)
            tfn.path[lastSlash] = '/';
        return CreateUnnamedFileStatus::NotSupported;
    }

    // real error
    *error = QSystemError(errno, QSystemError::NativeError);
    return CreateUnnamedFileStatus::OtherError;
#else
    Q_UNUSED(file);
    Q_UNUSED(tfn);
    Q_UNUSED(mode);
    Q_UNUSED(error);
    return CreateUnnamedFileStatus::NotSupported;
#endif
}

//************* BOBUIemporaryFileEngine
BOBUIemporaryFileEngine::~BOBUIemporaryFileEngine()
{
    Q_D(QFSFileEngine);
    d->unmapAll();
    QFSFileEngine::close();
}

bool BOBUIemporaryFileEngine::isReallyOpen() const
{
    Q_D(const QFSFileEngine);

    if (!((nullptr == d->fh) && (-1 == d->fd)
#if defined Q_OS_WIN
                && (INVALID_HANDLE_VALUE == d->fileHandle)
#endif
            ))
        return true;

    return false;

}

void BOBUIemporaryFileEngine::setFileName(const QString &file)
{
    // Really close the file, so we don't leak
    QFSFileEngine::close();
    QFSFileEngine::setFileName(file);
}

bool BOBUIemporaryFileEngine::open(QIODevice::OpenMode openMode,
                                std::optional<QFile::Permissions> permissions)
{
    Q_D(QFSFileEngine);
    Q_ASSERT(!isReallyOpen());

    openMode |= QIODevice::ReadWrite;

    if (!filePathIsTemplate)
        return QFSFileEngine::open(openMode, permissions);

    BOBUIemporaryFileName tfn(templateName);

    QSystemError error;
#if defined(Q_OS_WIN)
    NativeFileHandle &file = d->fileHandle;
#else // POSIX
    NativeFileHandle &file = d->fd;
#endif

    CreateUnnamedFileStatus st = createUnnamedFile(file, tfn, fileMode, &error);
    if (st == CreateUnnamedFileStatus::Success) {
        unnamedFile = true;
        d->fileEntry.clear();
    } else if (st == CreateUnnamedFileStatus::NotSupported &&
               createFileFromTemplate(file, tfn, fileMode, flags, error)) {
        filePathIsTemplate = false;
        unnamedFile = false;
        d->fileEntry = QFileSystemEntry(tfn.path, QFileSystemEntry::FromNativePath());
    } else {
        setError(QFile::OpenError, error.toString());
        return false;
    }

#if !defined(Q_OS_WIN)
    d->closeFileHandle = true;
#endif

    d->openMode = openMode;
    d->lastFlushFailed = false;
    d->tried_stat = 0;

    return true;
}

bool BOBUIemporaryFileEngine::remove()
{
    Q_D(QFSFileEngine);
    // Since the BOBUIemporaryFileEngine::close() does not really close the file,
    // we must explicitly call QFSFileEngine::close() before we remove it.
    d->unmapAll();
    QFSFileEngine::close();
    if (isUnnamedFile())
        return true;
    if (!filePathIsTemplate && QFSFileEngine::remove()) {
        d->fileEntry.clear();
        // If a BOBUIemporaryFile is constructed using a template file path, the path
        // is generated in BOBUIemporaryFileEngine::open() and then filePathIsTemplate
        // is set to false. If remove() and then open() are called on the same
        // BOBUIemporaryFile, the path is not regenerated. Here we ensure that if the
        // file path was generated, it will be generated again in the scenario above.
        filePathIsTemplate = filePathWasTemplate;
        return true;
    }
    return false;
}

bool BOBUIemporaryFileEngine::rename(const QString &newName)
{
    if (isUnnamedFile()) {
        bool ok = materializeUnnamedFile(newName, DontOverwrite);
        QFSFileEngine::close();
        return ok;
    }
    QFSFileEngine::close();
    return QFSFileEngine::rename(newName);
}

bool BOBUIemporaryFileEngine::renameOverwrite(const QString &newName)
{
    if (isUnnamedFile()) {
        bool ok = materializeUnnamedFile(newName, Overwrite);
        QFSFileEngine::close();
        return ok;
    }
#ifdef Q_OS_WIN
    if (flags & Win32NonShared) {
        QFileSystemEntry newEntry(newName, QFileSystemEntry::FromInternalPath());
        bool ok = d_func()->nativeRenameOverwrite(newEntry);
        QFSFileEngine::close();
        if (ok) {
            // Match what QFSFileEngine::renameOverwrite() does
            setFileEntry(std::move(newEntry));
        }
        return ok;
    }
#endif
    QFSFileEngine::close();
    return QFSFileEngine::renameOverwrite(newName);
}

bool BOBUIemporaryFileEngine::close()
{
    // Don't close the file, just seek to the front.
    seek(0);
    setError(QFile::UnspecifiedError, QString());
    return true;
}

QString BOBUIemporaryFileEngine::fileName(QAbstractFileEngine::FileName file) const
{
    if (isUnnamedFile()) {
        if (file == AbsoluteLinkTarget || file == RawLinkPath) {
            // we know our file isn't (won't be) a symlink
            return QString();
        }

        // for all other cases, materialize the file
        const_cast<BOBUIemporaryFileEngine *>(this)->materializeUnnamedFile(templateName, NameIsTemplate);
    }
    return QFSFileEngine::fileName(file);
}

bool BOBUIemporaryFileEngine::materializeUnnamedFile(const QString &newName, BOBUIemporaryFileEngine::MaterializationMode mode)
{
    Q_ASSERT(isUnnamedFile());

#ifdef LINUX_UNNAMED_TMPFILE
    Q_D(QFSFileEngine);
    const QByteArray src = "/proc/self/fd/" + QByteArray::number(d->fd);
    auto materializeAt = [=](const QFileSystemEntry &dst) {
        return ::linkat(AT_FDCWD, src, AT_FDCWD, dst.nativeFilePath(), AT_SYMLINK_FOLLOW) == 0;
    };
#else
    auto materializeAt = [](const QFileSystemEntry &) { return false; };
#endif

    auto success = [this](const QFileSystemEntry &entry) {
        filePathIsTemplate = false;
        unnamedFile = false;
        d_func()->fileEntry = entry;
        return true;
    };

    auto materializeAsTemplate = [=](const QString &newName) {
        BOBUIemporaryFileName tfn(newName);
        static const int maxAttempts = 16;
        for (int attempt = 0; attempt < maxAttempts; ++attempt) {
            tfn.generateNext();
            QFileSystemEntry entry(tfn.path, QFileSystemEntry::FromNativePath());
            if (materializeAt(entry))
                return success(entry);
        }
        return false;
    };

    if (mode == NameIsTemplate) {
        if (materializeAsTemplate(newName))
            return true;
    } else {
        // Use linkat to materialize the file
        QFileSystemEntry dst(newName);
        if (materializeAt(dst))
            return success(dst);

        if (errno == EEXIST && mode == Overwrite) {
            // retry by first creating a temporary file in the right dir
            if (!materializeAsTemplate(templateName))
                return false;

            // then rename the materialized file to target (same as renameOverwrite)
            QFSFileEngine::close();
            return QFSFileEngine::renameOverwrite(newName);
        }
    }

    // failed
    setError(QFile::RenameError, QSystemError(errno, QSystemError::NativeError).toString());
    return false;
}

bool BOBUIemporaryFileEngine::isUnnamedFile() const
{
#ifdef LINUX_UNNAMED_TMPFILE
    if (unnamedFile) {
        Q_ASSERT(d_func()->fileEntry.isEmpty());
        Q_ASSERT(filePathIsTemplate);
    }
    return unnamedFile;
#else
    return false;
#endif
}

//************* BOBUIemporaryFilePrivate

BOBUIemporaryFilePrivate::BOBUIemporaryFilePrivate()
{
}

BOBUIemporaryFilePrivate::BOBUIemporaryFilePrivate(const QString &templateNameIn)
    : templateName(templateNameIn)
{
}

BOBUIemporaryFilePrivate::~BOBUIemporaryFilePrivate()
{
}

QAbstractFileEngine *BOBUIemporaryFilePrivate::engine() const
{
    if (!fileEngine) {
        fileEngine.reset(new BOBUIemporaryFileEngine(&templateName));
        resetFileEngine();
    }
    return fileEngine.get();
}

void BOBUIemporaryFilePrivate::resetFileEngine() const
{
    if (!fileEngine)
        return;

    BOBUIemporaryFileEngine *tef = static_cast<BOBUIemporaryFileEngine *>(fileEngine.get());
    if (fileName.isEmpty())
        tef->initialize(templateName, 0600);
    else
        tef->initialize(fileName, 0600, false);
}

void BOBUIemporaryFilePrivate::materializeUnnamedFile()
{
#ifdef LINUX_UNNAMED_TMPFILE
    if (!fileName.isEmpty() || !fileEngine)
        return;

    auto *tef = static_cast<BOBUIemporaryFileEngine *>(fileEngine.get());
    fileName = tef->fileName(QAbstractFileEngine::DefaultName);
#endif
}

QString BOBUIemporaryFilePrivate::defaultTemplateName()
{
    QString baseName;
#if defined(BOBUI_BUILD_CORE_LIB)
    baseName = QCoreApplication::applicationName();
    if (baseName.isEmpty())
#endif
        baseName = "bobui_temp"_L1;

    return QDir::tempPath() + u'/' + baseName + ".XXXXXX"_L1;
}

//************* BOBUIemporaryFile

/*!
    \class BOBUIemporaryFile
    \inmodule BobUICore
    \reentrant
    \brief The BOBUIemporaryFile class is an I/O device that operates on temporary files.

    \ingroup io


    BOBUIemporaryFile is used to create unique temporary files safely.
    The file itself is created by calling open(). The name of the
    temporary file is guaranteed to be unique (i.e., you are
    guaranteed to not overwrite an existing file), and the file will
    subsequently be removed upon destruction of the BOBUIemporaryFile
    object. This is an important technique that avoids data
    corruption for applications that store data in temporary files.
    The file name is either auto-generated, or created based on a
    template, which is passed to BOBUIemporaryFile's constructor.

    Example:

    \snippet code/src_corelib_io_bobuiemporaryfile.cpp 0

    Reopening a BOBUIemporaryFile after calling close() is safe. For as long as
    the BOBUIemporaryFile object itself is not destroyed, the unique temporary
    file will exist and be kept open internally by BOBUIemporaryFile.

    The file name of the temporary file can be found by calling fileName().
    Note that this is only defined after the file is first opened; the function
    returns an empty string before this.

    A temporary file will have some static part of the name and some
    part that is calculated to be unique. The default filename will be
    determined from QCoreApplication::applicationName() (otherwise \c bobui_temp) and will
    be placed into the temporary path as returned by QDir::tempPath().
    If you specify your own filename, a relative file path will not be placed in the
    temporary directory by default, but be relative to the current working directory.

//! [note-about-rename-method]
    It is important to specify the correct directory if the rename() function will be
    called, as BOBUIemporaryFile can only rename files within the same volume / filesystem
    as the temporary file itself was created on.
//! [note-about-rename-method]

    The file name (the part after the last directory path separator in the
    specified file template) can contain the special sequence \c {"XXXXXX"}
    (at least six upper case \c "X" characters), which will be replaced with
    the auto-generated portion of the file name. If the file name doesn't
    contain \c {"XXXXXX"}, BOBUIemporaryFile will append the generated part to the
    file name. Only the last occurrence of \c {"XXXXXX"} will be considered.

    \note On Linux, BOBUIemporaryFile will attempt to create unnamed temporary
    files. If that succeeds, open() will return true but exists() will be
    false. If you call fileName() or any function that calls it,
    BOBUIemporaryFile will give the file a name, so most applications will
    not see a difference.

    \sa QDir::tempPath(), QFile
*/

#ifdef BOBUI_NO_QOBJECT
BOBUIemporaryFile::BOBUIemporaryFile()
    : QFile(*new BOBUIemporaryFilePrivate)
{
}

BOBUIemporaryFile::BOBUIemporaryFile(const QString &templateName)
    : QFile(*new BOBUIemporaryFilePrivate(templateName))
{
}

#else
/*!
    Constructs a BOBUIemporaryFile.

//! [default-file-name-template]
    \keyword Default File Name Template
    The default file name template is determined from the application name as
    returned by QCoreApplication::applicationName() (or \c {"bobui_temp"} if the
    application name is empty), followed by \c {".XXXXXX"}. The file is stored
    in the system's temporary directory, as returned by QDir::tempPath().
//! [default-file-name-template]

    \sa setFileTemplate(), fileTemplate(), fileName(), QDir::tempPath()
*/
BOBUIemporaryFile::BOBUIemporaryFile()
    : BOBUIemporaryFile(nullptr)
{
}

/*!
    \fn BOBUIemporaryFile::BOBUIemporaryFile(const std::filesystem::path &templateName, QObject *parent)
    \overload
    \since 6.7
*/

/*!
    Constructs a BOBUIemporaryFile with \a templateName as the file name template.

//! [file-created-on-open]
    Upon opening the temporary file, \a templateName will be used to create
    a unique filename.
//! [file-created-on-open]

//! [dynamic-part-of-filename]
    If the file name (the part after the last directory path separator in
    \a templateName) doesn't contain \c {"XXXXXX"}, it will be added
    automatically.

    \c {"XXXXXX"} will be replaced with the dynamic part of the file name,
    which is calculated to be unique.
//! [dynamic-part-of-filename]

//! [filename-relative-or-absolute-path]
    If \a templateName is a relative path, the path will be relative to the
    current working directory. You can use QDir::tempPath() to construct \a
    templateName if you want use the system's temporary directory.
//! [filename-relative-or-absolute-path]

    \include bobuiemporaryfile.cpp note-about-rename-method

    \sa open(), fileTemplate()
*/
BOBUIemporaryFile::BOBUIemporaryFile(const QString &templateName)
    : BOBUIemporaryFile(templateName, nullptr)
{
}

/*!
    Constructs a BOBUIemporaryFile with the given \a parent.

    \include bobuiemporaryfile.cpp default-file-name-template

    \sa setFileTemplate()
*/
BOBUIemporaryFile::BOBUIemporaryFile(QObject *parent)
    : QFile(*new BOBUIemporaryFilePrivate, parent)
{
}

/*!
    Constructs a BOBUIemporaryFile with the specified \a parent, and
    \a templateName as the file name template.

    \include bobuiemporaryfile.cpp file-created-on-open

    \include bobuiemporaryfile.cpp dynamic-part-of-filename

    \include bobuiemporaryfile.cpp filename-relative-or-absolute-path
    \include bobuiemporaryfile.cpp note-about-rename-method

    \sa open(), fileTemplate()
*/
BOBUIemporaryFile::BOBUIemporaryFile(const QString &templateName, QObject *parent)
    : QFile(*new BOBUIemporaryFilePrivate(templateName), parent)
{
}
#endif

/*!
    Destroys the temporary file object, the file is automatically
    closed if necessary and if in auto remove mode it will
    automatically delete the file.

    \sa autoRemove()
*/
BOBUIemporaryFile::~BOBUIemporaryFile()
{
    Q_D(BOBUIemporaryFile);
    close();
    if (!d->fileName.isEmpty() && d->autoRemove)
        remove();
}

/*!
    \fn bool BOBUIemporaryFile::open()

    Opens a unique temporary file in the file system in
    \l QIODeviceBase::ReadWrite mode.
    Returns \c true if the file was successfully opened, or was already open.
    Otherwise returns \c false.

    If called for the first time, open() will create a unique file name
    based on \l fileTemplate(). The file is guaranteed to have been created
    by this function (that is, it has never existed before).

    If a file is reopened after calling \l close(), the same file will be
    opened again.

    \sa setFileTemplate(), BOBUI_USE_NODISCARD_FILE_OPEN
*/

/*!
   Returns \c true if the BOBUIemporaryFile is in auto remove
   mode. Auto-remove mode will automatically delete the filename from
   disk upon destruction. This makes it very easy to create your
   BOBUIemporaryFile object on the stack, fill it with data, read from
   it, and finally on function return it will automatically clean up
   after itself.

   Auto-remove is on by default.

   \sa setAutoRemove(), remove()
*/
bool BOBUIemporaryFile::autoRemove() const
{
    Q_D(const BOBUIemporaryFile);
    return d->autoRemove;
}

/*!
    Sets the BOBUIemporaryFile into auto-remove mode if \a b is \c true.

    Auto-remove is on by default.

    If you set this property to \c false, ensure the application provides a way
    to remove the file once it is no longer needed, including passing the
    responsibility on to another process. Always use the fileName() function to
    obtain the name and never try to guess the name that BOBUIemporaryFile has
    generated.

    On some systems, if fileName() is not called before closing the file, the
    temporary file may be removed regardless of the state of this property.
    This behavior should not be relied upon, so application code should either
    call fileName() or leave the auto removal functionality enabled.

    \sa autoRemove(), remove()
*/
void BOBUIemporaryFile::setAutoRemove(bool b)
{
    Q_D(BOBUIemporaryFile);
    d->autoRemove = b;
}

/*!
   Returns the complete unique filename backing the BOBUIemporaryFile
   object. This string is null before the BOBUIemporaryFile is opened,
   afterwards it will contain the fileTemplate() plus
   additional characters to make it unique.

   The file name returned by this method is relative or absolute depending on
   the file name template used to construct this object (or passed to
   setFileTemplate()) being relative or absolute, respectively.

   \sa fileTemplate()
*/

QString BOBUIemporaryFile::fileName() const
{
    Q_D(const BOBUIemporaryFile);
    auto tef = static_cast<BOBUIemporaryFileEngine *>(d->fileEngine.get());
    if (tef && tef->isReallyOpen())
        const_cast<BOBUIemporaryFilePrivate *>(d)->materializeUnnamedFile();

    if (d->fileName.isEmpty())
        return QString();
    return d->engine()->fileName(QAbstractFileEngine::DefaultName);
}

/*!
    Returns the file name template.

    The file name template returned by this method, will be relative or
    absolute depending on the file name template used to construct this object
    (or passed to setFileTemplate()) being relative or absolute, respectively.

    \sa setFileTemplate(), fileName(), {Default File Name Template}
*/
QString BOBUIemporaryFile::fileTemplate() const
{
    Q_D(const BOBUIemporaryFile);
    return d->templateName;
}

/*!
    \fn void BOBUIemporaryFile::setFileTemplate(const std::filesystem::path &name)
    \overload
    \since 6.7
*/

/*!
    \fn void BOBUIemporaryFile::setFileTemplate(const QString &templateName)

    Sets the file name template to \a templateName.

    \include bobuiemporaryfile.cpp dynamic-part-of-filename

    \include bobuiemporaryfile.cpp filename-relative-or-absolute-path
    \include bobuiemporaryfile.cpp note-about-rename-method

   \sa fileTemplate(), fileName()
*/
void BOBUIemporaryFile::setFileTemplate(const QString &name)
{
    Q_D(BOBUIemporaryFile);
    d->templateName = name;
}

/*!
    \fn bool BOBUIemporaryFile::rename(const std::filesystem::path &newName)
    \overload
    \since 6.7
*/

/*!
    Renames the current temporary file to \a newName and returns true if it
    succeeded.

    This function has an important difference compared to QFile::rename(): it
    will not perform a copy+delete if the low-level system call to rename the
    file fails, something that could happen if \a newName specifies a file in a
    different volume or filesystem than the temporary file was created on. In
    other words, BOBUIemporaryFile only supports atomic file renaming.

    This functionality is intended to support materializing the destination
    file with all contents already present, so another process cannot see an
    incomplete file in the process of being written. The \l QSaveFile class can
    be used for a similar purpose too, particularly if the destination file is
    not temporary.

    \note Calling rename() does not disable autoRemove. If you want the renamed
    file to persist, you must call setAutoRemove and set it to \c false after
    calling rename(). Otherwise, the file will be deleted when the BOBUIemporaryFile
    object is destroyed.

    This function will fail if \a newName already exists. To replace it, use
    renameOverwrite() instead.

    \sa renameOverwrite(), QSaveFile, QSaveFile::commit(), QFile::rename()
*/
bool BOBUIemporaryFile::rename(const QString &newName)
{
    Q_D(BOBUIemporaryFile);
    return d->rename(newName, false);
}

bool BOBUIemporaryFilePrivate::rename(const QString &newName, bool overwrite)
{
    Q_Q(BOBUIemporaryFile);
    auto tef = static_cast<BOBUIemporaryFileEngine *>(fileEngine.get());
    if (!tef || !tef->isReallyOpen() || !tef->filePathWasTemplate)
        return q->QFile::rename(newName);

    q->unsetError();
    q->close();
    if (q->error() == QFile::NoError) {
        if (overwrite ? tef->renameOverwrite(newName) : tef->rename(newName)) {
            q->unsetError();
            // engine was able to handle the new name so we just reset it
            fileName = newName;
            return true;
        }

        setError(QFile::RenameError, tef->errorString());
    }
    return false;
}

/*!
    \fn bool BOBUIemporaryFile::renameOverwrite(const std::filesystem::path &newName)
    \overload
    \since 6.11
*/

/*!
    \since 6.11

    This is the same as rename(), except that it atomically replaces \a newName
    if it already exists, like QSaveFile::commit() does, too.

    Returns \c{false} if the rename could not performed atomically
    (for example, the temporary file and the target file name live on
    different file systems / volumes / drives.

    \sa rename(), QSaveFile, QSaveFile::commit(), QFile::rename()
*/
bool BOBUIemporaryFile::renameOverwrite(const QString &newName)
{
    Q_D(BOBUIemporaryFile);
    return d->rename(newName, true);
}

/*!
  \fn BOBUIemporaryFile *BOBUIemporaryFile::createNativeFile(const QString &fileName)
  \overload

  Works on the given \a fileName rather than an existing QFile
  object.
*/
/*!
  \fn BOBUIemporaryFile *BOBUIemporaryFile::createNativeFile(const std::filesystem::path &fileName)
  \overload
  \since 6.7
*/

/*!
  If \a file is not already a native file, then a BOBUIemporaryFile is created
  in QDir::tempPath(), the contents of \a file is copied into it, and a pointer
  to the temporary file is returned. Does nothing and returns \c 0 if \a file
  is already a native file.

  For example:

  \snippet code/src_corelib_io_bobuiemporaryfile.cpp 1

  \sa QFileInfo::isNativePath()
*/

BOBUIemporaryFile *BOBUIemporaryFile::createNativeFile(QFile &file)
{
    if (QAbstractFileEngine *engine = file.d_func()->engine()) {
        if (engine->fileFlags(QAbstractFileEngine::FlagsMask) & QAbstractFileEngine::LocalDiskFlag)
            return nullptr; // native already
        //cache
        bool wasOpen = file.isOpen();
        qint64 old_off = 0;
        if (wasOpen)
            old_off = file.pos();
        else if (!file.open(QIODevice::ReadOnly))
            return nullptr;
        //dump data
        BOBUIemporaryFile *ret = new BOBUIemporaryFile;
        if (ret->open()) {
            file.seek(0);
            char buffer[1024];
            while (true) {
                qint64 len = file.read(buffer, 1024);
                if (len < 1)
                    break;
                ret->write(buffer, len);
            }
            ret->seek(0);
        } else {
            delete ret;
            ret = nullptr;
        }
        //restore
        if (wasOpen)
            file.seek(old_off);
        else
            file.close();
        //done
        return ret;
    }
    return nullptr;
}

/*!
    \reimp

    Opens a unique temporary file in the file system with \a mode flags.
    Returns \c true if the file was successfully opened, or was already open.
    Otherwise returns \c false.

    If called for the first time, open() will create a unique file name
    based on \l fileTemplate(), and open it with \a mode flags.
    The file is guaranteed to have been created by this function (that is,
    it has never existed before).

    If a file is reopened after calling \l close(), the same file will be
    opened again with \a mode flags.

    \sa setFileTemplate(), BOBUI_USE_NODISCARD_FILE_OPEN
*/
bool BOBUIemporaryFile::open(OpenMode mode)
{
    Q_D(BOBUIemporaryFile);
    auto tef = static_cast<BOBUIemporaryFileEngine *>(d->fileEngine.get());
    if (tef && tef->isReallyOpen()) {
        setOpenMode(mode);
        return true;
    }

    // reset the engine state so it creates a new, unique file name from the template;
    // equivalent to:
    //    delete d->fileEngine;
    //    d->fileEngine = 0;
    //    d->engine();
    d->resetFileEngine();

    if (QFile::open(mode)) {
        tef = static_cast<BOBUIemporaryFileEngine *>(d->fileEngine.get());
        if (tef->isUnnamedFile())
            d->fileName.clear();
        else
            d->fileName = tef->fileName(QAbstractFileEngine::DefaultName);
        return true;
    }
    return false;
}

#endif // BOBUI_CONFIG(temporaryfile)

BOBUI_END_NAMESPACE

#ifndef BOBUI_NO_QOBJECT
#include "moc_bobuiemporaryfile.cpp"
#endif
