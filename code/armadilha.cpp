#include "armadilha.h"
#include <QBrush>

Armadilha::Armadilha(qreal x, qreal y, qreal width, qreal height): ElementoFase(x,y,width,height)
{
    setBrush(Qt::red);
}

int Armadilha::dano() const
{
    return 1;
}