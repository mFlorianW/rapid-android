// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include <QJsonObject>
#include <QObject>
#include <QSignalSpy>
#include <QTest>
#include <QWebSocketServer>
#include <QtConcurrentRun>
#include <RapidLiveSession/LiveSessionWebsocketEventSource.hpp>
#include <TestHelper/FakeLaptimer.hpp>
#include <TestHelper/LaptimerEvents.hpp>
#include <TestHelper/LiveSessionEventDeserializer.hpp>

namespace RapidAndroid::RapidLiveSession::Tests
{
using Lses = LiveSessionWebsocketEventSource<TestHelper::LiveSessionEventDeserializer>;
class LiveSessionWebsocketEventSourceTest : public QObject
{
    Q_OBJECT

Q_SIGNALS:
    void webSocketClientConnected();
    void webSocketClientDisconnected();

private:
    std::unique_ptr<QWebSocketServer> mWebSocketServer;
    std::unique_ptr<QWebSocket> mWebSocketClient;

    TestHelper::LiveSessionEventDeserializer deserializer;
    std::unique_ptr<Lses> liveSessionEventSource;

private:
    void waitForConnect()
    {
        auto connectionSpy = QSignalSpy{this, &LiveSessionWebsocketEventSourceTest::webSocketClientConnected};
        QTRY_COMPARE_WITH_TIMEOUT(connectionSpy.count(), 1, std::chrono::seconds{1});
    }

    Common::DeviceSettings getDeviceSettings() const
    {
        Common::DeviceSettings settings;
        settings.ip = QHostAddress::LocalHost;
        settings.port = mWebSocketServer->serverPort();
        return settings;
    }

    void sendEventToClient(QString const& message)
    {
        QVERIFY(mWebSocketClient != nullptr);
        mWebSocketClient->sendTextMessage(message);
    }
private Q_SLOTS:
    void init()
    {
        mWebSocketServer =
            std::make_unique<QWebSocketServer>(QStringLiteral("TestServer"), QWebSocketServer::NonSecureMode);
        QVERIFY(mWebSocketServer->listen(QHostAddress::LocalHost, 0));
        connect(mWebSocketServer.get(), &QWebSocketServer::newConnection, this, [this]() {
            mWebSocketClient = std::unique_ptr<QWebSocket>(mWebSocketServer->nextPendingConnection());
            connect(mWebSocketClient.get(),
                    &QWebSocket::disconnected,
                    this,
                    &LiveSessionWebsocketEventSourceTest::webSocketClientDisconnected);
            Q_EMIT webSocketClientConnected();
        });
        liveSessionEventSource = std::make_unique<Lses>(&deserializer);
        liveSessionEventSource->setLaptimerConfig(getDeviceSettings());
        waitForConnect();
    }

    void cleanup()
    {
        mWebSocketServer->close();
        if (mWebSocketClient != nullptr) {
            mWebSocketClient->close();
        }
        QTest::qWait(0);
        QVERIFY(testing::Mock::VerifyAndClearExpectations(&deserializer));
    }

    void testCurrentLaptimeUpdates()
    {
        auto laptimeChangedSpy = QSignalSpy{liveSessionEventSource.get(), &Lses::currentLaptimeChanged};
        auto laptime = QTime{0, 1, 30, 500};
        auto jsonEvent = TestHelper::laptimeEventJson(laptime);
        auto laptimeEvent = Common::LaptimeEvent{.laptime = laptime};

        EXPECT_CALL(deserializer, deserialize(testing::StrEq(jsonEvent.toStdString())))
            .WillOnce(testing::Return(QtConcurrent::run([&laptimeEvent]() -> std::optional<RapidLiveSession::Event> {
                return laptimeEvent;
            })));

        sendEventToClient(jsonEvent);
        QTRY_COMPARE_WITH_TIMEOUT(laptimeChangedSpy.count(), 1, std::chrono::seconds{1});
        auto receivedEvent = laptimeChangedSpy.takeFirst().at(0).value<Common::LaptimeEvent>();
        QCOMPARE(receivedEvent.laptime, laptime);
    }
};

} // namespace RapidAndroid::RapidLiveSession::Tests

QTEST_MAIN(RapidAndroid::RapidLiveSession::Tests::LiveSessionWebsocketEventSourceTest)
#include "LiveSessionWebsocketEventSourceTest.moc"
