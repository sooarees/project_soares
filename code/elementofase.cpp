#include "elementofase.h"
#include <QPainterPath>

ElementoFase::ElementoFase(qreal x, qreal y, qreal width, qreal height)
{
    setRect(0, 0, width, height);
    definirHitbox(rect());
    setPos(x, y);
}

QPainterPath ElementoFase::shape() const
{
    QPainterPath path;
    path.addRect(hitbox());
    return path;
}

QRectF ElementoFase::hitbox() const
{
    return areaHitbox;
}

void ElementoFase::definirHitbox(const QRectF &novaHitbox)
{
    areaHitbox = novaHitbox;
}
