#include "serra.h"
#include <QDateTime>
#include <QPainter>

Serra::Serra(qreal x, qreal y)
    : Armadilha(x,y-95,150,150)
{
    sprite.load(":/Sprites/Game Images/Royal/Armadilhas/serra.png");
}

void Serra::paint(
    QPainter *painter,
    const QStyleOptionGraphicsItem *option,
    QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    const int frameWidth = 32;
    const int frameHeight = 32;
    const int totalFrames = 8;
    const int frame = (QDateTime::currentMSecsSinceEpoch() / 25) % totalFrames;

    painter->drawPixmap(
        QRectF(0,0,150,150),
        sprite,
        QRectF(frame * frameWidth,0,frameWidth,frameHeight)
        );
}
