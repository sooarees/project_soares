#include "hud.h"

#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>

HUD::HUD(QWidget *parent)
    : QLabel(parent)
{
    vidas = 5;

    setAttribute(Qt::WA_TranslucentBackground);
}

void HUD::setVidas(int novasVidas)
{
    vidas = novasVidas;

    atualizar();
}

void HUD::atualizar()
{
    update();
}

void HUD::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, false);

    QPixmap background(":/Sprites/Game Images/Royal/Hud/background.png");
    QPixmap vida(":/Sprites/Game Images/Royal/Hud/vida.png");

    painter.drawPixmap(rect(), background, background.rect());

    if(vidas <= 0)
    {
        return;
    }

    const int tamanhoVida = 32;
    const int espaco = 8;
    const int larguraTotalVidas = vidas * tamanhoVida + (vidas - 1) * espaco;
    const int inicioX = (width() - larguraTotalVidas) / 2;
    const int y = (height() - tamanhoVida) / 2;

    for(int i = 0; i < vidas; i++)
    {
        int x = inicioX + i * (tamanhoVida + espaco);

        painter.drawPixmap(
            QRect(x, y, tamanhoVida, tamanhoVida),
            vida,
            vida.rect()
            );
    }
}
