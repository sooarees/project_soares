#include "Game.h"
#include <QBrush>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QPen>
#include <QVBoxLayout>

namespace // funcao valor so existe nesse arquivo
{
// (PARA O ARQUIVO JSON) pega o objeto onde vai rodar o for, chave eh oq eu quero ler por exemplo x,y,widght,height e se nao achar nada devolve 0
qreal valor(const QJsonObject &objeto, const QString &chave, qreal padrao = 0)
{
    return objeto.value(chave).toDouble(padrao); // devolve o valor da chave e transforma em double
}
}

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

    // Ocupa todo espaco disponivel
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

    // DEBUG VISUAL
    scene->addRect(
        scene->sceneRect(),
        QPen(Qt::red, 2),
        QBrush(Qt::NoBrush)
        );

    QFile arquivo(":/Fases/fases.json");
    if(!arquivo.open(QIODevice::ReadOnly)) // se nao abrir, bota ele em uma posicao padrao
    {
        knight->setPos(120,750);
        scene->addItem(knight);
        knight->setFocus();
        return;
    }

    // le o arquivo todo readAll ee converte pra um objeto Qt
    QJsonDocument documento = QJsonDocument::fromJson(arquivo.readAll());

    // abre o documento (objeto), pega o valor de fases e converte pra um objeto Qt
    QJsonObject fases = documento.object().value("fases").toObject();

    // procura a fase desejada com base no valor de int fase e coloca os valores em dados fase como um objeto qt
    QJsonObject dadosFase = fases.value(QString::number(fase)).toObject();


    // checa se ta vazio e retorna pra fase 1
    if(dadosFase.isEmpty())
    {
        faseAtual = 1;
        dadosFase = fases.value("1").toObject();
    }

    // pega o valor de plataformas e converte pra um array (toArray)
    for(const QJsonValue &valorPlataforma : dadosFase.value("plataformas").toArray())
    {
        // transforma em um objeto qt e varre ele usando a funcao valor
        QJsonObject plataforma = valorPlataforma.toObject();
        scene->addItem(new Plataforma(
            valor(plataforma, "x"),
            valor(plataforma, "y"),
            valor(plataforma, "width"),
            valor(plataforma, "height")
            ));
    }

    for(const QJsonValue &valorPortal : dadosFase.value("portais").toArray())
    {
        QJsonObject portal = valorPortal.toObject();
        scene->addItem(new Portal(
            valor(portal, "x"),
            valor(portal, "y"),
            portal.value("destino").toInt(1)
            ));
    }

    for(const QJsonValue &valorArmadilha : dadosFase.value("armadilhas").toArray())
    {
        QJsonObject armadilha = valorArmadilha.toObject();
        QString tipo = armadilha.value("tipo").toString();

        if(tipo == "espinho")
            scene->addItem(new Espinho(valor(armadilha, "x"), valor(armadilha, "y")));
        else if(tipo == "serra")
            scene->addItem(new Serra(valor(armadilha, "x"), valor(armadilha, "y")));
    }

    // le o spawn e converte em objeto
    QJsonObject spawn = dadosFase.value("spawn").toObject();
    knight->setPos(valor(spawn, "x", 120), valor(spawn, "y", 750));
    scene->addItem(knight);
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
