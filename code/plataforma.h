#ifndef PLATAFORMA_H
#define PLATAFORMA_H

#include "elementofase.h"

class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;

class Plataforma : public ElementoFase
{
public:
    Plataforma(
        qreal x,
        qreal y,
        qreal width,
        qreal height);

    void paint(QPainter
                   *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
};

#endif // PLATAFORMA_H
