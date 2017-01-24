#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
class QTcpSocket;
class QPushButton;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected slots:
    void tryConnection();
    void tryDisconnection();
    void tcpConnected();
    void tcpError();
    void readyRead();

private:
    QTcpSocket * tcpClient;
    QPushButton * start;
    QPushButton * over;
    QByteArray inBlock,outBlock;
    int count;
};

#endif // WIDGET_H
