#include "plataforma.h"
#include <QBrush>
#include <QPainter>
#include <QPen>
#include <QPixmap>
#include <QString>
#include <QtGlobal>

namespace
{
const int tamanhoTextura = 48; // aumenta a textura 16x16 para nao repetir demais

QPixmap textura(const QString &caminho)
{
    QPixmap imagem(caminho);
    return imagem.scaled(
        tamanhoTextura,
        tamanhoTextura,
        Qt::IgnoreAspectRatio,
        Qt::FastTransformation
        );
}

void desenharTextura(QPainter *painter, const QRectF &area, const QPixmap &textura)
{
    for(qreal y = area.top(); y < area.bottom(); y += tamanhoTextura)
    {
        for(qreal x = area.left(); x < area.right(); x += tamanhoTextura)
        {
            QRectF destino(x, y, tamanhoTextura, tamanhoTextura);
            painter->drawPixmap(destino, textura, QRectF(0,0,textura.width(),textura.height()));
        }
    }
}
}

Plataforma::Plataforma(qreal x, qreal y, qreal width, qreal height) : ElementoFase(x, y, width, height)
{
    setPen(Qt::NoPen);
}

void Plataforma::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->save();

    QPixmap platUp = textura(":/Sprites/Game Images/Royal/Castle/plat_up.png");
    QPixmap platMid = textura(":/Sprites/Game Images/Royal/Castle/plat_mid.png");
    QPixmap platEsq = textura(":/Sprites/Game Images/Royal/Castle/plat_esq.png");
    QPixmap platDir = textura(":/Sprites/Game Images/Royal/Castle/plat.dir.png");
    QPixmap platDown = textura(":/Sprites/Game Images/Royal/Castle/plat_down.png");

    QRectF area = rect();
    painter->setClipRect(area);

    qreal larguraBorda = qMin<qreal>(tamanhoTextura, area.width() / 2);
    qreal alturaBorda = qMin<qreal>(tamanhoTextura, area.height() / 2);

    QRectF meio(area.x(), area.y(), area.width(), area.height());
    QRectF topo(area.x(), area.y(), area.width(), alturaBorda);
    QRectF baixo(area.x(), area.bottom() - alturaBorda, area.width(), alturaBorda);
    QRectF esquerda(area.x(), area.y() + alturaBorda, larguraBorda, area.height() - alturaBorda * 2);
    QRectF direita(area.right() - larguraBorda, area.y() + alturaBorda, larguraBorda, area.height() - alturaBorda * 2);

    desenharTextura(painter, meio, platMid); // miolo da plataforma

    if(esquerda.height() > 0)
    {
        desenharTextura(painter, esquerda, platEsq); // borda esquerda
        desenharTextura(painter, direita, platDir); // borda direita
    }

    desenharTextura(painter, topo, platUp); // borda de cima, incluindo os cantos
    desenharTextura(painter, baixo, platDown); // borda de baixo, incluindo os cantos

    painter->restore();
}
