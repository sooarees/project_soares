#include "princesa.h"
#include <QPainter>
#include <QPainterPath>

namespace
{
const qreal larguraVisual = 140;
const qreal alturaVisual = 70;

QRectF hitboxPrincesa()
{
    return QRectF(0,0,140,60);
}
}

Princesa::Princesa(qreal x, qreal y)
    : ElementoFase(x,y,larguraVisual,alturaVisual)
{
    sprite.load(":/Sprites/Game Images/Royal/Princess/temporario.png");
}

QRectF Princesa::boundingRect() const
{
    return QRectF(0,0,larguraVisual,alturaVisual);
}

QPainterPath Princesa::shape() const
{
    QPainterPath path;
    path.addRect(hitboxPrincesa());
    return path;
}

void Princesa::paint(
    QPainter *painter,
    const QStyleOptionGraphicsItem *option,
    QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->drawPixmap(
        QRectF(0,0,larguraVisual,alturaVisual),
        sprite,
        sprite.rect()
        );

    // DEBUG temporario: azul = sprite, vermelho = hitbox de colisao
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(Qt::blue, 2));
    painter->drawRect(boundingRect());

    painter->setPen(QPen(Qt::red, 2));
    painter->drawRect(hitboxPrincesa());
}
