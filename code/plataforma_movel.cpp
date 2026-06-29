#include "plataforma_movel.h"
#include <QLineF>
#include <QTimer>

PlataformaMovel::PlataformaMovel(
    qreal x,
    qreal y,
    qreal width,
    qreal height,
    qreal fimX,
    qreal fimY,
    qreal velocidade)
    : Plataforma(x, y, width, height),
      inicio(x, y),
      fim(fimX, fimY),
      destinoAtual(fim),
      deslocamentoUltimo(0, 0),
      velocidade(velocidade),
      timer(new QTimer(this))
{
    connect(timer, &QTimer::timeout, this, [this]()
            { atualizarMovimento(); });

    timer->start(16);
}

QPointF PlataformaMovel::deslocamentoUltimoFrame() const
{
    return deslocamentoUltimo;
}

void PlataformaMovel::atualizarMovimento()
{
    QPointF posicaoAtual = pos();
    QLineF caminho(posicaoAtual, destinoAtual);

    if (caminho.length() <= velocidade)
    {
        setPos(destinoAtual);
        deslocamentoUltimo = destinoAtual - posicaoAtual;
        destinoAtual = (destinoAtual == fim) ? inicio : fim;
        return;
    }

    qreal dx = caminho.dx() / caminho.length();
    qreal dy = caminho.dy() / caminho.length();

    QPointF novaPosicao(
        x() + dx * velocidade,
        y() + dy * velocidade);

    setPos(novaPosicao);
    deslocamentoUltimo = novaPosicao - posicaoAtual;
}
