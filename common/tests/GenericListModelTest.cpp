// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include <Common/GenericListModel.hpp>
#include <QModelIndex>
#include <QTest>

namespace RapidAndroid::Common
{

namespace
{

struct TestType
{
    qint32 value = 0;

    bool operator==(TestType const& rhs) const = default;
    bool operator!=(TestType const& rhs) const = default;
};

class TestModel : public GenericListModel<TestType>
{
    Q_OBJECT
public:
    enum Role
    {
        Role1 = ::Qt::UserRole + 1,
    };

    explicit TestModel(std::vector<TestType> const& initialData = {})
        : GenericListModel<TestType>{getTestRoles(), initialData}
    {
    }

    ~TestModel() override = default;

    Q_DISABLE_COPY_MOVE(TestModel)

    Roles getTestRoles() const noexcept
    {
        return {{static_cast<qint32>(Role::Role1), "role1"}};
    }

    QVariant data(QModelIndex const& index, int role = ::Qt::DisplayRole) const noexcept override
    {
        if (not index.isValid()) {
            return {};
        }

        auto maybeElement = getElement(static_cast<std::size_t>(index.row()));
        if (role == static_cast<int>(Role1) && maybeElement.has_value()) {
            return maybeElement.value()->value;
        }
        return {};
    }
};

class DefaultRoleNameModel : public GenericListModel<TestType>
{
    Q_OBJECT
public:
    QVariant data([[maybe_unused]] QModelIndex const& index,
                  [[maybe_unused]] int role = ::Qt::DisplayRole) const noexcept override
    {
        return {};
    }

    Roles getDefaultRoles()
    {
        return {{Qt::DisplayRole, "display"},
                {Qt::DecorationRole, "decoration"},
                {Qt::EditRole, "edit"},
                {Qt::ToolTipRole, "toolTip"},
                {Qt::StatusTipRole, "statusTip"},
                {Qt::WhatsThisRole, "whatsThis"}};
    }
};

struct TestData
{
    std::vector<TestType> data{TestType{.value = 1}, TestType{.value = 2}};
};

} // namespace

class GenericListModelTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testRoleNamesCustom()
    {
        TestModel model;
        QCOMPARE(model.roleNames(), model.getTestRoles());
    }

    void testRoleNamesDefault()
    {
        DefaultRoleNameModel model;
        QCOMPARE(model.roleNames(), model.getDefaultRoles());
    }

    void testRowCount()
    {
        TestData fix;
        TestModel model(fix.data);
        QCOMPARE(model.rowCount(QModelIndex{}), 2);
    }

    void testDataReturnsValues()
    {
        TestData fix;
        TestModel model(fix.data);
        QCOMPARE(model.data(model.index(0), TestModel::Role1).value<qint32>(), 1);
        QCOMPARE(model.data(model.index(1), TestModel::Role1).value<qint32>(), 2);
    }

    void testInsertOperations()
    {
        TestData fix;
        TestModel model(fix.data);

        // Insert at the end
        QCOMPARE(model.insertItem(TestType{.value = 3}), 2);
        QCOMPARE(model.data(model.index(2), TestModel::Role1).value<qint32>(), 3);

        // Insert at a position smaller than elements in the model
        QCOMPARE(model.insertItem(1, TestType{.value = 4}), 1);
        QCOMPARE(model.data(model.index(1), TestModel::Role1).value<qint32>(), 4);

        // Insert at a position bigger than elements in the model
        QCOMPARE(model.insertItem(100, TestType{.value = 4}), 4);
        QCOMPARE(model.data(model.index(4), TestModel::Role1).value<qint32>(), 4);
    }

    void testRemoveOperations()
    {
        TestData fix;
        TestModel model(fix.data);

        // Delete a valid item by row
        QCOMPARE(model.rowCount(QModelIndex{}), 2);
        QVERIFY(model.removeItem(0));
        QCOMPARE(model.rowCount(QModelIndex{}), 1);

        // Reset to original for next checks
        QVERIFY(model.insertItem(0, TestType{.value = 1}) >= 0);
        QCOMPARE(model.rowCount(QModelIndex{}), 2);

        // Delete an invalid item by row
        QVERIFY(!model.removeItem(100));
        QCOMPARE(model.rowCount(QModelIndex{}), 2);

        // Delete a valid item by item
        QVERIFY(model.removeItem(TestType{.value = 1}));
        QCOMPARE(model.rowCount(QModelIndex{}), 1);

        // Delete an invalid item by item
        QVERIFY(!model.removeItem(TestType{.value = 100}));
        QCOMPARE(model.rowCount(QModelIndex{}), 1);
    }

    void testUpdateOperations()
    {
        TestData fix;
        TestModel model(fix.data);

        // Update a valid item by row
        QVERIFY(model.updateItem(1, TestType{.value = 10}));
        QCOMPARE(model.data(model.index(1), TestModel::Role1).value<qint32>(), 10);

        // Update an invalid item by row
        QVERIFY(!model.updateItem(100, TestType{.value = 10}));
        QCOMPARE(model.data(model.index(1), TestModel::Role1).value<qint32>(), 10);

        // Update a valid item by item
        QVERIFY(model.updateItem(TestType{.value = 10}, TestType{.value = 20}));
        QCOMPARE(model.data(model.index(1), TestModel::Role1).value<qint32>(), 20);

        // Update an invalid item by item
        QVERIFY(!model.updateItem(TestType{.value = 100}, TestType{.value = 10}));
        QCOMPARE(model.data(model.index(0), TestModel::Role1).value<qint32>(), 1);
    }

    void testGetElementReturnsPointerOrNullopt()
    {
        {
            TestModel model;
            QCOMPARE(model.getElement(0U), std::nullopt);
        }

        {
            TestData fix;
            TestModel model(fix.data);
            constexpr std::size_t invalidIndex = 2U;
            QCOMPARE(model.getElement(invalidIndex), std::nullopt);
        }

        {
            TestData fix;
            TestModel model(fix.data);
            constexpr std::size_t validIndex = 1U;
            auto* element = model.getElement(validIndex).value_or(nullptr);
            QVERIFY(element != nullptr);
            QCOMPARE(*element, fix.data[1]);
        }
    }
};

} // namespace RapidAndroid::Common

QTEST_MAIN(RapidAndroid::Common::GenericListModelTest)
#include "GenericListModelTest.moc"
