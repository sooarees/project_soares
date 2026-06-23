#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>

#include "Player.h"
#include "plataforma.h"
#include "portal.h"
#include "hud.h"
#include "armadilha.h"
#include "espinho.h"

class Game : public QWidget
{
    Q_OBJECT

public:
    Game(QWidget *parent = nullptr);

private slots:
    void update();

private:
    QGraphicsScene *scene;
    QGraphicsView *view = nullptr;;
    Player *knight;
    QTimer *gameTimer;

    // fase
    int faseAtual;
    void carregarFase(int fase);

    // vidas
    void perderVida();
    void ganharVida();
    HUD *hud = nullptr;;
    int vidas;
protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // GAME_H
