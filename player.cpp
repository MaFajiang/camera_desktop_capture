#include "player.h"
#include "ui_player.h"

Player::Player(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Player)
{
    ui->setupUi(this);
    pCapture = new Capture;
    connect(pCapture, SIGNAL(updateFrame(QImage)), this, SLOT(SlotRefreshImg(QImage)));
}

Player::~Player()
{
    delete ui;
}

void Player::SlotRefreshImg(QImage img)
{
    m_DispImg = img;
    update();
}

void Player::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setBrush(Qt::white);
    painter.drawRect(0, 0, this->width(), this->height()); //先画成白色

    if (m_DispImg.size().width() <= 0) return;

    //将图像按比例缩放成和窗口一样大小
    QImage img = m_DispImg.scaled(this->size(),Qt::KeepAspectRatio);

    int x = this->width() - img.width();
    int y = this->height() - img.height();

    x /= 2;
    y /= 2;

    painter.drawImage(QPoint(x,y),img); //画出图像
}
