#include <QApplication>
#include <QMessageBox>
#include "connect_mysql.h"
#include "farm_server.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(connect_mysql() == false)
    {
        QMessageBox *messagebox = new QMessageBox(QMessageBox::Warning,"错误","加载数据失败，请检查网络状态后重启!");
        messagebox->exec();
        exit(0);
    }
    farm_Server farm_server;
    return a.exec();
}
