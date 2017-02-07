#ifndef BASESCENE_H
#define BASESCENE_H


#include <QGraphicsView>

class QPushButton;

class BaseScene : public QGraphicsView
{
    Q_OBJECT
public:
    explicit BaseScene(QWidget *parent = 0);
    ~BaseScene();

signals:

public slots:

private:
    QPushButton * closeButton;
};

#endif // BASESCENE_H
