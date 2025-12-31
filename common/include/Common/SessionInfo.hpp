// SPDX-FileCopyrightText: 2025 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_COMMON_SESSIONINFO_HPP
#define RAPIDANDROID_COMMON_SESSIONINFO_HPP

#include <QDateTime>
#include <QString>

namespace RapidAndroid::Common
{

/**
 * @brief Describes a recorded session.
 *
 * @details Identifying metadata for a single session.
 */
struct SessionInfo
{
    Q_GADGET

    Q_PROPERTY(QString id MEMBER id)
    Q_PROPERTY(QDateTime date MEMBER date)
    Q_PROPERTY(QString trackName MEMBER trackName)
    Q_PROPERTY(quint32 laps MEMBER laps)
public:
    QString id; //!< Unique session identifier (e.g., UUID or descriptive string).
    QDateTime date; //!< Timestamp when the session was recorded.
    QString trackName; //!< Human-readable track name associated with the session.
    quint32 laps; //!< Number of completed laps in the session.

    bool operator==(SessionInfo const& other) const noexcept = default;
    bool operator!=(SessionInfo const& other) const noexcept = default;
};

} // namespace RapidAndroid::Common

#endif // RAPIDANDROID_COMMON_SESSIONINFO_HPP
