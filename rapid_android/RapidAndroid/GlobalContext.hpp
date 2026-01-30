// SPDX-FileCopyrightText: 2025, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_GLOBALCONTEXT_HPP
#define RAPIDANDROID_GLOBALCONTEXT_HPP

#include <Common/DeviceSettings.hpp>
#include <QObject>
#include <QtQmlIntegration/qqmlintegration.h>
#include <RapidLaptimer/FileSettingsBackend.hpp>
#include <RapidLiveSession/LiveSessionJsonDeserializer.hpp>
#include <RapidLiveSession/LiveSessionWebsocketEventSource.hpp>
#include <RapidSession/FilesystemStorage.hpp>
#include <RapidSession/SessionJsonDeserializer.hpp>
#include <RapidSession/SessionJsonSerializer.hpp>
#include <Workflow/DeviceManagement.hpp>
#include <Workflow/HttpDeviceSessionManagement.hpp>
#include <Workflow/LiveSessionManagement.hpp>
#include <Workflow/LocalSessionManagement.hpp>
#include <Workflow/SessionAnalyzer.hpp>
#include <memory>

namespace RapidAndroid
{

/**
 * @brief Global context for the RapidAndroid application.
 *
 * Exposes application-wide services and workflows to C++ and QML.
 * Registered as a QML element and singleton to be accessible as a single shared instance from QML.
 */
class GlobalContext : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    /**
     * @property Rapid::Workflow::Qt::DeviceManagement*
     *
     * Gives the DeviceManagement workflow for managing the laptimer.
     */
    Q_PROPERTY(RapidAndroid::Workflow::IDeviceManagement* deviceManagement READ getDeviceManagement CONSTANT)

    /**
     * @property RapidAndroid::Workflow::IDeviceSessionManagement*
     *
     * Gives the DeviceSessionManagement workflow for managing device sessions.
     */
    Q_PROPERTY(RapidAndroid::Workflow::IDeviceSessionManagement* deviceSessionManagement READ getDeviceSessionManagement
                   CONSTANT)

    /**
     * @property RapidAndroid::Workflow::ILocalSessionManagement*
     *
     * Gives the LocalSessionManagement workflow for managing local sessions.
     */
    Q_PROPERTY(
        RapidAndroid::Workflow::ILocalSessionManagement* localSessionManagement READ getLocalSessionManagement CONSTANT)

    /**
     * @property RapidAndroid::Workflow::ISessionAnalyzer*
     *
     * Gives the SessionAnalyzer for analyzing sessions.
     */
    Q_PROPERTY(RapidAndroid::Workflow::ISessionAnalyzer* sessionAnalyzer READ getSessionAnalyzer CONSTANT)

    /**
     * @property RapidAndroid::Workflow::ILiveSessionManagement*
     *
     * Gives the LiveSessionManagement workflow for managing live sessions.
     */
    Q_PROPERTY(
        RapidAndroid::Workflow::ILiveSessionManagement* liveSessionManagement READ getLiveSessionManagement CONSTANT)

public:
    Q_DISABLE_COPY_MOVE(GlobalContext)

    /**
     * @brief Construct the global context.
     */
    GlobalContext();

    /**
     * @brief Destroy the global context.
     */
    ~GlobalContext() override;

    /**
     * @brief Get the device-management workflow.
     * @return Non-null pointer owned by GlobalContext. Do not delete.
     */
    Workflow::DeviceManagement* getDeviceManagement() const noexcept;

    /**
     * @brief Get the device-session-management workflow.
     * @return Non-null pointer owned by GlobalContext. Do not delete.
     */
    RapidAndroid::Workflow::IDeviceSessionManagement* getDeviceSessionManagement() const noexcept;

    /**
     * @brief Get the local-session-management workflow.
     * @return Non-null pointer owned by GlobalContext. Do not delete.
     */
    Workflow::ILocalSessionManagement* getLocalSessionManagement() noexcept;

    /**
     * @brief Get the session-analyzer workflow.
     * @return Non-null pointer owned by GlobalContext. Do not delete.
     */
    Workflow::ISessionAnalyzer* getSessionAnalyzer() noexcept;

    /**
     * @brief Get the live-session-management workflow.
     * @return Non-null pointer owned by GlobalContext. Do not delete.
     */
    Workflow::ILiveSessionManagement* getLiveSessionManagement() noexcept;

private:
    FileSettingsBackend mSettingsBackend;
    std::unique_ptr<Workflow::DeviceManagement> mDeviceManagement;

    using Storage = Session::FilesystemStorage<Session::SessionJsonSerializer, Session::SessionJsonDeserializer>;
    Session::SessionJsonSerializer mSessionSerializer;
    Session::SessionJsonDeserializer mSessionJsonDeserializer;
    Storage mSessionStorage;

    using DeviceSessionMgmt = Workflow::HttpDeviceSessionManagement<Session::SessionJsonDeserializer, Storage>;
    RapidAndroid::Session::SessionJsonDeserializer mSessionDeserializer;
    std::unique_ptr<DeviceSessionMgmt> mDeviceSessionManagement;

    using LocalSessionMgmt = Workflow::LocalSessionManagement<Storage>;
    std::unique_ptr<LocalSessionMgmt> mLocalSessionManagement;

    std::unique_ptr<Workflow::SessionAnalyzer> mSessionAnalyzer;

    RapidLiveSession::LiveSessionJsonDeserializer mLiveSessionJsonDeserializer;
    using LiveSessionEventSource =
        RapidLiveSession::LiveSessionWebsocketEventSource<RapidLiveSession::LiveSessionJsonDeserializer>;
    std::unique_ptr<RapidLiveSession::LiveSessionWebsocketEventSource<RapidLiveSession::LiveSessionJsonDeserializer>>
        mLiveSessionEventSource;
    using LiveSessionMgmt = Workflow::LiveSessionManagement<LiveSessionEventSource>;
    std::unique_ptr<LiveSessionMgmt> mLiveSessionManagement;
};

struct Wf
{
    Q_GADGET
    QML_FOREIGN(RapidAndroid::Workflow::IDeviceSessionManagement)
    QML_NAMED_ELEMENT(DeviceSessionManagement)
    QML_UNCREATABLE("Interface class - an instance of it is provided by a global context.")
};

} // namespace RapidAndroid

#endif // RAPIDANDROID_GLOBALCONTEXT_HPP
