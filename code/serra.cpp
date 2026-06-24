#include "serra.h"
#include <QPainter>

Serra::Serra(qreal x, qreal y)
    : Armadilha(x,y,50,50)
{
}

void Serra::paint(
    QPainter *painter,
    const QStyleOptionGraphicsItem *option,
    QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setBrush(Qt::gray);
    painter->setPen(QPen(Qt::darkGray, 3));
    painter->drawEllipse(QRectF(0,0,50,50));

    painter->setBrush(Qt::lightGray);
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(QRectF(18,18,14,14));
}
