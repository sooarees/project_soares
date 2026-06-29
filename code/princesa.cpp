#include "princesa.h"
#include <QPainter>

namespace
{
const qreal larguraVisual = 115;
const qreal alturaVisual = 50;
const QRectF hitboxPrincesa(0, 0, 115, 45);
} // namespace

Princesa::Princesa(qreal x, qreal y)
    : ElementoFase(x, y, larguraVisual, alturaVisual)
{
    definirHitbox(hitboxPrincesa);
    sprite.load(":/Sprites/Game Images/Royal/Princess/deitada.png");
}

void Princesa::paint(
    QPainter *painter,
    const QStyleOptionGraphicsItem *option,
    QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->drawPixmap(
        QRectF(0, 0, larguraVisual, alturaVisual),
        sprite,
        sprite.rect());
}
