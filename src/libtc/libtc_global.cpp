#include "libtc_global.h"

#include "tc/tutorials/tutorial.h"

#include <QSqlDatabase>
#include <QVector>

void libtc_init()
{
    QSqlDatabase::addDatabase("QSQLITE");

    qRegisterMetaType<QVector<tc::tutorials::Tutorial*>>("Tutorials");
}
