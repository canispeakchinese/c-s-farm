#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>

class QLabel;
class QLineEdit;
class QPushButton;

class Login : public QWidget
{
    Q_OBJECT

public:
    Login(QWidget * parent = 0);
    ~Login();

signals:
    void sendLoginRequest(QString username, QString password, bool isSignin);

protected slots:
    void tryLogin();
    void trySign();

private:
    QLabel * userName;
    QLabel * passWord;
    QLineEdit * inputName;
    QLineEdit * inputWord;
    QPushButton * login;
    QPushButton * sign;
};

#endif // LOGIN_H
