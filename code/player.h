#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsRectItem>
#include <QObject>
#include <QKeyEvent>
#include <QPainterPath>
#include <QTimer>
#include "elementofase.h"

class PlataformaMovel;

class Player: public QObject, public ElementoFase
{
    Q_OBJECT

public:
    Player (qreal x, qreal y);

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void updateMovement();
    void limparPlataformaMovel();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QRectF boundingRect() const override;
    QPainterPath shape() const override;

private slots:
    void advanceAnimation();

private:

    // movimentacao
    bool leftPressed;
    bool rightPressed;
    bool downPressed;

    qreal veloX;
    qreal veloY;

    bool onGround;
    int coyoteFramesRestantes;
    PlataformaMovel *plataformaMovelAtual;

    // timers
    QTimer *animTimer;

    // graficos
    QPixmap spriteSheetIdle;
    QPixmap spriteSheetRun;
    QPixmap spriteSheetJump;

    // animacao
    int currentFrame;
    int totalFrames;
    int frameWidth;
    int frameHeight;
    bool isLooping;

    // status para saber qual animacao tocar
    enum State {Idle, Running, Jumping };
    State currentState;

    // trocar de sprite
    void setAnimation (State newState);
    QRectF hitbox() const;

    // guardar direcao do knight
    bool facingRight;

};


#endif // PLAYER_H
