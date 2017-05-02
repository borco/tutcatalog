#include "mainwindow.h"
#include "config.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QSettings>

int main(int argc, char *argv[])
{
    QSettings::setDefaultFormat(QSettings::IniFormat);

    QApplication a(argc, argv);
    QApplication::setOrganizationName(MY_COMPANY);
    QApplication::setOrganizationDomain(MY_DOMAIN);
    QApplication::setApplicationVersion(MY_VERSION);
    QApplication::setApplicationName("tut-catalog");

    QCommandLineParser parser;
    parser.setApplicationDescription("tutorial catalog app");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", QApplication::translate("main", ".tutcatalog file, optional"), "[file]");
    parser.process(a);

    const QStringList args = parser.positionalArguments();

    MainWindow w(args.size() == 1 ? args[0] : QString());
    w.show();

    return a.exec();
}
