#ifndef PRINCESA_H
#define PRINCESA_H

#include "elementofase.h"
#include <QPixmap>

class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;

class Princesa : public ElementoFase
{
public:
    Princesa(qreal x, qreal y);

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
