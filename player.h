#ifndef PLAYER_H
#define PLAYER_H

#include <QMainWindow>
#include <QPainter>
#include "capture.h"

namespace Ui {
class Player;
}

class Player : public QMainWindow
{
    Q_OBJECT

public:
    explicit Player(QWidget *parent = 0);
    ~Player();
public slots:
    void SlotRefreshImg(QImage img);

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::Player *ui;
    Capture *pCapture;
    QImage m_DispImg;
};

#endif // PLAYER_H
