#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class QTcpServer;
class QTcpSocket;
class QTimer;
class QPushButton;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected slots:
    void newConnection();
    void timeOut();
    void reTran();
    void readyRead();

private:
    int count;
    QTcpServer * tcpServer;
    QTcpSocket * tcpServerConnection;
    QTimer * timer;
    QByteArray inBlock,outBlock;
    QPushButton * retran;
};

#endif // WIDGET_H
