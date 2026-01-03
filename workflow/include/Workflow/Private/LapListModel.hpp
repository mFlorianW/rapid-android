// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPID_RAPIDANDROID_WORKFLOW_LAPLISTMODEL_HPP
#define RAPID_RAPIDANDROID_WORKFLOW_LAPLISTMODEL_HPP

#include <Common/GenericListModel.hpp>
#include <Common/Lap.hpp>

namespace RapidAndroid::Workflow
{

/**
 * @class RapidAndroid::Workflow::LapListModel
 * @brief Model representing a list of laps.
 *
 * This model provides access to lap data for use in QML views.
 */
class LapListModel : public Common::GenericListModel<Common::Lap>
{
    Q_OBJECT
public:
    Q_DISABLE_COPY_MOVE(LapListModel)

    /**
     * @brief The custom display roles supported by the model
     * @details The main purpose for the roles are for the QML context
     */
    enum DisplayRole
    {
        Laptime = ::Qt::UserRole + 1,
    };
    Q_ENUM(DisplayRole)

    /**
     * @brief Creates an empty instance of @ref RapidAndroid::Workflow::LapListModel.
     */
    LapListModel();

    /**
     * @brief Default destructor.
     */
    ~LapListModel() override;

    /**
     * @brief Creates an instance of @ref Rapid::Common::Qt::LapListModel with initial data.
     *
     * @details The laps are inserted to the model in the order of the vector.
     */
    [[nodiscard]] LapListModel(std::vector<Common::Lap> const& laps) noexcept;

    /**
     * @copydoc RapidAndroid::Common::GenericListModel
     */
    QVariant data(QModelIndex const& index, int role = ::Qt::DisplayRole) const noexcept override;
};

} // namespace RapidAndroid::Workflow

#endif // RAPID_RAPIDANDROID_WORKFLOW_LAPLISTMODEL_HPP
