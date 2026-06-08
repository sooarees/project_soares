#include "Player.h"
#include "elementofase.h"
#include "plataforma.h"
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QPainter>
//#include <utility>

Player::Player(qreal x, qreal y): ElementoFase(x,y,51,99)
{
    // iniciando movimentacoes
    leftPressed = false;
    rightPressed = false;
    downPressed = false;

    veloX = 0;
    veloY = 0;

    onGround = false;
    facingRight = true; // comeca olhando pra direita

    // carrega as spritesheets
    spriteSheetIdle.load(":/Sprites/Game Images/Royal/Knight/IDLE.png");
    spriteSheetRun.load(":/Sprites/Game Images/Royal/Knight/RUN.png");
    spriteSheetJump.load(":/Sprites/Game Images/Royal/Knight/JUMP.png");

    // timer animacoes
    animTimer = new QTimer(this);
    connect(animTimer, &QTimer::timeout, this, &Player::advanceAnimation);

    // iniciando animacoes
    currentState = Idle;
    setAnimation(Idle);
    animTimer->start(165); // muda o frame a cada 165ms
}

void Player::setAnimation(State newState)
{
    if (currentState == newState && animTimer->isActive())
    {
        return;
    }

    // troca o estado e reinicia o frame
    currentState = newState;
    currentFrame = 0;

    // configura para cada sprite
    switch (currentState) {
    case Idle:
        frameWidth = 96; frameHeight = 84; totalFrames = 3; isLooping = true; //224
        animTimer->setInterval(165);
        break;
    case Running:
        frameWidth = 96; frameHeight = 84; totalFrames = 8; isLooping = true;
        animTimer->setInterval(100);
        break;
    case Jumping:
        frameWidth = 96; frameHeight = 84; totalFrames = 5; isLooping = false;
        animTimer->setInterval(80);
        break;
    }
}

void Player::advanceAnimation()
{
    // avanca os frames e checa se esta em looping
    currentFrame++;
    if (currentFrame >= totalFrames) {
        if (isLooping) {
            currentFrame = 0;
        } else {
            currentFrame = totalFrames - 1;
        }
    }

    // força o Qt a redesenhar o Player chamando a função paint() no próximo ciclo
    update();
}

void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QPixmap currentSheet;

    // seleciona o sprite
    switch (currentState)
    {
    case Idle:
        currentSheet = spriteSheetIdle; break;
    case Running:
        currentSheet = spriteSheetRun;  break;
    case Jumping:
        currentSheet = spriteSheetJump; break;
    }

    int sourceX = currentFrame * frameWidth;
    QRect sourceRect(sourceX, 0, frameWidth, frameHeight);

    // define o tamanho visual
    qreal visualWidth = 240;
    qreal visualHeight = 210;

    // ajuste de offset horizontal
    qreal ajustemeio = 35;
    qreal offsetX = (rect().width() - visualWidth) / 2;
    // ajuste de offset vertical
    qreal ajusteDeChao = 55;
    qreal offsetY = (rect().height() - visualHeight) + ajusteDeChao;

    painter->save();

    // espelha o player
    if (!facingRight)
    {
        painter->translate(visualWidth + offsetX * 2, 0);
        painter->scale(-1, 1);
    }

    // desenha o sprite com o tamanho correto, ignorando o tamanho da hitbox
    painter->drawPixmap(QRectF(offsetX, offsetY, visualWidth, visualHeight), currentSheet, sourceRect);

    painter->restore();
}

// corrige os "fantasmas" das animacoes
QRectF Player::boundingRect() const
{
    qreal visualWidth = 240;
    qreal visualHeight = 210;

    qreal offsetX = (rect().width() - visualWidth) / 2;
    qreal offsetY = rect().height() - visualHeight;

    return QRectF(offsetX, offsetY, visualWidth, visualHeight);
}

// movimentacao
void Player::keyPressEvent(QKeyEvent *event)
{
    // esquerda
    if (event->key() == Qt::Key_A){
        leftPressed = true;
    }
    // direita
    else if (event->key() == Qt::Key_D){
        rightPressed = true;
    }
    // jump
    if(event->key() == Qt::Key_Space && onGround)
    {
        veloY = -15;
        onGround = false;
    }
    // fast Fall
    if(event->key() == Qt::Key_S && !onGround)
    {
        downPressed = true;
    }
/* DOUBLE JUMP - PRECISA POR UMA VARIAVEL QUE VERIFICA E SÓ DEIXA 1 DOUBLE JUMP
    if(event->key() == Qt::Key_Space && !onGround)
    {
        veloY = -18;
    }
*/
}

void Player::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_A)
        leftPressed = false;

    if(event->key() == Qt::Key_D)
        rightPressed = false;

    if(event->key() == Qt::Key_S)
        downPressed = false;
}

void Player::updateMovement()
{
    const qreal gravity = 0.6;
    const qreal speed   = 6;

    // horizontal
    veloX = 0;
    if(leftPressed)
    {
        veloX = -speed;
        facingRight = false;
    }
    if(rightPressed)
    {
        veloX = speed;
        facingRight = true;
    }

    // salva bordas antes de mover em X
    qreal prevRight = x() + rect().width();
    qreal prevLeft  = x();

    setX(x() + veloX);

    for(QGraphicsItem *item : collidingItems())
    {
        Plataforma *plat = dynamic_cast<Plataforma*>(item);
        if(!plat) continue;

        qreal platLeft  = plat->x();
        qreal platRight = plat->x() + plat->rect().width();

        // player vindo da esquerda, empurra pra fora a esquerda
        if(prevRight <= platLeft && veloX >0)
            setX(platLeft - rect().width());
        // player vindo da direita, empurra pra fora a direita
        else if(prevLeft >= platRight && veloX < 0)
            setX(platRight);
    }

    // vertical

    // fast-fall
    if(downPressed && !onGround)
        veloY += 0.85;

    // gravidade
    veloY += gravity;

    // salva bordas antes de mover em Y
    qreal prevBottom = y() + rect().height();
    qreal prevTop    = y();

    setY(y() + veloY);

    onGround = false;

    for(QGraphicsItem *item : collidingItems())
    {
        Plataforma *plat = dynamic_cast<Plataforma*>(item);
        if(!plat) continue;

        //salva a posicao da plataforma
        qreal platTop    = plat->y();
        qreal platBottom = plat->y() + plat->rect().height();

        // player estava acima, pousa no topo
        if(prevBottom <= platTop && veloY > 0)
        {
            setY(platTop - rect().height());
            veloY = 0;
            onGround = true;
        }
        // Player estava abaixo, bate no teto
        else if(prevTop >= platBottom && veloY < 0)
        {
            setY(platBottom);
            veloY = 0;
            onGround = false;
        }
    }

    // Animacoes
    State nextState;

    if (!onGround)
    {
        nextState = Jumping;
    }
    else
    {
        if (veloX == 0)
        {
            nextState = Idle;
        }
        else
        {
            nextState = Running;
        }
    }

    // só chama a função de animação se o estado mudou
    if (nextState != currentState) {
        setAnimation(nextState);
    }
}
