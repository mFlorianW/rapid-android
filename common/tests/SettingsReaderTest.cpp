// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include <Common/Private/SettingsKeys.hpp>
#include <Common/SettingsReader.hpp>
#include <QHostAddress>
#include <QTest>
#include <TestHelper/SettingsMemoryBackend.hpp>

using namespace RapidAndroid::Common;
using namespace RapidAndroid::TestHelper;

namespace RapidAndroid::Common::Tests
{

class SettingsReaderTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testLoadDeviceSettings()
    {
        SettingsMemoryBackend settingsBackend;
        constexpr auto devices = int{2};

        QVERIFY(settingsBackend.storeValue(DeviceSettingsSize, devices));
        for (int i = 0; i < devices; ++i) {
            QVERIFY(settingsBackend.storeValue(DeviceSettingsName.toString().arg(i), QStringLiteral("Rapid%1").arg(i)));
            QVERIFY(settingsBackend.storeValue(DeviceSettingsIp.toString().arg(i),
                                               QHostAddress{QHostAddress::LocalHost}.toString()));
            QVERIFY(settingsBackend.storeValue(DeviceSettingsPort.toString().arg(i), 80));
            QVERIFY(settingsBackend.storeValue(DeviceSettingsDef.toString().arg(i), false));
        }

        SettingsReader settings(&settingsBackend);
        auto const deviceSettings = settings.getDeviceSettings();

        QCOMPARE(deviceSettings.size(), devices);
        for (int i = 0; i < devices; ++i) {
            QCOMPARE(deviceSettings.at(i).name, QStringLiteral("Rapid%1").arg(i));
            QCOMPARE(deviceSettings.at(i).ip, QHostAddress{QHostAddress::LocalHost});
            QCOMPARE(deviceSettings.at(i).port, 80);
            QCOMPARE(deviceSettings.at(i).defaultDevice, false);
        }
    }
};

} // namespace RapidAndroid::Common::Tests

QTEST_MAIN(RapidAndroid::Common::Tests::SettingsReaderTest)
#include "SettingsReaderTest.moc"
