#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include "Disassembler/Architecture.h"
#include <QDebug>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QSplitter>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QCheckBox>
#include <QSpacerItem>
#include <QTimer>
#include <cinttypes>

// HACK: IDA defines these Qt functions in their headers, so we rename them to avoid conflicts
#define qalloc ida_qalloc
#define qrealloc ida_qrealloc
#define qcalloc ida_qcalloc
#define qfree ida_qfree
#define qstrdup ida_qstrdup
#define qstrlen ida_qstrlen
#define qstrncmp ida_qstrncmp
#define qstrncpy ida_qstrncpy
#define qsnprintf ida_qsnprintf
#define qvsnprintf ida_qvsnprintf

#include "idalib.hpp"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupNavigation();
    setupWidgets();

    // Initialize idalib
    auto status = init_library();
    if(status != 0)
    {
        QMessageBox::critical(this, tr("Error"), tr("Failed to initialize idalib: %1").arg(status));
        QApplication::quit();
        return;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadFile(const QString & path)
{
    // TODO: always errors with 0x5783f450 (undocumented error code)
    auto status = open_database(path.toUtf8().constData(), true);
    if(status != 0)
    {
        QMessageBox::critical(this, tr("Error"), tr("Failed to open database: 0x%1").arg(status, 0, 16));
        return;
    }
}

void MainWindow::setupNavigation()
{
}

struct DefaultArchitecture : Architecture
{
    bool disasm64() const override
    {
        return true;
    }

    bool addr64() const override
    {
        return true;
    }
} gArchitecture;

Architecture* GlobalArchitecture()
{
    return &gArchitecture;
}

void MainWindow::setupWidgets()
{

}

void MainWindow::on_action_Load_file_triggered()
{
    // TODO: remember the previous browse directory
    auto fileName = QFileDialog::getOpenFileName(this, "Load file", QString(), "All files (*)");
    if(!fileName.isEmpty())
    {
        loadFile(fileName);
    }
}
