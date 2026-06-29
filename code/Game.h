#ifndef GAME_H
#define GAME_H

#include <QElapsedTimer>
#include <QWidget>

class Armadilha;
class Fase;
class HUD;
class QLabel;
class QAudioOutput;
class QGraphicsScene;
class QGraphicsView;
class QMediaPlayer;
class QPushButton;
class QResizeEvent;
class QTimer;
class Portal;
class Player;

class Game : public QWidget
{
    Q_OBJECT

public:
    Game(QWidget *parent = nullptr);
    ~Game() override;

private slots:
    void atualizarJogo();
    void atualizarCronometro();

private:
    QGraphicsScene *scene = nullptr;
    QGraphicsView *view = nullptr;
    Player *knight = nullptr;
    QTimer *gameTimer = nullptr;
    QTimer *cronometroTimer = nullptr;
    QElapsedTimer cronometro;
    QLabel *labelCronometro = nullptr;
    QMediaPlayer *soundtrack = nullptr;
    QAudioOutput *saidaAudio = nullptr;

    // fase
    Fase *fase = nullptr;
    int faseAtual = 1;
    void configurarCena();
    void configurarJogador();
    void configurarFaseInicial();
    void configurarView();
    void configurarHud();
    void configurarBotaoFechar();
    void configurarCronometro();
    void configurarSoundtrack();
    void iniciarGameLoop();
    void reposicionarInterface();
    void carregarFase(int numeroFase);
    void verificarColisoes();
    void verificarQueda();
    void colidirComPortal(Portal *portal);
    void colidirComPrincesa();
    void colidirComArmadilha(Armadilha *armadilha);

    // vidas
    void perderVida();
    void ganharVida();
    void ganharJogo();
    void reiniciarJogo();
    HUD *hud = nullptr;
    QPushButton *botaoFechar = nullptr;
    int vidas = 0;

protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // GAME_H
