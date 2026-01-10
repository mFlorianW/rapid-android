// SPDX-FileCopyrightText: 2025, 2026 All contributors
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include <QTest>
#include <TestHelper/Session.hpp>
#include <Workflow/Private/LapListModel.hpp>
#include <Workflow/SessionAnalyzer.hpp>

namespace RapidAndroid::Workflow::Tests
{

class SessionAnalyzerTest : public QObject
{
    Q_OBJECT
public:
    Q_DISABLE_COPY_MOVE(SessionAnalyzerTest)

    SessionAnalyzerTest() = default;
    ~SessionAnalyzerTest() override = default;

private Q_SLOTS:
    void testFillLaptimeModel()
    {
        SessionAnalyzer analyzer;
        auto session = TestHelper::getOscherslebenSession();
        auto* model = analyzer.getLapListModel();
        QVERIFY(model != nullptr);

        analyzer.analyzeSession(session);

        QCOMPARE(model->rowCount(), static_cast<int>(session.getLaps().size()));
        auto index = model->index(0, 0);
        QCOMPARE(model->data(index, Workflow::LapListModel::DisplayRole::Laptime).toString(),
                 session.getLaps().at(0).laptime().toString("mm:ss.zzz"));
        QCOMPARE(model->data(index, Workflow::LapListModel::DisplayRole::SectorTimes).toList().size(),
                 static_cast<int>(session.getLaps().at(0).sectors.size()));
        for (int i = 0; i < static_cast<int>(session.getLaps().at(0).sectors.size()); ++i) {
            QCOMPARE(model->data(index, Workflow::LapListModel::DisplayRole::SectorTimes).toList().at(i).toString(),
                     session.getLaps().at(0).sectors.at(i).toString("mm:ss.zzz"));
        }
    }
};

} // namespace RapidAndroid::Workflow::Tests

QTEST_MAIN(RapidAndroid::Workflow::Tests::SessionAnalyzerTest)
#include "SessionAnalyzerTest.moc"
