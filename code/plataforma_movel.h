#ifndef PLATAFORMA_MOVEL_H
#define PLATAFORMA_MOVEL_H

#include "plataforma.h"
#include <QObject>
#include <QPointF>

class QTimer;

class PlataformaMovel : public QObject, public Plataforma
{
public:
    PlataformaMovel(
        qreal x,
        qreal y,
        qreal width,
        qreal height,
        qreal fimX,
        qreal fimY,
        qreal velocidade
        );

    QPointF deslocamentoUltimoFrame() const;

private:
    QPointF inicio;
    QPointF fim;
    QPointF destinoAtual;
    QPointF deslocamentoUltimo;
    qreal velocidade;
    QTimer *timer;

    void atualizarMovimento();
};

#endif
