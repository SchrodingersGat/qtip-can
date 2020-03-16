#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString title = "QTipCAN Tester";

    bool result = can.connectDevice();

    if (result)
    {
        title += " - ";
        title += QString::number(can.getPortNumber());
    }
    else
    {
        title += " - Not connected";
    }

    setWindowTitle(title);

}


MainWindow::~MainWindow()
{
    can.disconnectDevice();

    delete ui;
}
