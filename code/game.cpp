#include "Game.h"
#include "tempo.h"
#include <QBrush>
#include <QFile>
#include <QFontDatabase>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QLabel>
#include <QPainter>
#include <QPaintEvent>
#include <QPen>
#include <QPixmap>
#include <QPushButton>
#include <QMessageBox>
#include <QSettings>
#include <QStringList>
#include <QVBoxLayout>

namespace // funcao valor so existe nesse arquivo
{
// (PARA O ARQUIVO JSON) pega o objeto onde vai rodar o for, chave eh oq eu quero ler por exemplo x,y,widght,height e se nao achar nada devolve 0
qreal valor(const QJsonObject &objeto, const QString &chave, qreal padrao = 0)
{
    return objeto.value(chave).toDouble(padrao); // devolve o valor da chave e transforma em double
}

const int larguraCena = 1920;
const int alturaCena = 1080;
const int spawnInicialX = 120;
const int spawnInicialY = 750;
const int vidasMaximas = 5;
const int limiteQueda = 1030;
const int frameMs = 16;
const int cronometroFrameMs = 30;

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
    // Cena
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,larguraCena,alturaCena);

    QPixmap background(":/Sprites/Game Images/Royal/Castle/background.png");
    scene->setBackgroundBrush(QBrush(background)); // repete a imagem para preencher a cena

    // Jogador
    knight = new Player(spawnInicialX,spawnInicialY);

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
    vidas = vidasMaximas;

    // hud
    hud = new HUD(this);

    hud->setGeometry(20,20,220,60);

    hud->show();
    hud->raise();

    // botao temporario para fechar o jogo
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
    botaoFechar->move((width() - botaoFechar->width()) / 2,20);
    botaoFechar->show();
    botaoFechar->raise();

    connect(botaoFechar, &QPushButton::clicked, this, &Game::close);

    // cronometro
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

    labelCronometro->move(width() - labelCronometro->width() - 20,20);
    labelCronometro->show();
    labelCronometro->raise();

    cronometro.start();
    cronometroTimer = new QTimer(this);
    connect(cronometroTimer, &QTimer::timeout, this, &Game::atualizarCronometro);
    cronometroTimer->start(cronometroFrameMs);
    atualizarCronometro();

    // gameTimer
    gameTimer = new QTimer(this);

    connect(gameTimer, &QTimer::timeout, this, &Game::update);

    gameTimer->start(frameMs); // aproximadamente 60 FPS
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

        // princesa
        Princesa *princesa = dynamic_cast<Princesa*>(item);

        if(princesa)
        {
            ganharJogo();
            break;
        }

        // armadilha
        Armadilha *armadilha = dynamic_cast<Armadilha*>(item);

        if(armadilha && armadilha->causaDano())
        {
            perderVida();
            break;
        }
    }

    // "buraco"
    if(knight->y() > limiteQueda)
    {
        perderVida();
    }
}

void Game::carregarFase(int fase)
{
    knight->limparPlataformaMovel();
    scene->removeItem(knight);
    scene->clear();

    QFile arquivo(":/Fases/fases.json");
    if(!arquivo.open(QIODevice::ReadOnly)) // se nao abrir, bota ele em uma posicao padrao
    {
        knight->setPos(spawnInicialX,spawnInicialY);
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

    for(const QJsonValue &valorPlataforma : dadosFase.value("plataformas_moveis").toArray())
    {
        QJsonObject plataforma = valorPlataforma.toObject();
        scene->addItem(new PlataformaMovel(
            valor(plataforma, "x"),
            valor(plataforma, "y"),
            valor(plataforma, "width"),
            valor(plataforma, "height"),
            valor(plataforma, "fimX"),
            valor(plataforma, "fimY"),
            valor(plataforma, "velocidade", 2)
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

    QJsonObject princesa = dadosFase.value("princesa").toObject();
    if(!princesa.isEmpty())
    {
        scene->addItem(new Princesa(
            valor(princesa, "x"),
            valor(princesa, "y")
            ));
    }

    // le o spawn e converte em objeto
    QJsonObject spawn = dadosFase.value("spawn").toObject();
    knight->setPos(valor(spawn, "x", spawnInicialX), valor(spawn, "y", spawnInicialY));
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

