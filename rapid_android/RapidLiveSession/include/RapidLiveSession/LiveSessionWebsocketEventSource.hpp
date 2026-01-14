// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_RAPIDLIVESESSION_LIVESESSIONWEBSOCKETEVENTSOURCE_HPP
#define RAPIDANDROID_RAPIDLIVESESSION_LIVESESSIONWEBSOCKETEVENTSOURCE_HPP

#include <QDebug>
#include <QFutureWatcher>
#include <QLoggingCategory>
#include <QObject>
#include <QWebSocket>
#include <RapidLiveSession/ILiveSessionEventDeserializer.hpp>
#include <Workflow/ILiveSessionEventSource.hpp>
#include <memory>
#include <unordered_map>

namespace RapidAndroid::RapidLiveSession
{

/**
 * @brief Concept to ensure that the LiveSessionEventDeserializerType
 *        conforms to the ILiveSessionEventDeserializer interface.
 */
QLoggingCategory const& rljeLog();

// Base class to allow forward declaration with Q_OBJECT macro
class LiveSessionWebsocketEventSourceBase : public QObject
{
    Q_OBJECT
public:
    Q_DISABLE_COPY_MOVE(LiveSessionWebsocketEventSourceBase)

    ~LiveSessionWebsocketEventSourceBase() override = default;

Q_SIGNALS:
    /**
     * @brief Emitted when a new laptime has started.
     */
    void laptimeStarted();

    /**
     * @brief Emitted when the current laptime has changed.
     * @param event The laptime event containing the updated laptime information.
     */
    void currentLaptimeChanged(RapidAndroid::Common::LaptimeEvent const& event);

    /**
     * @brief Emitted when the current laptime has finished.
     * @param event The lap finished event containing the final laptime information.
     */
    void laptimeFinished(RapidAndroid::Common::LapFinishedEvent const& event);

    /**
     * @brief Emitted when a sector of the current lap has been completed.
     * @param event The lap sector event containing the sector information.
     */
    void lapSectorFinished(RapidAndroid::Common::LapSectorEvent const& event);

protected:
    LiveSessionWebsocketEventSourceBase() = default;
};

/**
 * @brief A LiveSession event source that connects to a WebSocket server to receive live session events.
 *
 * This class uses a WebSocket connection to receive live session events in real-time.
 * It deserializes the received messages using the provided LiveSessionEventDeserializerType
 * and emits signals for different types of events.
 *
 * @tparam LiveSessionEventDeserializerType The type of the event deserializer used to deserialize incoming messages.
 */
template <typename LiveSessionEventDeserializerType>
    requires LiveSessionEventDeserializerConcept<LiveSessionEventDeserializerType>
class LiveSessionWebsocketEventSource : public LiveSessionWebsocketEventSourceBase
{
public:
    Q_DISABLE_COPY_MOVE(LiveSessionWebsocketEventSource)

    /**
     * @brief Constructs a LiveSessionWebsocketEventSource with the given event deserializer.
     * @param deserializer A pointer to the event deserializer used to deserialize incoming messages.
     */
    LiveSessionWebsocketEventSource(LiveSessionEventDeserializerType* deserializer)
        : mEventDeserializer{deserializer}
    {
    }

    /**
     * @brief Destructor that closes the WebSocket connection if it is valid.
     */
    ~LiveSessionWebsocketEventSource() override
    {
        qDebug() << "Destroying LiveSessionWebsocketEventSource";
        if (mWebSocket != nullptr and mWebSocket->isValid()) {
            mWebSocket->close();
        }
    }

    /**
     * @brief Sets the configuration for the laptimer by establishing a WebSocket connection.
     * @param deviceSettings The device settings containing the IP address and port for the WebSocket server.
     */
    void setLaptimerConfig(Common::DeviceSettings const& deviceSettings)
    {
        close();
        mDeviceSettings = deviceSettings;
        mWebSocket = std::make_unique<QWebSocket>();
        auto url = QUrl{QString{"ws://%1:%2/v1/live_session"}.arg(deviceSettings.ip.toString(),
                                                                  QString::number(deviceSettings.port))};
        mWebSocket->open(QUrl{url});
        connect(mWebSocket.get(), &QWebSocket::connected, this, &LiveSessionWebsocketEventSource::handleConnected);
        qCInfo(rljeLog) << "Connecting to WebSocket at" << url.toString();
    }

private:
    /**
     * @brief Closes the WebSocket connection if it is valid.
     */
    void close()
    {
        if (mWebSocket != nullptr and mWebSocket->isValid()) {
            mWebSocket->close();
        }
    }

    /**
     * @brief Handles the WebSocket connection established event.
     */
    void handleConnected()
    {
        connect(mWebSocket.get(),
                &QWebSocket::textMessageReceived,
                this,
                &LiveSessionWebsocketEventSource::handleTextMessage);
        connect(mWebSocket.get(), &QWebSocket::disconnected, this, [this]() {
            qCInfo(rljeLog) << "WebSocket disconnected from device" << mDeviceSettings;
            close();
        });
        qCInfo(rljeLog) << "WebSocket connected to device" << mDeviceSettings;
    }

    /**
     * @brief Handles WebSocket connection errors.
     * @param error The socket error that occurred.
     */
    void handleConnectionError(QAbstractSocket::SocketError error)
    {
        qCCritical(rljeLog) << "Failed to connect to" << mDeviceSettings << "WebSocket connection error:" << error;
    }

    /**
     * @brief Handles incoming text messages from the WebSocket.
     * @param message The received text message.
     */
    void handleTextMessage(QString const& message)
    {
        auto const deserializeTask = mEventDeserializer->deserialize(message.toUtf8());
        auto taskWatcher = std::make_unique<QFutureWatcher<std::optional<Event>>>();
        taskWatcher->setFuture(deserializeTask);
        connect(taskWatcher.get(),
                &QFutureWatcher<std::optional<Event>>::finished,
                this,
                &LiveSessionWebsocketEventSource::handleDeserializationFinished);
        mDeserializeTasks.emplace(taskWatcher.get(), std::move(taskWatcher));
    }

    /**
     * @brief Handles the completion of the deserialization task.
     */
    void handleDeserializationFinished()
    {
        auto it = mDeserializeTasks.find(sender());
        if (it == mDeserializeTasks.end()) {
            return;
        }
        auto optionalEvent = it->second->result();
        if (not optionalEvent.has_value()) {
            return;
        }

        std::visit(
            [this](auto const& event) {
                using T = std::decay_t<decltype(event)>;
                if constexpr (std::is_same_v<T, Common::LaptimeEvent>) {
                    Q_EMIT currentLaptimeChanged(event);
                }
            },
            optionalEvent.value());
        mDeserializeTasks.erase(it);
    }

private:
private:
    LiveSessionEventDeserializerType* mEventDeserializer;
    std::unique_ptr<QWebSocket> mWebSocket;
    std::unordered_map<QObject*, std::unique_ptr<QFutureWatcher<std::optional<Event>>>> mDeserializeTasks;
    Common::DeviceSettings mDeviceSettings;
};

} // namespace RapidAndroid::RapidLiveSession

#endif // RAPIDANDROID_RAPIDLIVESESSION_LIVESESSIONWEBSOCKETEVENTSOURCE_HPP
