#include "espinho.h"
#include <QPainter>

Espinho::Espinho(qreal x, qreal y)
    : Armadilha(x,y,60,40)
{
    sprite.load(":/Sprites/Game Images/Royal/Espinho/espinho.png");
}

void Espinho::paint(
    QPainter *painter,
    const QStyleOptionGraphicsItem *option,
    QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);


    painter->drawPixmap(
        QRectF(0,0,60,40),
        sprite,
        sprite.rect()
        );
}