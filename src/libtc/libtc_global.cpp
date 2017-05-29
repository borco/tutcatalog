#include "libtc_global.h"

#include "tc/tutorials/tutorial.h"

#include <QSqlDatabase>

void libtc_init()
{
    QSqlDatabase::addDatabase("QSQLITE");

    qRegisterMetaType<QList<tc::tutorials::Tutorial*>>("Tutorials");
}
