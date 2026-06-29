#include "Game.h"
#include "armadilha.h"
#include "fase.h"
#include "portal.h"
#include "princesa.h"
#include "tempo.h"
#include <QBrush>
#include <QFontDatabase>
#include <QLabel>
#include <QMediaPlayer>
#include <QPainter>
#include <QPaintEvent>
#include <QPen>
#include <QPixmap>
#include <QPushButton>
#include <QAudioOutput>
#include <QMessageBox>
#include <QSettings>
#include <QStringList>
#include <QUrl>
#include <QVBoxLayout>

namespace
{
const int larguraCena = 1920;
const int alturaCena = 1080;
const int vidasMaximas = 5;
const int limiteQueda = 1030;
const int frameMs = 16;
const int cronometroFrameMs = 30;
const qreal volumeSoundtrack = 0.2;
const char *caminhoSoundtrack = "qrc:/Audio/SwitchWithMeTheme.wav";

class CronometroLabel : public QLabel
{
public:
    explicit CronometroLabel(QWidget *parent = nullptr)
        : QLabel(parent)
    {
        setAttribute(Qt::WA_TranslucentBackground);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        Q_UNUSED(event);

        QPainter painter(this);
        painter.setRenderHint(QPainter::SmoothPixmapTransform, false);

        QPixmap background(":/Sprites/Game Images/Royal/Hud/background.png");
        painter.drawPixmap(rect(), background, background.rect());

        painter.setPen(QColor("#ffd75a"));
        painter.setFont(font());
        painter.drawText(rect(), alignment(), text());
    }
};
}

Game::Game(QWidget *parent): QWidget(parent)
{
    configurarCena();
    configurarJogador();
    configurarFaseInicial();
    configurarView();

    showFullScreen();
    //showMaximized();

    configurarHud();
    configurarBotaoFechar();
    configurarCronometro();
    configurarSoundtrack();
    iniciarGameLoop();
    reposicionarInterface();
}

Game::~Game()
{
    delete fase;
}

void Game::configurarCena()
{
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0,0,larguraCena,alturaCena);

    QPixmap background(":/Sprites/Game Images/Royal/Castle/background.png");
    scene->setBackgroundBrush(QBrush(background)); // repete a imagem para preencher a cena
}

void Game::configurarJogador()
{
    knight = new Player(Fase::spawnPadraoX, Fase::spawnPadraoY);

    knight->setFlag(QGraphicsItem::ItemIsFocusable);
    knight->setFocus();

    scene->addItem(knight);
}

void Game::configurarFaseInicial()
{
    fase = new Fase(scene, knight);
    faseAtual = 1;
    carregarFase(faseAtual);
}

void Game::configurarView()
{
    view = new QGraphicsView(this);
    view->setScene(scene);
    view->setSceneRect(scene->sceneRect());

    view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(view);

    setLayout(layout);
}

void Game::configurarHud()
{
    vidas = vidasMaximas;

    hud = new HUD(this);
    hud->setGeometry(20,20,220,60);
    hud->show();
    hud->raise();
}

void Game::configurarBotaoFechar()
{
    botaoFechar = new QPushButton("X", this);
    botaoFechar->setFixedSize(28,28);
    botaoFechar->setFocusPolicy(Qt::NoFocus);
    botaoFechar->setStyleSheet(
        "QPushButton {"
        "background-color: rgba(20, 20, 20, 180);"
        "border: 2px solid white;"
        "color: white;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "background-color: rgba(180, 40, 40, 220);"
        "}"
        );
    botaoFechar->show();
    botaoFechar->raise();

    connect(botaoFechar, &QPushButton::clicked, this, &Game::close);
}

void Game::configurarCronometro()
{
    labelCronometro = new CronometroLabel(this);
    labelCronometro->setFixedSize(220,60);
    labelCronometro->setAlignment(Qt::AlignCenter);

    int idFonteCronometro = QFontDatabase::addApplicationFont(":/Sprites/Game Images/Royal/Menu/GravityBold8.ttf");
    QString familiaCronometro = "Georgia";

    if(idFonteCronometro != -1)
    {
        QStringList familias = QFontDatabase::applicationFontFamilies(idFonteCronometro);

        if(!familias.isEmpty())
            familiaCronometro = familias.first();
    }

    QFont fonteCronometro(familiaCronometro);
    fonteCronometro.setPointSize(16);
    fonteCronometro.setBold(true);
    labelCronometro->setFont(fonteCronometro);

    labelCronometro->show();
    labelCronometro->raise();

    cronometro.start();
    cronometroTimer = new QTimer(this);
    connect(cronometroTimer, &QTimer::timeout, this, &Game::atualizarCronometro);
    cronometroTimer->start(cronometroFrameMs);
    atualizarCronometro();
}

void Game::configurarSoundtrack()
{
    saidaAudio = new QAudioOutput(this);
    saidaAudio->setVolume(volumeSoundtrack);

    soundtrack = new QMediaPlayer(this);
    soundtrack->setAudioOutput(saidaAudio);
    soundtrack->setSource(QUrl(caminhoSoundtrack));
    soundtrack->setLoops(QMediaPlayer::Infinite);
    soundtrack->play();
}

void Game::iniciarGameLoop()
{
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &Game::atualizarJogo);
    gameTimer->start(frameMs); // aproximadamente 60 FPS
}

void Game::reposicionarInterface()
{
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
        hud->raise();
    }

    if(labelCronometro)
    {
        labelCronometro->move(width() - labelCronometro->width() - 20,20);
        labelCronometro->raise();
    }

    if(botaoFechar)
    {
        botaoFechar->move((width() - botaoFechar->width()) / 2,20);
        botaoFechar->raise();
    }
}

void Game::atualizarJogo()
{
    knight->updateMovement();

    verificarColisoes();
    verificarQueda();
}

void Game::verificarColisoes()
{
    for(QGraphicsItem *item : knight->collidingItems())
    {
        Portal *portal = dynamic_cast<Portal*>(item);
        if(portal)
        {
            colidirComPortal(portal);
            return;
        }

        Princesa *princesa = dynamic_cast<Princesa*>(item);
        if(princesa)
        {
            colidirComPrincesa();
            return;
        }

        Armadilha *armadilha = dynamic_cast<Armadilha*>(item);
        if(armadilha)
        {
            colidirComArmadilha(armadilha);
            return;
        }
    }
}

void Game::colidirComPortal(Portal *portal)
{
    ganharVida();

    faseAtual = portal->getDestino();
    carregarFase(faseAtual);
}

void Game::colidirComPrincesa()
{
    ganharJogo();
}

void Game::colidirComArmadilha(Armadilha *armadilha)
{
    if(armadilha->causaDano())
        perderVida();
}

void Game::verificarQueda()
{
    if(knight->y() > limiteQueda)
    {
        perderVida();
    }
}

void Game::carregarFase(int numeroFase)
{
    faseAtual = fase->carregar(numeroFase);
}

void Game::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    reposicionarInterface();
}

void Game::perderVida()
{
    vidas--;

    if(vidas <= 0)
    {
        reiniciarJogo();
        return;
    }

    hud->setVidas(vidas);
    carregarFase(faseAtual);
}

void Game::ganharVida()
{
    if(vidas < 5)
    {
        vidas++;

        hud->setVidas(vidas);
    }
}

void Game::ganharJogo()
{
    gameTimer->stop();
    if(cronometroTimer)
        cronometroTimer->stop();

    knight->clearFocus();
    knight->setFlag(QGraphicsItem::ItemIsFocusable, false);

    qint64 tempoFinalMs = cronometro.elapsed();
    QString tempoFinal = formatarTempo(tempoFinalMs);
    if(labelCronometro)
        labelCronometro->setText(tempoFinal);

    QSettings configuracoes("Royal Knight", "Royal Knight");
    qint64 melhorTempoMs = configuracoes.value("melhorTempoMs", -1).toLongLong();
    bool novoMelhorTempo = melhorTempoMs < 0 || tempoFinalMs < melhorTempoMs;

    if(novoMelhorTempo)
        configuracoes.setValue("melhorTempoMs", tempoFinalMs);

    QMessageBox::information(
        this,
        "Royal Knight",
        QString("Voce resgatou a princesa!\nTempo: ") + tempoFinal +
            (novoMelhorTempo ? "\nNovo melhor tempo!" : "")
        );

    close();
}

void Game::reiniciarJogo()
{
    faseAtual = 1;
    vidas = vidasMaximas;

    hud->setVidas(vidas);
    carregarFase(faseAtual);

    cronometro.restart();
    atualizarCronometro();
}

void Game::atualizarCronometro()
{
    if(!labelCronometro)
        return;

    labelCronometro->setText(formatarTempo(cronometro.elapsed()));
}
