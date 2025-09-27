#include "QDarkApplication.h"
#include "MainWindow.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 12, 0)
#error Your Qt version is likely too old, upgrade to 5.12 or higher
#endif // QT_VERSION

// https://www.qt.io/blog/dark-mode-on-windows-11-with-qt-6.5

int main(int argc, char* argv[])
{
    printf("argc: %d\n", argc);
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
