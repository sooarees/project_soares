#ifndef FASE_H
#define FASE_H

#include <QtGlobal>

class Player;
class QGraphicsScene;
class QJsonObject;
class QString;

class Fase
{
public:
    static constexpr int spawnPadraoX = 120;
    static constexpr int spawnPadraoY = 750;

    Fase(QGraphicsScene *scene, Player *jogador);

    int carregar(int numeroFase);

private:
    QGraphicsScene *scene;
    Player *jogador;

    qreal valor(const QJsonObject &objeto, const QString &chave, qreal padrao = 0) const;
    void adicionarPlataformas(const QJsonObject &dadosFase);
    void adicionarPlataformasMoveis(const QJsonObject &dadosFase);
    void adicionarPortais(const QJsonObject &dadosFase);
    void adicionarArmadilhas(const QJsonObject &dadosFase);
    void adicionarPrincesa(const QJsonObject &dadosFase);
    void posicionarJogador(const QJsonObject &dadosFase);
};

#endif // FASE_H
