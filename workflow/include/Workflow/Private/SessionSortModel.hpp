// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_WORKFLOW_PRIVATES_SESSIONSORTMODEL_HPP
#define RAPIDANDROID_WORKFLOW_PRIVATES_SESSIONSORTMODEL_HPP

#include <QSortFilterProxyModel>

namespace RapidAndroid::Workflow
{

/**
 * @brief Proxy model that defines custom sorting for session list models.
 *
 * @details Inherits from QSortFilterProxyModel and overrides lessThan() to provide a deterministic sort order for items from the source model.
 *          On default the items are sorted by date in descending order (newest first).
 *
 * @see QSortFilterProxyModel
 */
class SessionSortModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    Q_DISABLE_COPY_MOVE(SessionSortModel)

    /**
     * @brief Default constructor.
     *
     * Constructs the sort model in a valid state.
     * That means the sort column is set and the sort order is descending.
     */
    explicit SessionSortModel() noexcept;

    /**
     * @brief Destructor.
     */
    ~SessionSortModel() override = default;

protected:
    /**
     * @brief Compares two session infos date to determine their relative sort order.
     *
     * @param left Index of the left-hand item.
     * @param right Index of the right-hand item.
     * @return true if the item at @p left should appear before the item at @p right.
     */
    bool lessThan(QModelIndex const& left, QModelIndex const& right) const override;
};

} // namespace RapidAndroid::Workflow

#endif // RAPIDANDROID_WORKFLOW_PRIVATES_SESSIONSORTMODEL_HPP
