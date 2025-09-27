#include <QDarkApplication.h>
#include "MainWindow.h"

int main(int argc, char* argv[])
{
    QDarkApplication app(argc, argv);
    MainWindow w;
    w.show();

    // Load the dump provided on the command line
    if(argc > 1)
    {
        w.loadFile(QString::fromUtf8(argv[1]));
    }

    return app.exec();
}
