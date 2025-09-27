#include <QThread>

#include "MainWindow.h"
#include "Configuration.h"
#include "TableServer.h"
#include <QDarkApplication.h>

int main(int argc, char* argv[])
{
    QDarkApplication a(argc, argv);
    TableServer server;
    MainWindow w;
    w.show();
    return a.exec();
}
