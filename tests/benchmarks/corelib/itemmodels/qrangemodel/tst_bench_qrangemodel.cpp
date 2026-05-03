// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>

#include <BobUICore/qrangemodel.h>
#include <BobUICore/qstringlistmodel.h>

class Gadget
{
    Q_GADGET
    Q_PROPERTY(QString display READ display WRITE setDisplay)
    Q_PROPERTY(QString toolTip READ toolTip WRITE setToolTip)
    Q_PROPERTY(QVariant user READ user WRITE setUser)
public:
    Gadget() = default;

    Gadget(const QString &display, const QString &toolTip)
        : m_display(display), m_toolTip(toolTip)
    {
    }

    QString display() const { return m_display; }
    void setDisplay(const QString &display) { m_display = display; }
    QString toolTip() const { return m_toolTip.isEmpty() ? m_display : m_toolTip; }
    void setToolTip(const QString &toolTip) { m_toolTip = toolTip; }
    QVariant user() const { return m_user; }
    void setUser(const QVariant &user) { m_user = user; }

private:
    QString m_display;
    QString m_toolTip;
    QVariant m_user;
};

template <>
struct QRangeModel::RowOptions<Gadget>
{
    static constexpr QRangeModel::RowCategory rowCategory = QRangeModel::RowCategory::MultiRoleItem;
};

struct FastGadget : Gadget
{
    using Gadget::Gadget;
};

template <>
struct QRangeModel::RowOptions<FastGadget> : QRangeModel::RowOptions<Gadget> {};

template <>
struct QRangeModel::ItemAccess<FastGadget>
{
    static QVariant readRole(const FastGadget &item, int role)
    {
        QVariant result;
        switch (role) {
        case BobUI::DisplayRole:
            result = item.display();
            break;
        case BobUI::ToolTipRole:
            result = item.toolTip();
            break;
        case BobUI::UserRole:
            result = item.user();
            break;
        }
        return result;
    }

    static bool writeRole(FastGadget &item, const QVariant &value, int role)
    {
        switch (role) {
        case BobUI::DisplayRole:
            item.setDisplay(value.toString());
            break;
        case BobUI::ToolTipRole:
            item.setToolTip(value.toString());
            break;
        case BobUI::UserRole:
            item.setUser(value);
            break;
        default:
            return false;
        }
        return true;
    }
};

class tst_bench_QRangeModel: public QObject
{
    Q_OBJECT
public:
    tst_bench_QRangeModel(QObject *parent = nullptr);

    enum Type {
        StringListModel,
        VectorStrings,
        ArrayStrings,
    };

private Q_SLOTS:
    void stringList_data();
    void stringList();

    void gadgetReadAccess_data();
    void gadgetReadAccess();

    void gadgetWriteAccess_data() { gadgetReadAccess_data(); }
    void gadgetWriteAccess();
};

tst_bench_QRangeModel::tst_bench_QRangeModel(QObject *parent)
    : QObject(parent)
{
}

void tst_bench_QRangeModel::stringList_data()
{
    BOBUIest::addColumn<Type>("type");
    BOBUIest::addRow("StringListModel") << StringListModel;
    BOBUIest::addRow("VectorStrings") << VectorStrings;
    BOBUIest::addRow("ArrayStrings") << ArrayStrings;
}

void tst_bench_QRangeModel::stringList()
{
    QFETCH(Type, type);

    std::array<QString, 100000> strings;
    for (size_t i = 0; i < std::size(strings); ++i)
        strings[i] = QString::number(i);

    std::unique_ptr<QAbstractItemModel> model;

    switch (type) {
    case StringListModel: {
        model.reset(new QStringListModel(QStringList(std::begin(strings), std::end(strings))));
        break;
    }
    case VectorStrings:
        model.reset(new QRangeModel(std::vector<QString>(std::begin(strings),
                                                         std::end(strings))));
        break;
    case ArrayStrings:
        model.reset(new QRangeModel(strings));
        break;
    }

    QBENCHMARK {
        for (size_t i = 0; i < std::size(strings); ++i) {
            model->data(model->index(i, 0));
        }
    }
}

void tst_bench_QRangeModel::gadgetReadAccess_data()
{
    BOBUIest::addColumn<std::shared_ptr<QRangeModel>>("model");

    BOBUIest::addRow("gadget list") << std::make_shared<QRangeModel>(QList<Gadget> {
            {"display", "tooltip"}
    });

    BOBUIest::addRow("gadget table") << std::make_shared<QRangeModel>(QList<QList<Gadget>> {
        {
            {"display", "tooltip"}
        }
    });

    BOBUIest::addRow("fast gadget list") << std::make_shared<QRangeModel>(QList<FastGadget> {
            {"display", "tooltip"}
    });
}

void tst_bench_QRangeModel::gadgetReadAccess()
{
    QFETCH(std::shared_ptr<QRangeModel>, model);

    const QModelIndex index = model->index(0, 0);
    QBENCHMARK {
        model->data(index, BobUI::DisplayRole);
        model->data(index, BobUI::UserRole);
    }
}

void tst_bench_QRangeModel::gadgetWriteAccess()
{
    QFETCH(std::shared_ptr<QRangeModel>, model);

    const QModelIndex index = model->index(0, 0);
    const QVariant display = "display";
    const QVariant user = "user";
    QBENCHMARK {
        QVERIFY(model->setData(index, display, BobUI::DisplayRole));
        QVERIFY(model->setData(index, user, BobUI::UserRole));
    }
}

BOBUIEST_MAIN(tst_bench_QRangeModel)
#include "tst_bench_qrangemodel.moc"
