#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "Player.h"
#include "plataforma.h"
#include "portal.h"

class Game : public QObject
{
    Q_OBJECT

public:
    Game();

private slots:
    void update();

private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    Player *knight;
    QTimer *gameTimer;

    int faseAtual;

    void carregarFase(int fase);
};

#endif // GAME_H
