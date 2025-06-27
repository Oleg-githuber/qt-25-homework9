#include "udpworker.h"

UDPworker::UDPworker(QObject *parent) : QObject(parent)
{



}


/*!
 * @brief Метод инициализирует UDP сервер
 */
void UDPworker::InitSocket()
{

    serviceUdpSocket = new QUdpSocket(this);
    /*
     * Соединяем присваиваем адрес и порт серверу и соединяем функцию
     * обраотчик принятых пакетов с сокетом
     */
    serviceUdpSocket->bind(QHostAddress::LocalHost, BIND_PORT);

    textUdpSocket = new QUdpSocket(this);
    textUdpSocket->bind(QHostAddress::LocalHost, TXT_DG_PORT);

    connect(serviceUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readPendingDatagrams);
    connect(textUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readTextDatagram);
}

/*!
 * @brief Метод осуществляет обработку принятой датаграммы
 */
void UDPworker::ReadDatagram(QNetworkDatagram datagram)
{

    QByteArray data;
    data = datagram.data();


    QDataStream inStr(&data, QIODevice::ReadOnly);
    QDateTime dateTime;
    inStr >> dateTime;

    emit sig_sendTimeToGUI(dateTime);
}
/*!
 * @brief Метод осуществляет опередачу датаграммы
 */
void UDPworker::SendDatagram(QByteArray data)
{
    /*
     *  Отправляем данные на localhost и задефайненный порт
     */
    serviceUdpSocket->writeDatagram(data, QHostAddress::LocalHost, BIND_PORT);
}

/*!
 * @brief Метод осуществляет чтение датаграм из сокета
 */
void UDPworker::readPendingDatagrams( void )
{
    /*
     *  Производим чтение принятых датаграмм
     */
    while(serviceUdpSocket->hasPendingDatagrams()){
            QNetworkDatagram datagram = serviceUdpSocket->receiveDatagram();
            ReadDatagram(datagram);
    }

}

void UDPworker::ReadTextFromDatagram(QNetworkDatagram datagram)
{
    QByteArray data;
    data = datagram.data();

    QHostAddress ipSender = datagram.senderAddress();
    QString ipText = ipSender.toString();

    QDataStream inStr(&data, QIODevice::ReadOnly);
    QString txt;
    inStr >> txt;

    QString message = "Принято сообщение от " + ipText + ", размер сообщения " + QString::number(sizeof(datagram)) + " байт.";
    emit sig_sendTextToGUI(message);
    emit sig_sendTextToGUI("Сообщение: " + txt);
}

void UDPworker::readTextDatagram(void)
{
    while (textUdpSocket->hasPendingDatagrams()) {
        ReadTextFromDatagram(textUdpSocket->receiveDatagram());
    }
}

void UDPworker::receiveTextFromGUI(QString txt)
{
    QByteArray dataToSend;
    QDataStream outStr(&dataToSend, QIODevice::WriteOnly);
    outStr << txt;
    SendTextDatagram(dataToSend);
}

void UDPworker::SendTextDatagram(QByteArray data)
{
    textUdpSocket->writeDatagram(data, QHostAddress::LocalHost, TXT_DG_PORT);
}
