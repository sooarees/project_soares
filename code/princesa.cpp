#include "princesa.h"
#include <QPainter>

Princesa::Princesa(qreal x, qreal y)
    : ElementoFase(x,y,140,210)
{
    sprite.load(":/Sprites/Game Images/Royal/Princess/temporario.png");
}

void Princesa::paint(
    QPainter *painter,
    const QStyleOptionGraphicsItem *option,
    QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->drawPixmap(
        QRectF(0,0,140,210),
        sprite,
        sprite.rect()
        );
}
