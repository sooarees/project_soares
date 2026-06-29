#ifndef HUD_H
#define HUD_H

#include <QLabel>

class QPaintEvent;
class QWidget;

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

#endif // HUD_H
