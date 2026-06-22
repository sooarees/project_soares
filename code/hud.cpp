#include "hud.h"


HUD::HUD(QWidget *parent)
    : QLabel(parent)
{
    vidas = 5;

    setText("♥ ♥ ♥ ♥ ♥");

    setStyleSheet(
        "color:red;"
        "font-size:32px;"
        "font-weight:bold;"
        );
}

void HUD::setVidas(int novasVidas)
{
    vidas = novasVidas;

    atualizar();
}


void HUD::atualizar()
{
    QString texto;


    for(int i = 0; i < vidas; i++)
    {
        texto += "♥ ";
    }


    setText(texto);
}