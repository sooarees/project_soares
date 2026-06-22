#include "Game.h"
#include <QBrush>

Game::Game()
{
    // Cena
    scene = new QGraphicsScene();
    scene->setBackgroundBrush(Qt::white);
    scene->setSceneRect(0,0,1920,1080);

    // Jogador
    knight = new Player(120,750);

    knight->setFlag(QGraphicsItem::ItemIsFocusable);
    knight->setFocus();

    scene->addItem(knight);

    // Fase
    faseAtual = 1;
    carregarFase(faseAtual);

    /* plataformas
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



*/

    // View
    view = new QGraphicsView();
    view->setScene(scene);

    //view->showMaximized();

    view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
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

    Portal *portal = new Portal(1800, 850,2);
    scene->addItem(portal);
    //scene->addItem(new Portal(300, 400, 5));
}

void Game::update()
{
    // movement
    knight->updateMovement();

    // portal
    for(QGraphicsItem *item : knight->collidingItems())
    {
        Portal *portal = dynamic_cast<Portal*>(item);

        if(portal)
        {
            carregarFase(portal->getDestino());
            break;
        }
    }

}

void Game::carregarFase(int fase)
{
    scene->removeItem(knight);
    scene->clear();

    switch(fase)
    {
    case 1:
        scene->addItem(new Plataforma(0,950,1920,130));
        scene->addItem(new Plataforma(500,800,70,20));

        scene->addItem(new Portal(1800,850,2));

        knight->setPos(120,750);
        scene->addItem(knight);

        break;


    case 2:
        scene->addItem(new Plataforma(0,950,1920,130));
        scene->addItem(new Plataforma(800,700,200,20));

        scene->addItem(new Portal(1800,850,3));

        knight->setPos(100,750);
        scene->addItem(knight);

        break;
    }

    knight->setFocus();
}