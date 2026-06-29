#include "player.h"
#include "elementofase.h"
#include "plataforma.h"
#include "plataforma_movel.h"
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QPainter>
#include <QTimer>

namespace
{
const qreal gravidade = 0.6;
const qreal veloHorizontal = 6;
const qreal forcaPulo = -15;
const qreal forcaQuedaRapida = 0.85;
const int coyoteFrames = 8;
const qreal fatorPlataformaMovelAndando = 0.0;
} // namespace

Player::Player(qreal x, qreal y)
    : ElementoFase(x, y, 51, 96)
{
    definirHitbox(QRectF(rect().x(), rect().y() + 5, rect().width(), rect().height() - 5));

    // iniciando movimentacoes
    leftPressed = false;
    rightPressed = false;
    downPressed = false;

    veloX = 0;
    veloY = 0;

    onGround = false;
    coyoteFramesRestantes = 0;
    plataformaMovelAtual = nullptr;
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
    switch (currentState)
    {
    case Idle:
        frameWidth = 96;
        frameHeight = 84;
        totalFrames = 3;
        isLooping = true;
        animTimer->setInterval(165);
        break;
    case Running:
        frameWidth = 96;
        frameHeight = 84;
        totalFrames = 8;
        isLooping = true;
        animTimer->setInterval(100);
        break;
    case Jumping:
        frameWidth = 96;
        frameHeight = 84;
        totalFrames = 5;
        isLooping = false;
        animTimer->setInterval(80);
        break;
    }
}

void Player::advanceAnimation()
{
    // avanca os frames e checa se esta em looping
    currentFrame++;
    if (currentFrame >= totalFrames)
    {
        if (isLooping)
        {
            currentFrame = 0;
        }
        else
        {
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
        currentSheet = spriteSheetIdle;
        break;
    case Running:
        currentSheet = spriteSheetRun;
        break;
    case Jumping:
        currentSheet = spriteSheetJump;
        break;
    }

    int sourceX = currentFrame * frameWidth;
    QRect sourceRect(sourceX, 0, frameWidth, frameHeight);

    // define o tamanho visual
    qreal visualWidth = 240;
    qreal visualHeight = 210;

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
    painter->drawPixmap(
        QRectF(offsetX, offsetY, visualWidth, visualHeight),
        currentSheet,
        sourceRect);

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
    if (event->key() == Qt::Key_A)
    {
        leftPressed = true;
    }
    // direita
    else if (event->key() == Qt::Key_D)
    {
        rightPressed = true;
    }
    // jump
    if (event->key() == Qt::Key_Space && coyoteFramesRestantes > 0)
    {
        veloY = forcaPulo;
        onGround = false;
        coyoteFramesRestantes = 0;
    }
    // fast Fall
    if (event->key() == Qt::Key_S && !onGround)
    {
        downPressed = true;
    }
}

void Player::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_A)
        leftPressed = false;

    if (event->key() == Qt::Key_D)
        rightPressed = false;

    if (event->key() == Qt::Key_S)
        downPressed = false;
}

void Player::updateMovement()
{
    acompanharPlataformaMovel();
    plataformaMovelAtual = nullptr;

    moverHorizontalmente();
    moverVerticalmente();
    limitarDentroDaCena();
    atualizarEstadoAnimacao();
}

void Player::acompanharPlataformaMovel()
{
    if (onGround && plataformaMovelAtual)
    {
        QPointF deslocamento = plataformaMovelAtual->deslocamentoUltimoFrame();
        bool andandoHorizontalmente = leftPressed || rightPressed;

        if (andandoHorizontalmente)
            deslocamento.setX(deslocamento.x() * fatorPlataformaMovelAndando);

        setPos(pos() + deslocamento);
    }
}

void Player::atualizarVelocidadeHorizontal()
{
    veloX = 0;

    if (leftPressed)
    {
        veloX = -veloHorizontal;
        facingRight = false;
    }
    if (rightPressed)
    {
        veloX = veloHorizontal;
        facingRight = true;
    }
}

void Player::moverHorizontalmente()
{
    atualizarVelocidadeHorizontal();

    qreal prevRight = x() + hitbox().right();
    qreal prevLeft = x() + hitbox().left();

    setX(x() + veloX);

    for (QGraphicsItem *item : collidingItems())
    {
        Plataforma *plat = dynamic_cast<Plataforma *>(item);
        if (!plat)
            continue;

        qreal platLeft = plat->x();
        qreal platRight = plat->x() + plat->rect().width();

        // player vindo da esquerda, empurra pra fora a esquerda
        if (prevRight <= platLeft && veloX > 0)
            setX(platLeft - hitbox().right());
        // player vindo da direita, empurra pra fora a direita
        else if (prevLeft >= platRight && veloX < 0)
            setX(platRight - hitbox().left());
    }
}

void Player::moverVerticalmente()
{
    if (downPressed && !onGround)
        veloY += forcaQuedaRapida;

    veloY += gravidade;

    qreal prevBottom = y() + hitbox().bottom();
    qreal prevTop = y() + hitbox().top();

    setY(y() + veloY);

    bool estavaNoChao = onGround;
    onGround = false;

    for (QGraphicsItem *item : collidingItems())
    {
        Plataforma *plat = dynamic_cast<Plataforma *>(item);
        if (!plat)
            continue;

        qreal platTop = plat->y();
        qreal platBottom = plat->y() + plat->rect().height();

        // player estava acima, pousa no topo
        if (prevBottom <= platTop && veloY > 0)
        {
            setY(platTop - hitbox().bottom());
            veloY = 0;
            onGround = true;
            coyoteFramesRestantes = coyoteFrames;
            plataformaMovelAtual = dynamic_cast<PlataformaMovel *>(plat);
        }
        // Player estava abaixo, bate no teto
        else if (prevTop >= platBottom && veloY < 0)
        {
            setY(platBottom - hitbox().top());
            veloY = 0;
            onGround = false;
            coyoteFramesRestantes = 0;
        }
    }

    atualizarCoyoteTime(estavaNoChao);
}

void Player::atualizarCoyoteTime(bool estavaNoChao)
{
    if (!onGround && estavaNoChao && coyoteFramesRestantes <= 0)
    {
        coyoteFramesRestantes = coyoteFrames;
    }

    if (!onGround && coyoteFramesRestantes > 0)
    {
        coyoteFramesRestantes--;
    }
}

void Player::limitarDentroDaCena()
{
    if (scene())
    {
        QRectF limites = scene()->sceneRect();

        // esquerda
        if (x() + hitbox().left() < limites.left())
            setX(limites.left() - hitbox().left());

        // direita
        if (x() + hitbox().right() > limites.right())
            setX(limites.right() - hitbox().right());

        // topo
        if (y() + hitbox().top() < limites.top())
        {
            setY(limites.top() - hitbox().top());
            veloY = 0;
        }

        // caiu fora da fase
        if (y() > limites.bottom())
        {
            veloY = 0;
            onGround = false;
        }
    }
}

void Player::atualizarEstadoAnimacao()
{
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
    if (nextState != currentState)
    {
        setAnimation(nextState);
    }
}

void Player::limparPlataformaMovel()
{
    plataformaMovelAtual = nullptr;
}
