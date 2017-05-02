#include <QtTest>

#include "tc/project.h"
#include "tc/folders/folderinfo.h"

using namespace tc;
using namespace tc::folders;

class Test : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testParseTutorialPath_data() {
        QTest::addColumn<QByteArray>("input");
        QTest::addColumn<QStringList>("folders");

        QTest::newRow("folders: none set") << QByteArray("") << QStringList();
        QTest::newRow("folders: set one") << QByteArray("tutorial folders:\n- path: foo\n") << QStringList("foo");
        QTest::newRow("folders: set many") << QByteArray("tutorial folders:\n- path: foo\n- path: bar") << QStringList({ "foo", "bar" });
    }

    void testParseTutorialPath() {
        QFETCH(QByteArray, input);
        QFETCH(QStringList, folders);

        Project p;
        QVERIFY(p.readFromData(input));

        QStringList list;
        for(auto i: p.folderInfos()) {
            list << i->path();
        }
        QCOMPARE(list, folders);
    }

    void testParseTutorialTrashPath_data() {
        QTest::addColumn<QByteArray>("input");
        QTest::addColumn<QString>("value");

        QTest::newRow("trash: not set") << QByteArray("tutorial folders:\n- path: foo\n") << "";
        QTest::newRow("trash: set") << QByteArray("tutorial folders:\n- trash : bar\n") << "bar";
    }

    void testParseTutorialTrashPath() {
        QFETCH(QByteArray, input);
        QFETCH(QString, value);

        Project p;
        QVERIFY(p.readFromData(input));
        QCOMPARE(p.folderInfos().size(), 1);
        QCOMPARE(p.folderInfos().at(0)->trashPath(), value);
    }

    void testParseTutorialWithCopies_data() {
        QTest::addColumn<QByteArray>("input");
        QTest::addColumn<bool>("value");

        QTest::newRow("with copies: not set") << QByteArray("tutorial folders:\n- path: foo\n") << false;
        QTest::newRow("with copies: set to 'no'") << QByteArray("tutorial folders:\n- with copies: no\n") << false;
        QTest::newRow("with copies: set to 'No'") << QByteArray("tutorial folders:\n- with copies: No\n") << false;
        QTest::newRow("with copies: set to some invalid value") << QByteArray("tutorial folders:\n- with copies: Foo\n") << false;
        QTest::newRow("with copies: set to 'yes'") << QByteArray("tutorial folders:\n- with copies: yes\n") << true;
        QTest::newRow("with copies: set to 'Yes'") << QByteArray("tutorial folders:\n- with copies: Yes\n") << true;
    }

    void testParseTutorialWithCopies() {
        QFETCH(QByteArray, input);
        QFETCH(bool, value);

        Project p;
        QVERIFY(p.readFromData(input));
        QCOMPARE(p.folderInfos().size(), 1);
        QCOMPARE(p.folderInfos().at(0)->withCopies(), value);
    }

    void testParseVideoExtensions_data() {
        QTest::addColumn<QByteArray>("input");
        QTest::addColumn<QStringList>("videoExtensions");

        QTest::newRow("video extensions: not set") << QByteArray("") << QStringList();
        QTest::newRow("video extensions: one extension") << QByteArray("video extensions:\n- foo\n") << QStringList({"foo"});
        QTest::newRow("video extensions: many extensions") << QByteArray("video extensions:\n- foo\n- bar\n") << QStringList({"foo", "bar"});
        QTest::newRow("video extensions: compact, empty") << QByteArray("video extensions: []\n") << QStringList();
        QTest::newRow("video extensions: compact, one extension") << QByteArray("video extensions: [ foo ]\n") << QStringList({"foo"});
        QTest::newRow("video extensions: compact, many extensions") << QByteArray("video extensions: [ foo, bar]\n") << QStringList({"foo", "bar"});
    }

    void testParseVideoExtensions() {
        QFETCH(QByteArray, input);
        QFETCH(QStringList, videoExtensions);

        Project p;
        QVERIFY(p.readFromData(input));
        QCOMPARE(p.videoExtensions(), videoExtensions);
    }

    void testParseFfprobe_data() {
        QTest::addColumn<QByteArray>("input");
        QTest::addColumn<QString>("ffprobePath");

        QTest::newRow("ffprobe: not set") << QByteArray("") << "";
        QTest::newRow("ffprobe: set") << QByteArray("ffprobe: foo\n") << "foo";
    }

    void testParseFfprobe() {
        QFETCH(QByteArray, input);
        QFETCH(QString, ffprobePath);

        Project p;
        QVERIFY(p.readFromData(input));
        QCOMPARE(p.ffprobePath(), ffprobePath);
    }
};

QTEST_APPLESS_MAIN(Test)

#include "test.moc"
