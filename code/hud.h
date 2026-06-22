#ifndef HUD_H
#define HUD_H

#include <QLabel>
#include <QWidget>


class HUD : public QLabel
{
public:
    HUD(QWidget *parent = nullptr);

    void setVidas(int vidas);

private:
    int vidas;

    void atualizar();
};

#endif