// SPDX-FileCopyrightText: 2025, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_WORKFLOW_IDEVICEMANAGEMENT
#define RAPIDANDROID_WORKFLOW_IDEVICEMANAGEMENT

#include <Common/DeviceSettings.hpp>
#include <QAbstractItemModel>
#include <QObject>

namespace RapidAndroid::Workflow
{

/**
 * @brief Interface to manage the devies inside the application
 *
 * @details This interface defines the functions to manage devices (laptimer) inside the application
 *          Those functions are loading, saving, displaying and editing.
 */
class IDeviceManagement : public QObject
{
    Q_OBJECT

    /**
     * @property QAbstractItemModel const* const
     *
     * Gives the model for displaying the configured device (laptimer).
     */
    Q_PROPERTY(QAbstractItemModel const* const model READ getModel CONSTANT FINAL)

    /**
     * @property Rapid::Commont::Qt::DeviceSettings
     *
     * Gives the active laptimer to interact with.
     */
    Q_PROPERTY(RapidAndroid::Common::DeviceSettings activeLaptimer READ getActiveLaptimer NOTIFY activeLaptimerChanged)

public:
    /**
     * Default destructor
     */
    ~IDeviceManagement() override = default;

    /**
     * Disabled copy constructor
     */
    IDeviceManagement(IDeviceManagement const&) = delete;

    /**
     * Disablbed copy assignment operator
     */
    IDeviceManagement& operator=(IDeviceManagement const&) = delete;

    /**
     * Disabled move constructor
     */
    IDeviceManagement(IDeviceManagement&&) noexcept = delete;

    /**
     * Disablbed move assignment operator
     */
    IDeviceManagement& operator=(IDeviceManagement&&) noexcept = delete;

    /**
     * @brief Gives a model for displaying the laptimer in the UI
     *
     * @details The ownership of the returned pointer belongs to the DeviceManagement instance.
     *          The liftime of the pointer is as long as the DeviceManagement class exists.
     *
     * @return A QAbstractItemModel* pointer to display in the UI.
     */
    [[nodiscard]] virtual QAbstractItemModel const* const getModel() const noexcept = 0;

    /**
     * @brief Gives the active laptimer
     *
     * @details The active laptimer is either activated by the user or is the first device of the device list.
     *          This is the laptimer that is used for the interaction e.g. session download or live session view.
     *
     * @return The laptimer that shall be used for the interaction.
     */
    [[nodiscard]] virtual Common::DeviceSettings getActiveLaptimer() const noexcept = 0;

    /**
     * @brief Stores a new laptimers
     *
     * @details The store function also updates the model on success.
     *
     * @param device The laptimer that shall be stored.
     *
     * @return True the laptimer is successful stored otherwise false
     */
    virtual Q_INVOKABLE bool store(RapidAndroid::Common::DeviceSettings const& device) noexcept = 0;

    /**
     * @brief Removes a laptimer
     *
     * @details This function also updates the model on success.
     *
     * @param device The laptimer that shall be removed.
     *
     * @return True the laptimer is successful stored otherwise false
     */
    virtual Q_INVOKABLE bool remove(RapidAndroid::Common::DeviceSettings const& device) noexcept = 0;

    /**
     * @brief Updates a laptimer with a new configuration
     *
     * @details This function also updates the model on success.
     *
     * @param olddDevice The laptimer that shall be updated.
     * @param newDevice The new laptimer configuration that replaces the old device configuration.
     *
     * @return True the laptimer is successful stored otherwise false.
     */
    virtual Q_INVOKABLE bool update(RapidAndroid::Common::DeviceSettings const& oldDevice,
                                    RapidAndroid::Common::DeviceSettings const& newDevice) noexcept = 0;

    /**
     * @brief Helper Method to create @ref DeviceSettings in the QML context
     *
     * @return The @ref RapidAndroid::Common::DeviceSetting instance with created from the string parameters
     */
    Q_INVOKABLE RapidAndroid::Common::DeviceSettings create(QString const& name,
                                                            QString const& ip,
                                                            QString port,
                                                            bool enabled) noexcept;
Q_SIGNALS:
    /**
     * This signal is emitted when the active laptimer changes
     */
    void activeLaptimerChanged();

protected:
    /**
     * Default constructor
     */
    IDeviceManagement() = default;
};

} // namespace RapidAndroid::Workflow

#endif // RAPIDANDROID_WORKFLOW_IDEVICEMANAGEMENT
