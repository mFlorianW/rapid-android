// SPDX-FileCopyrightText: 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef RAPIDANDROID_WORKFLOW_ISESSIONLIVEVIEWEVENTSOURCE_HPP
#define RAPIDANDROID_WORKFLOW_ISESSIONLIVEVIEWEVENTSOURCE_HPP

#include <Common/DeviceSettings.hpp>
#include <Common/LaptimeEvents.hpp>
#include <QObject>
#include <concepts>

namespace RapidAndroid::Workflow
{

template <typename T>
concept LiveSessionEventSourceConcept = std::derived_from<T, QObject> and requires(T obj) {
    { &T::laptimeStarted } -> std::convertible_to<void (T::*)()>;
    { &T::currentLaptimeChanged } -> std::convertible_to<void (T::*)(Common::LaptimeEvent const&)>;
    { &T::laptimeFinished } -> std::convertible_to<void (T::*)(Common::LapFinishedEvent const&)>;
    { &T::lapSectorFinished } -> std::convertible_to<void (T::*)(Common::LapSectorEvent const&)>;
    { obj.setLaptimerConfig(std::declval<Common::DeviceSettings>()) } -> std::same_as<void>;
};

} // namespace RapidAndroid::Workflow

#endif // RAPIDANDROID_WORKFLOW_ISESSIONLIVEVIEWEVENTSOURCE_HPP
