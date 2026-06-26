
#ifndef PORTAL_H
#define PORTAL_H

#include "elementofase.h"
#include <QPixmap>

class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;

class Portal : public ElementoFase
{
public:
    Portal(qreal x, qreal y, int destino);

    int getDestino() const;

protected:
    void paint(
        QPainter *painter,
        const QStyleOptionGraphicsItem *option,
        QWidget *widget
        ) override;

private:
    int faseDestino;
    QPixmap sprite;
};

#endif
