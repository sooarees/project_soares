#include "portal.h"
#include <QDateTime>
#include <QPainter>

Portal::Portal(qreal x, qreal y, int destino): ElementoFase(x, y, 50, 100),faseDestino(destino)
{
    sprite.load(":/Sprites/Game Images/Royal/Portal/Dimensional_Portal.png");
}

int Portal::getDestino() const
{
    return faseDestino;
}

void Portal::paint(
    QPainter *painter,
    const QStyleOptionGraphicsItem *option,
    QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    const int frameWidth = 32;
    const int frameHeight = 32;
    const int totalFrames = 6;
    const int frame = (QDateTime::currentMSecsSinceEpoch() / 120) % totalFrames;
    const int coluna = frame / 2;
    const int linha = frame % 2;

    painter->drawPixmap(
        QRectF(0,0,77,100),
        sprite,
        QRectF(coluna * frameWidth, linha * frameHeight, frameWidth, frameHeight)
        );
}
