#include <QtTest>

#include "tc/project.h"
#include "tc/tutorials/folder.h"
#include "tc/tutorials/folderinfo.h"

using namespace tc;
using namespace tc::tutorials;

class Test : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testSetInfo() {
        FolderInfo info;
        info.set_name("name");
        info.set_path("path");
        info.set_cachePath("cache");
        info.set_noBackupPath("no backup");
        info.set_trashPath("trash");
        info.set_withCopies(true);

        Folder item;
        item.setup(&info);

        QVERIFY(*item.info() == info);
    }

    void testClearInfo() {
        FolderInfo info;
        info.set_name("name");
        info.set_path("path");
        info.set_cachePath("cache");
        info.set_noBackupPath("no backup");
        info.set_trashPath("trash");
        info.set_withCopies(true);

        Folder item;
        item.setup(&info);
        item.setup(nullptr);

        QVERIFY(*item.info() == FolderInfo());
    }
};

QTEST_APPLESS_MAIN(Test)

#include "test.moc"
