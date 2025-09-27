#pragma once

#include <memory>

#include <QApplication>

#include "Configuration.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 12, 0)
#error Your Qt version is likely too old, upgrade to 5.12 or higher
#endif // QT_VERSION

class QDarkApplication : public QApplication
{
    Q_OBJECT

public:
    QDarkApplication(int argc, char** argv);

private:
    std::unique_ptr<Configuration> mConfiguration;
};
