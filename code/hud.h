#ifndef HUD_H
#define HUD_H

#include <QLabel>
#include <QWidget>

class QPaintEvent;

class HUD : public QLabel
{
public:
    HUD(QWidget *parent = nullptr);

    void setVidas(int vidas);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int vidas;

    void atualizar();
};

#endif
