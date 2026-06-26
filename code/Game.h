#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QElapsedTimer>
#include <QString>

class QLabel;
class QPushButton;

#include "player.h"
#include "plataforma.h"
#include "portal.h"
#include "hud.h"
#include "armadilha.h"
#include "espinho.h"
#include "serra.h"
#include "princesa.h"

class Game : public QWidget
{
    Q_OBJECT

public:
    Game(QWidget *parent = nullptr);

private slots:
    void update();
    void atualizarCronometro();

private:
    QGraphicsScene *scene;
    QGraphicsView *view = nullptr;
    Player *knight;
    QTimer *gameTimer;
    QTimer *cronometroTimer = nullptr;
    QElapsedTimer cronometro;
    QLabel *labelCronometro = nullptr;

    // fase
    int faseAtual;
    void carregarFase(int fase);

    // vidas
    void perderVida();
    void ganharVida();
    void ganharJogo();
    QString formatarTempo(qint64 milissegundos) const;
    HUD *hud = nullptr;
    QPushButton *botaoFechar = nullptr;
    int vidas;
protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // GAME_H
