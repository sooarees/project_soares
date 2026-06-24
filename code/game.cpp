#include "Game.h"
#include <QVBoxLayout>
#include <QBrush>

Game::Game(QWidget *parent): QWidget(parent)
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

    // View
    view = new QGraphicsView(this);
    view->setScene(scene);

    view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // ocupa todo espaço disponível
    view->setSizePolicy(
        QSizePolicy::Expanding,
        QSizePolicy::Expanding
        );

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(view);

    setLayout(layout);

    view->setSceneRect(scene->sceneRect());

    showFullScreen();
    //showMaximized();

    view->fitInView(
        scene->sceneRect(),
        Qt::KeepAspectRatio
        );

    // vidas
    vidas = 5;

    // hud
    hud = new HUD(this);

    hud->setGeometry(20,20,300,50);

    hud->show();
    hud->raise();

    // gameTimer
    gameTimer = new QTimer(this);

    connect(gameTimer, &QTimer::timeout, this, &Game::update);

    gameTimer->start(16); // 16ms aproximadamente 60fps, 60fps = 1000ms/16ms
}

void Game::update()
{
    // movement
    knight->updateMovement();

    for(QGraphicsItem *item : knight->collidingItems())
    {

        // portal
        Portal *portal = dynamic_cast<Portal*>(item);

        if(portal)
        {
            ganharVida();

            faseAtual = portal->getDestino();

            carregarFase(faseAtual);
            break;
        }
        // armadilha
        Armadilha *armadilha = dynamic_cast<Armadilha*>(item);

        if(armadilha)
        {
            perderVida();
            break;
        }
    }

    // "buraco"
    if(knight->y() > 1100)
    {
        perderVida();
    }

}

void Game::carregarFase(int fase)
{
    scene->removeItem(knight);
    scene->clear();

    // DEBUG VISIUAL
    scene->addRect(
        scene->sceneRect(),
        QPen(Qt::red, 2),
        QBrush(Qt::NoBrush)
        );

    switch(fase)
    {
    case 1:
    {
        // chão 1
        scene->addItem(new Plataforma(0,950,500,130));

        // chão 2
        scene->addItem(new Plataforma(700,950,1220,130));


        // plataforma inicial
        scene->addItem(new Plataforma(250,850,150,20));

        // subida
        scene->addItem(new Plataforma(500,750,120,20));
        scene->addItem(new Plataforma(700,650,120,20));

        // salto maior
        scene->addItem(new Plataforma(950,550,150,20));

        // descida
        scene->addItem(new Plataforma(1200,700,150,20));
        scene->addItem(new Plataforma(1450,600,120,20));


        // obstáculo vertical
        //scene->addItem(new Plataforma(900,750,60,200));


        // trecho final
        scene->addItem(new Plataforma(1600,500,200,20));


        // portal final
        scene->addItem(new Portal(1800,850,2));


        // espinho
        scene->addItem(new Espinho(900,910));

        // serra
        scene->addItem(new Serra(1100,900));

        // spawn
        knight->setPos(120,750);
        scene->addItem(knight);

        break;
    }

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

void Game::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    if(view)
    {
        view->fitInView(
            scene->sceneRect(),
            Qt::KeepAspectRatio
            );

        view->centerOn(scene->sceneRect().center());
    }

    if(hud && view)
    {
        int margemX = (width() - view->viewport()->width()) / 2;
        hud->move(margemX + 20,20);
    }
}

void Game::perderVida()
{
    vidas--;

    hud->setVidas(vidas);


    if(vidas <= 0)
    {
        vidas = 5;
        hud->setVidas(vidas);

        carregarFase(1);
    }
    else
    {
        carregarFase(faseAtual);
    }
}

void Game::ganharVida()
{
    if(vidas < 5)
    {
        vidas++;

        hud->setVidas(vidas);
    }
}
