#include "portal.h"
#include <QBrush>

Portal::Portal(qreal x, qreal y, int destino): ElementoFase(x, y, 50, 100),faseDestino(destino)
{
    setBrush(Qt::green);
}

int Portal::getDestino() const
{
    return faseDestino;
}