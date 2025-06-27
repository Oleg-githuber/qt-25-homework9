#ifndef UDPWORKER_H
#define UDPWORKER_H

#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QDateTime>


#define BIND_PORT 12345
#define TXT_DG_PORT 12346   // Порт для передачи текстовой датаграммы

class UDPworker : public QObject
{
    Q_OBJECT
public:
    explicit UDPworker(QObject *parent = nullptr);
    void InitSocket( void );
    void ReadDatagram( QNetworkDatagram datagram);
    void SendDatagram(QByteArray data );

    /**
     * @brief Обработка принятой текстовой датаграммы
     * @param Принятая датаграмма
     */
    void ReadTextFromDatagram(QNetworkDatagram datagram);

    /**
     * @brief Передача текстовой датаграммы
     * @param Датаграмма в виде QByteArray
     */
    void SendTextDatagram(QByteArray data);

private slots:
    void readPendingDatagrams(void);

    /**
     * @brief Чтение текстовой датаграммы
     */
    void readTextDatagram(void);

public slots:
    /**
     * @brief Приём текста из GUI
     * @param Принимаемый текст
     */
    void receiveTextFromGUI(QString txt);

private:
    QUdpSocket* serviceUdpSocket;
    /**
     * @brief Сокет для передачи текстовой датаграммы
     */
    QUdpSocket* textUdpSocket;

signals:
    void sig_sendTimeToGUI(QDateTime data);

    /**
     * @brief Передача текста в GUI
     * @param Передаваемый текст
     */
    void sig_sendTextToGUI(QString txt);

};

#endif // UDPWORKER_H
