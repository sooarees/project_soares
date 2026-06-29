#include "princesa.h"
#include <QPainter>
#include <QPainterPath>

namespace
{
const qreal larguraVisual = 115;
const qreal alturaVisual = 50;

QRectF hitboxPrincesa()
{
    return QRectF(0,0,115,45);
}
}

Princesa::Princesa(qreal x, qreal y)
    : ElementoFase(x,y,larguraVisual,alturaVisual)
{
    sprite.load(":/Sprites/Game Images/Royal/Princess/deitada.png");
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
}
