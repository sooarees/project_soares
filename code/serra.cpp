#include "serra.h"
#include <QDateTime>
#include <QPainter>
#include <QPainterPath>
#include <QtGlobal>

namespace
{
const int tamanhoSerra = 130;
}

Serra::Serra(qreal x, qreal y)
    : Armadilha(x,y-85,tamanhoSerra,tamanhoSerra)
{
    sprite.load(":/Sprites/Game Images/Royal/Armadilhas/serra.png");
}

QPainterPath Serra::shape() const
{
    qreal diametro = qMin(rect().width(), rect().height());
    QRectF circulo(
        rect().center().x() - diametro / 2,
        rect().center().y() - diametro / 2,
        diametro,
        diametro
        );

    QPainterPath path;
    path.addEllipse(circulo);
    return path;
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
        QRectF(0,0,tamanhoSerra,tamanhoSerra),
        sprite,
        QRectF(frame * frameWidth,0,frameWidth,frameHeight)
        );

}
