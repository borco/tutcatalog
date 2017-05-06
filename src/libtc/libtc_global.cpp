#include "libtc_global.h"
#include <QSqlDatabase>

void libtc_init()
{
    QSqlDatabase::addDatabase("QSQLITE");
}
