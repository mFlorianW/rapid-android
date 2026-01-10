// SPDX-FileCopyrightText: 2025, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include <Common/Private/SettingsKeys.hpp>
#include <Common/SettingsReader.hpp>
#include <Common/SettingsWriter.hpp>
#include <QHostAddress>
#include <QTest>
#include <TestHelper/SettingsMemoryBackend.hpp>

using namespace RapidAndroid::Common;
using namespace RapidAndroid::TestHelper;

namespace RapidAndroid::Common::Tests
{

class SettingsWriterTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testStoreDeviceSettings()
    {
        SettingsMemoryBackend backend;
        SettingsWriter writer(&backend);
        auto deviceSettings = QList<DeviceSettings>{
            DeviceSettings{
                .name = QStringLiteral("Rapid0"),
                .ip = QHostAddress{QHostAddress::LocalHost},
                .port = 80,
                .defaultDevice = false,
            },
            DeviceSettings{
                .name = QStringLiteral("Rapid1"),
                .ip = QHostAddress{QHostAddress::LocalHost},
                .port = 80,
                .defaultDevice = false,
            },
        };

        QVERIFY(writer.storeDeviceSettings(deviceSettings));
        for (int i = 0; i < deviceSettings.size(); ++i) {
            // clang-format off
            QCOMPARE(backend.getValue(DeviceSettingsName.toString().arg(i)), QStringLiteral("Rapid%1").arg(i));
            QCOMPARE(backend.getValue(DeviceSettingsIp.toString().arg(i)),QHostAddress{QHostAddress::LocalHost}.toString());
            QCOMPARE(backend.getValue(DeviceSettingsPort.toString().arg(i)), 80);
            QCOMPARE(backend.getValue(DeviceSettingsDef.toString().arg(i)), false);
            // clang-format on
        }
    }
};

} // namespace RapidAndroid::Common::Tests

QTEST_MAIN(RapidAndroid::Common::Tests::SettingsWriterTest)
#include "SettingsWriterTest.moc"
