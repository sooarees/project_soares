#ifndef ELEMENTOFASE_H
#define ELEMENTOFASE_H

#include <QGraphicsRectItem>
#include <QPainterPath>

class ElementoFase : public QGraphicsRectItem
{
public:
    ElementoFase(
        qreal x,
        qreal y,
        qreal width,
        qreal height);
    ~ElementoFase() override = default;

    QPainterPath shape() const override;

protected:
    QRectF hitbox() const;
    void definirHitbox(const QRectF &novaHitbox);

private:
    QRectF areaHitbox;
};
#endif // ELEMENTOFASE_H
