#include "Game.h"
#include <QBrush>

Game::Game()
{
    // Cena
    scene = new QGraphicsScene();
    scene->setBackgroundBrush(Qt::white);
    scene->setSceneRect(0,0,1920,1080);

    // plataformas
    Plataforma *ground = new Plataforma(0,950,1920,180);
    scene->addItem(ground);

    Plataforma *plat5 = new Plataforma(850,800,70,20);
    scene->addItem(plat5);

    Plataforma *plat1 = new Plataforma(500,800,70,20);
    scene->addItem(plat1);

    Plataforma *plat2 = new Plataforma(800,340,150,20);
    scene->addItem(plat2);

    Plataforma *plat3 = new Plataforma(1200,200,200,20);
    scene->addItem(plat3);

    Plataforma *plat4 = new Plataforma(650,750,59,200);
    scene->addItem(plat4);


    // Jogador
    knight = new Player(120,750);

    knight->setFlag(QGraphicsItem::ItemIsFocusable);
    knight->setFocus();

    scene->addItem(knight);

    // View
    view = new QGraphicsView();
    view->setScene(scene);
    //view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //view->showFullScreen(); //pra jogar melhor pra testar n
    view->showMaximized();

    //view->resize(1920,1080);
    view->show();

    // gameTimer
    gameTimer = new QTimer(this);

    connect(gameTimer, &QTimer::timeout, this, &Game::update);

    gameTimer->start(16); // 16ms aproximadamente 60fps, 60fps = 1000ms/16ms
}

void Game::update()
{
    // player center
    knight->updateMovement();
}