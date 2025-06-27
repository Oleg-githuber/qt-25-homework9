#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    udpWorker = new UDPworker(this);
    udpWorker->InitSocket();

    connect(udpWorker, &UDPworker::sig_sendTimeToGUI, this, &MainWindow::DisplayTime);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&]{

        QDateTime dateTime = QDateTime::currentDateTime();

        QByteArray dataToSend;
        QDataStream outStr(&dataToSend, QIODevice::WriteOnly);

        outStr << dateTime;

        udpWorker->SendDatagram(dataToSend);
        timer->start(TIMER_DELAY);

    });

    connect(udpWorker, &UDPworker::sig_sendTextToGUI, this, &MainWindow::DisplayText);  // Отображение информации о текстовой датаграмме
    connect(ui->buttonSendDG, &QPushButton::clicked, this, &MainWindow::buttonSendTextHandler); // Обработка нажания кнопки
    connect(this, &MainWindow::sig_sendTextToWorker, udpWorker, &UDPworker::receiveTextFromGUI);    // Передача текста

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pb_start_clicked()
{
    timer->start(TIMER_DELAY);
}


void MainWindow::DisplayTime(QDateTime data)
{
    counterPck++;
    if(counterPck % 20 == 0){
        ui->te_result->clear();
    }

    ui->te_result->append("Текущее время: " + data.toString() + ". "
                "Принято пакетов " + QString::number(counterPck));


}

void MainWindow::DisplayText(QString txt)
{
    ui->te_result->append(txt);
}


void MainWindow::on_pb_stop_clicked()
{
    timer->stop();
}

void MainWindow::buttonSendTextHandler()
{
    emit sig_sendTextToWorker(ui->inputText->text());
}
