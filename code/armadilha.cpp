#include "armadilha.h"

Armadilha::Armadilha(qreal x, qreal y, qreal width, qreal height)
    : ElementoFase(x, y, width, height)
{
}

bool Armadilha::causaDano() const
{
    return true;
}
