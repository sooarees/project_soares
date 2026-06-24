#ifndef SERRA_H
#define SERRA_H

#include "armadilha.h"

class Serra : public Armadilha
{
public:
    Serra(qreal x, qreal y);

protected:
    void paint(
        QPainter *painter,
        const QStyleOptionGraphicsItem *option,
        QWidget *widget
        ) override;
};

#endif
