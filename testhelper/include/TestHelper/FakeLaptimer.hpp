// SPDX-FileCopyrightText: 2025, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_TESTHELPER_FAKELAPTIMER_HPP
#define RAPIDANDROID_TESTHELPER_FAKELAPTIMER_HPP

#include <Common/DeviceSettings.hpp>
#include <Common/Session.hpp>
#include <Common/SessionInfo.hpp>
#include <QHttpServer>
#include <QObject>

namespace RapidAndroid::TestHelper
{

/**
 * A fake laptimer device that can be used in tests.
 *
 * It starts a local HTTP server that can be used to simulate a laptimer device.
 *
 * The laptimer implements the REST API as defined in:
 *  https://github.com/mFlorianW/rapid-rusty/blob/main/docs/main.md
 *
 *  The device always returns the same data on the REST resources.
 */
class FakeLaptimer : public QObject
{
    Q_OBJECT
public:
    Q_DISABLE_COPY_MOVE(FakeLaptimer)

    FakeLaptimer();
    ~FakeLaptimer() override;

    Common::DeviceSettings deviceSettings() const;

    Common::SessionInfo getSessionInfo1() const;
    Common::SessionInfo getSessionInfo2() const;

    std::unique_ptr<Common::Session> getSession1() const;

private:
    QHttpServer mServer;
};

} // namespace RapidAndroid::TestHelper

#endif // RAPIDANDROID_TESTHELPER_FAKEDEVICE_HPP
