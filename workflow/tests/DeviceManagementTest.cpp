// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "Common/DeviceSettingsListModel.hpp"
#include <Common/DeviceSettings.hpp>
#include <Common/Private/SettingsKeys.hpp>
#include <QHostAddress>
#include <QSignalSpy>
#include <QString>
#include <QTest>
#include <QVariant>
#include <TestHelper/SettingsMemoryBackend.hpp>
#include <Workflow/DeviceManagement.hpp>

using namespace RapidAndroid::TestHelper;
using namespace RapidAndroid::Workflow;
using namespace RapidAndroid::Common;

namespace RapidAndroid::Workflow::Tests
{

class DeviceManagementTest : public QObject
{
    Q_OBJECT

    Common::DeviceSettings createDeviceSettings(std::size_t deviceIndex)
    {
        return {.name = QString{"Rapid%1"}.arg(deviceIndex),
                .ip = QHostAddress{"192.168.0.1"},
                .port = 27018,
                .defaultDevice = false};
    }

private Q_SLOTS:
    void testModelIsProvided()
    {
        auto settingsMemoryBackend = SettingsMemoryBackend{};
        DeviceManagement deviceManagement{std::addressof(settingsMemoryBackend)};

        // Give the model a valid model
        auto* model = deviceManagement.getModel();
        QVERIFY(model != nullptr);

        // Property gives a valid model
        QVERIFY(deviceManagement.property("model").value<QAbstractItemModel*>() != nullptr);
    }

    void testDefaultDeviceCreationWhenEmpty()
    {
        auto const device = DeviceSettings{.name = QStringLiteral("Rapid"),
                                           .ip = QHostAddress{QStringLiteral("192.168.1.1")},
                                           .port = 27015,
                                           .defaultDevice = false};
        auto settingsMemoryBackend = SettingsMemoryBackend{};
        auto deviceManagement = DeviceManagement{std::addressof(settingsMemoryBackend)};

        auto* model = deviceManagement.getModel();
        QCOMPARE(model->rowCount(), 1);
        auto laptimerSettings =
            model->data(model->index(0, 0), DeviceSettingsListModel::Laptimer).value<DeviceSettings>();
        QCOMPARE(laptimerSettings.name, QStringLiteral("Rapid"));
        QCOMPARE(laptimerSettings.ip, QHostAddress("192.168.1.1"));
        QCOMPARE(laptimerSettings.port, 27018);
        QCOMPARE(laptimerSettings.defaultDevice, true);
    }

    void testStoreDevice()
    {
        auto const device = DeviceSettings{.name = QStringLiteral("Loopi"),
                                           .ip = QHostAddress{QStringLiteral("192.168.1.2")},
                                           .port = 27015,
                                           .defaultDevice = false};
        auto settingsMemoryBackend = SettingsMemoryBackend{};
        auto deviceManagement = DeviceManagement{std::addressof(settingsMemoryBackend)};
        QVERIFY(deviceManagement.store(device));
        QCOMPARE(deviceManagement.getModel()->rowCount(), 2);
    }

    void testStoreDeviceFailure()
    {
        auto settingsMemoryBackend = SettingsMemoryBackend{};
        auto deviceManagement = DeviceManagement{std::addressof(settingsMemoryBackend)};
        auto settings = createDeviceSettings(0);
        settingsMemoryBackend.enableAlwaysFailureMode();
        QVERIFY(!deviceManagement.store(settings));
        QCOMPARE(deviceManagement.getModel()->rowCount(), 1);
    }

    void testStoreDuplicateDevice()
    {
        auto settingsMemoryBackend = SettingsMemoryBackend{};
        auto deviceManagement = DeviceManagement{std::addressof(settingsMemoryBackend)};
        QVERIFY(deviceManagement.store(createDeviceSettings(0)));
        QVERIFY(!deviceManagement.store(createDeviceSettings(0)));
    }

    void testDeleteDevice()
    {
        auto settingsMemoryBackend = SettingsMemoryBackend{};
        auto deviceManagement = DeviceManagement{std::addressof(settingsMemoryBackend)};
        QVERIFY(deviceManagement.remove(DeviceSettings{.name = QStringLiteral("Rapid"),
                                                       .ip = QHostAddress{"192.168.1.1"},
                                                       .port = 27018,
                                                       .defaultDevice = true}));
        QCOMPARE(deviceManagement.getModel()->rowCount(), 0);
    }

    void testDeleteNotExistingDevice()
    {
        auto settingsMemoryBackend = SettingsMemoryBackend{};
        auto deviceManagement = DeviceManagement{std::addressof(settingsMemoryBackend)};
        QVERIFY(!deviceManagement.remove(DeviceSettings{}));
        QCOMPARE(deviceManagement.getModel()->rowCount(), 1);
    }

    void testUpdateDevice()
    {
        auto settingsMemoryBackend = SettingsMemoryBackend{};
        auto deviceManagement = DeviceManagement{std::addressof(settingsMemoryBackend)};
        auto updatedDevice = DeviceSettings{.name = QStringLiteral("Rapid"),
                                            .ip = QHostAddress{"192.168.1.1"},
                                            .port = 27019,
                                            .defaultDevice = true};
        auto oldDevice = DeviceSettings{.name = QStringLiteral("Rapid"),
                                        .ip = QHostAddress{"192.168.1.1"},
                                        .port = 27018,
                                        .defaultDevice = true};
        QVERIFY(deviceManagement.update(oldDevice, updatedDevice));
    }

    void testUpdateNotExistingDevice()
    {
        auto settingsMemoryBackend = SettingsMemoryBackend{};
        auto deviceManagement = DeviceManagement{std::addressof(settingsMemoryBackend)};
        QVERIFY(!deviceManagement.update(DeviceSettings{}, DeviceSettings{}));
    }

    void testEnabledDeviceBehavior()
    {
        auto settingsMemoryBackend = SettingsMemoryBackend{};
        auto deviceManagement = DeviceManagement{std::addressof(settingsMemoryBackend)};
        auto device = DeviceSettings{.name = QStringLiteral("Rapid"),
                                     .ip = QHostAddress{"192.168.1.1"},
                                     .port = 27018,
                                     .defaultDevice = true};
        QSignalSpy activeLaptimerSpy{&deviceManagement, &DeviceManagement::activeLaptimerChanged};
        QVERIFY(deviceManagement.enable(device));
        auto model = deviceManagement.getModel();
        QVERIFY(
            model->data(model->index(0, 0), DeviceSettingsListModel::Laptimer).value<DeviceSettings>().defaultDevice);
        QCOMPARE(deviceManagement.property("activeLaptimer").value<DeviceSettings>(), device);
        QCOMPARE(activeLaptimerSpy.size(), 1);
    }

    void testDefaultDeviceWhenNoneEnabled()
    {
        auto settingsMemoryBackend = SettingsMemoryBackend{};
        auto deviceManagement = DeviceManagement{std::addressof(settingsMemoryBackend)};
        auto device = DeviceSettings{.name = QStringLiteral("Rapid"),
                                     .ip = QHostAddress{"192.168.1.1"},
                                     .port = 27018,
                                     .defaultDevice = true};
        QCOMPARE(deviceManagement.property("activeLaptimer").value<DeviceSettings>(), device);
    }

    void testEnabledDeviceDeleted()
    {
        auto settingsMemoryBackend = SettingsMemoryBackend{};
        auto deviceManagement = DeviceManagement{std::addressof(settingsMemoryBackend)};
        auto enabledDevice = DeviceSettings{.name = QStringLiteral("Rapid"),
                                            .ip = QHostAddress{"192.168.1.1"},
                                            .port = 27018,
                                            .defaultDevice = true};
        auto* const model = deviceManagement.getModel();
        QVERIFY(deviceManagement.enable(enabledDevice));
        enabledDevice = model->data(model->index(1, 0), DeviceSettingsListModel::Laptimer).value<DeviceSettings>();
        deviceManagement.remove(enabledDevice);
        auto const isDeviceEnabled =
            model->data(model->index(0, 0), DeviceSettingsListModel::Laptimer).value<DeviceSettings>().defaultDevice;
        QVERIFY(isDeviceEnabled);
    }
};

} // namespace RapidAndroid::Workflow::Tests

QTEST_MAIN(RapidAndroid::Workflow::Tests::DeviceManagementTest)
#include "DeviceManagementTest.moc"
