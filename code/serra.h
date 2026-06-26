#ifndef SERRA_H
#define SERRA_H

#include "armadilha.h"
#include <QPainterPath>
#include <QPixmap>

class Serra : public Armadilha
{
public:
    Serra(qreal x, qreal y);
    QPainterPath shape() const override;

protected:
    void paint(
        QPainter *painter,
        const QStyleOptionGraphicsItem *option,
        QWidget *widget
        ) override;

private:
    QPixmap sprite;
};

#endif
