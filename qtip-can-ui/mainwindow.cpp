#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <qcanbusframe.h>

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


    connect(ui->sendPacket, SIGNAL(released()), this, SLOT(sendPacket()));

}


MainWindow::~MainWindow()
{
    can.disconnectDevice();

    delete ui;
}


void MainWindow::sendPacket()
{
    QCanBusFrame frame;

    QByteArray bytes;

    bytes.append(0xAA);
    bytes.append(0xBB);
    bytes.append(0xCC);

    frame.setPayload(bytes);
    frame.setFrameId(0x12345678);
    frame.setFrameType(QCanBusFrame::FrameType::DataFrame);
    frame.setExtendedFrameFormat(true);

    can.writeFrame(frame);
}
