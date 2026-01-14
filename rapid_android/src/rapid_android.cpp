// SPDX-FileCopyrightText: 2025, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QtQml/QQmlExtensionPlugin>

Q_IMPORT_QML_PLUGIN(Rapid_AndroidPlugin)
Q_IMPORT_QML_PLUGIN(Rapid_ControlsPlugin)
Q_IMPORT_QML_PLUGIN(Rapid_LaptimerPlugin)
Q_IMPORT_QML_PLUGIN(Rapid_SessionPlugin)
Q_IMPORT_QML_PLUGIN(Rapid_LiveSessionPlugin)

int main(int argc, char** argv)
{
    auto app = QGuiApplication{argc, argv};
    QQuickStyle::setStyle("Material");

    QCoreApplication::setOrganizationName("Rapid");
    QCoreApplication::setOrganizationDomain("rapid.de");
    QCoreApplication::setApplicationName("Rapid Laptimer");

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() {
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(QUrl{"qrc:/qt/qml/Rapid/Android/qml/RapidAndroid.qml"});

    return app.exec();
}
