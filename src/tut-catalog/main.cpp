#include "mainwindow.h"
#include "config.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setOrganizationName(MY_COMPANY);
    QApplication::setOrganizationDomain(MY_DOMAIN);
    QApplication::setApplicationVersion(MY_VERSION);
    QApplication::setApplicationName(MY_APPLICATION);

    QCommandLineParser parser;
    parser.setApplicationDescription("---\nA database for your local, remote and on-line tutorials.");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption({ { "i", "ini"}, QCoreApplication::translate("main", "Custom settings file. (eg: foo.ini)"), "INI" });
    parser.addPositionalArgument("PROJECT", QApplication::translate("main", "File defining top tutorial folders. (eg: foo.tutcatalog)"), "PROJECT");
    parser.process(a);

    MainWindow w(parser);
    w.show();

    return a.exec();
}
