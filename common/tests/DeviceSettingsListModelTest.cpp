// SPDX-FileCopyrightText: 2025, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include <Common/DeviceSettingsListModel.hpp>
#include <QByteArray>
#include <QHash>
#include <QHostAddress>
#include <QString>
#include <QTest>
#include <QVariant>

using namespace RapidAndroid::Common;

class DeviceSettingsListModelTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testRoleNames()
    {
        DeviceSettingsListModel model;
        QHash<int, QByteArray> expected{{Qt::DisplayRole, "display"}, {DeviceSettingsListModel::Laptimer, "laptimer"}};
        QCOMPARE(model.roleNames(), expected);
    }

    void testDataValidIndex()
    {
        DeviceSettingsListModel model;
        DeviceSettings expectedDevice{.name = "Device1", .ip = QHostAddress{QHostAddress::LocalHost}, .port = 10};
        QCOMPARE(model.insertItem(expectedDevice), 0);

        QModelIndex idx = model.index(0);
        QCOMPARE(qvariant_cast<QString>(model.data(idx, Qt::DisplayRole)), QStringLiteral("Device1"));
        QCOMPARE(qvariant_cast<DeviceSettings>(model.data(idx, DeviceSettingsListModel::Laptimer)), expectedDevice);
    }

    void testDataInvalidIndex()
    {
        DeviceSettingsListModel model;
        DeviceSettings expectedDevice{.name = "Device1", .ip = QHostAddress{QHostAddress::LocalHost}, .port = 10};
        QCOMPARE(model.insertItem(expectedDevice), 0);

        QModelIndex invalidIdx = model.index(1);
        QCOMPARE(model.data(invalidIdx, Qt::DisplayRole), QVariant{});
        QCOMPARE(model.data(invalidIdx, DeviceSettingsListModel::Laptimer), QVariant{});
    }
};

QTEST_MAIN(DeviceSettingsListModelTest)
#include "DeviceSettingsListModelTest.moc"
