#include "plataforma.h"
#include <QBrush>

Plataforma::Plataforma(qreal x, qreal y, qreal width, qreal height) : ElementoFase(x, y, width, height)
{
    setBrush(Qt::darkGray);
}