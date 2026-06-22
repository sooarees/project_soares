#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>

#include "Player.h"
#include "plataforma.h"
#include "portal.h"

class Game : public QWidget
{
    Q_OBJECT

public:
    Game(QWidget *parent = nullptr);

private slots:
    void update();

private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    Player *knight;
    QTimer *gameTimer;

    int faseAtual;
    void carregarFase(int fase);
protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // GAME_H
