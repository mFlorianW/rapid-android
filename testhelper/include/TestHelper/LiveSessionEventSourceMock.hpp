// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include <QObject>
#include <Workflow/ILiveSessionEventSource.hpp>
#include <gmock/gmock.h>

namespace RapidAndroid::TestHelper
{

class LiveSessionEventSourceMock : public QObject
{
    Q_OBJECT
public:
    Q_DISABLE_COPY_MOVE(LiveSessionEventSourceMock)

    LiveSessionEventSourceMock() = default;
    ~LiveSessionEventSourceMock() override = default;

    MOCK_METHOD(void, setLaptimerConfig, (Common::DeviceSettings const& settings));

Q_SIGNALS:
    void laptimeStarted();
    void currentLaptimeChanged(RapidAndroid::Common::LaptimeEvent const& event);
    void laptimeFinished(RapidAndroid::Common::LapFinishedEvent const& event);
    void lapSectorFinished(RapidAndroid::Common::LapSectorEvent const& event);
    void currentSessionEventReceived(RapidAndroid::Common::CurrentSessionEvent const& event);
};

} // namespace RapidAndroid::TestHelper
