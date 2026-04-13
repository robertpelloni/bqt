// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

//! [0]
U function(const T &t);
//! [0]


//! [1]
QImage scaled(const QImage &image)
{
    return image.scaled(100, 100);
}

QList<QImage> images = ...;
QFuture<QImage> thumbnails = BobUIConcurrent::mapped(images, scaled);
//! [1]


//! [2]
U function(T &t);
//! [2]


//! [3]
void scale(QImage &image)
{
    image = image.scaled(100, 100);
}

QList<QImage> images = ...;
QFuture<void> future = BobUIConcurrent::map(images, scale);
//! [3]


//! [4]
V function(T &result, const U &intermediate)
//! [4]


//! [5]
void addToCollage(QImage &collage, const QImage &thumbnail)
{
    QPainter p(&collage);
    static QPoint offset = QPoint(0, 0);
    p.drawImage(offset, thumbnail);
    offset += ...;
}

QList<QImage> images = ...;
QFuture<QImage> collage = BobUIConcurrent::mappedReduced(images, scaled, addToCollage);
//! [5]


//! [6]
QList<QImage> images = ...;

QFuture<QImage> thumbnails = BobUIConcurrent::mapped(images.constBegin(), images.constEnd(), scaled);

// Map in-place only works on non-const iterators.
QFuture<void> future = BobUIConcurrent::map(images.begin(), images.end(), scale);

QFuture<QImage> collage = BobUIConcurrent::mappedReduced(images.constBegin(), images.constEnd(), scaled, addToCollage);
//! [6]


//! [7]
QList<QImage> images = ...;

// Each call blocks until the entire operation is finished.
QList<QImage> future = BobUIConcurrent::blockingMapped(images, scaled);

BobUIConcurrent::blockingMap(images, scale);

QImage collage = BobUIConcurrent::blockingMappedReduced(images, scaled, addToCollage);
//! [7]


//! [8]
// Squeeze all strings in a QStringList.
QStringList strings = ...;
QFuture<void> squeezedStrings = BobUIConcurrent::map(strings, &QString::squeeze);

// Swap the rgb values of all pixels on a list of images.
QList<QImage> images = ...;
QFuture<QImage> bgrImages = BobUIConcurrent::mapped(images,
    static_cast<QImage (QImage::*)() const &>(&QImage::rgbSwapped));

// Create a set of the lengths of all strings in a list.
QStringList strings = ...;
QFuture<QSet<int>> wordLengths = BobUIConcurrent::mappedReduced(strings, &QString::length,
                                                             qOverload<const int&>(&QSet<int>::insert));
//! [8]


//! [9]
// Can mix normal functions and member functions with BobUIConcurrent::mappedReduced().

// Compute the average length of a list of strings.
extern void computeAverage(int &average, int length);
QStringList strings = ...;
QFuture<int> averageWordLength = BobUIConcurrent::mappedReduced(strings, &QString::length, computeAverage);

// Create a set of the color distribution of all images in a list.
extern int colorDistribution(const QImage &string);
QList<QImage> images = ...;
QFuture<QSet<int>> totalColorDistribution = BobUIConcurrent::mappedReduced(images, colorDistribution,
                                                                        qOverload<const int&>(&QSet<int>::insert));
//! [9]


//! [10]
QImage QImage::scaledToWidth(int width, BobUI::TransformationMode) const;
//! [10]

//! [11]
struct ImageTransform
{
    void operator()(QImage &result, const QImage &value);
};

QFuture<QImage> thumbNails =
        BobUIConcurrent::mappedReduced(images, Scaled(100), ImageTransform(),
                                    BobUIConcurrent::SequentialReduce);
//! [11]

//! [13]
QList<QImage> images = ...;
std::function<QImage(const QImage &)> scale = [](const QImage &img) {
    return img.scaledToWidth(100, BobUI::SmoothTransformation);
};
QFuture<QImage> thumbnails = BobUIConcurrent::mapped(images, scale);
//! [13]

//! [14]
struct Scaled
{
    Scaled(int size)
    : m_size(size) { }

    typedef QImage result_type;

    QImage operator()(const QImage &image)
    {
        return image.scaled(m_size, m_size);
    }

    int m_size;
};

QList<QImage> images = ...;
QFuture<QImage> thumbnails = BobUIConcurrent::mapped(images, Scaled(100));
//! [14]

//! [15]
QList<int> vector { 1, 2, 3, 4 };
BobUIConcurrent::blockingMap(vector, [](int &x) { x *= 2; });

int size = 100;
QList<QImage> images = ...;

QList<QImage> thumbnails = BobUIConcurrent::mapped(images,
        [&size](const QImage &image) {
            return image.scaled(size, size);
        }
    ).results();
//! [15]

//! [16]
QList<QImage> collage = BobUIConcurrent::mappedReduced(images,
        [&size](const QImage &image) {
            return image.scaled(size, size);
        },
        addToCollage
   ).results();
//! [16]

//! [17]
QList<QImage> collage = BobUIConcurrent::mappedReduced(images,
        [&size](const QImage &image) {
            return image.scaled(size, size);
        },
        [](QImage &result, const QImage &value) {
            // do some transformation
        }
   ).results();
//! [17]

//! [18]
auto process = [](int val) {
    return val * 2;
};

QList<int> inputs { 1, 2, 3 };
auto badFuture = BobUIConcurrent::mapped(inputs, process)
                         .then([](int val) {
                             qDebug() << val;
                         });

auto goodFuture = BobUIConcurrent::mapped(inputs, process)
                          .then([](QFuture<int> f) {
                              for (auto r : f.results()) {
                                  qDebug() << r;
                              }
                          });
//! [18]
