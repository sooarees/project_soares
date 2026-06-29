#ifndef ELEMENTOFASE_H
#define ELEMENTOFASE_H

#include <QGraphicsRectItem>

class ElementoFase : public QGraphicsRectItem
{
public:
    ElementoFase(qreal x, qreal y, qreal width, qreal height);
    ~ElementoFase() override = default;
};
#endif // ELEMENTOFASE_H
