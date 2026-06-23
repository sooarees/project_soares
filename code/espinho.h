#ifndef ESPINHO_H
#define ESPINHO_H

#include "armadilha.h"
#include <QPixmap>

class Espinho : public Armadilha
{
public:
    Espinho(qreal x, qreal y);

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