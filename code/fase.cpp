#include "fase.h"

#include "espinho.h"
#include "plataforma.h"
#include "plataforma_movel.h"
#include "player.h"
#include "portal.h"
#include "princesa.h"
#include "serra.h"

#include <QFile>
#include <QGraphicsScene>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>

Fase::Fase(QGraphicsScene *scene, Player *jogador)
    : scene(scene),
      jogador(jogador)
{
}

int Fase::carregar(int numeroFase)
{
    jogador->limparPlataformaMovel();
    scene->removeItem(jogador);
    scene->clear();

    QFile arquivo(":/Fases/fases.json");
    if(!arquivo.open(QIODevice::ReadOnly))
    {
        jogador->setPos(spawnPadraoX, spawnPadraoY);
        scene->addItem(jogador);
        jogador->setFocus();
        return numeroFase;
    }

    QJsonDocument documento = QJsonDocument::fromJson(arquivo.readAll());
    QJsonObject fases = documento.object().value("fases").toObject();
    QJsonObject dadosFase = fases.value(QString::number(numeroFase)).toObject();

    if(dadosFase.isEmpty())
    {
        numeroFase = 1;
        dadosFase = fases.value("1").toObject();
    }

    adicionarPlataformas(dadosFase);
    adicionarPlataformasMoveis(dadosFase);
    adicionarPortais(dadosFase);
    adicionarArmadilhas(dadosFase);
    adicionarPrincesa(dadosFase);
    posicionarJogador(dadosFase);

    return numeroFase;
}

qreal Fase::valor(const QJsonObject &objeto, const QString &chave, qreal padrao) const
{
    return objeto.value(chave).toDouble(padrao);
}

void Fase::adicionarPlataformas(const QJsonObject &dadosFase)
{
    for(const QJsonValue &valorPlataforma : dadosFase.value("plataformas").toArray())
    {
        QJsonObject plataforma = valorPlataforma.toObject();
        scene->addItem(new Plataforma(
            valor(plataforma, "x"),
            valor(plataforma, "y"),
            valor(plataforma, "width"),
            valor(plataforma, "height")
            ));
    }
}

void Fase::adicionarPlataformasMoveis(const QJsonObject &dadosFase)
{
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
}

void Fase::adicionarPortais(const QJsonObject &dadosFase)
{
    for(const QJsonValue &valorPortal : dadosFase.value("portais").toArray())
    {
        QJsonObject portal = valorPortal.toObject();
        scene->addItem(new Portal(
            valor(portal, "x"),
            valor(portal, "y"),
            portal.value("destino").toInt(1)
            ));
    }
}

void Fase::adicionarArmadilhas(const QJsonObject &dadosFase)
{
    for(const QJsonValue &valorArmadilha : dadosFase.value("armadilhas").toArray())
    {
        QJsonObject armadilha = valorArmadilha.toObject();
        QString tipo = armadilha.value("tipo").toString();

        if(tipo == "espinho")
            scene->addItem(new Espinho(valor(armadilha, "x"), valor(armadilha, "y")));
        else if(tipo == "serra")
            scene->addItem(new Serra(valor(armadilha, "x"), valor(armadilha, "y")));
    }
}

void Fase::adicionarPrincesa(const QJsonObject &dadosFase)
{
    QJsonObject princesa = dadosFase.value("princesa").toObject();
    if(princesa.isEmpty())
        return;

    scene->addItem(new Princesa(
        valor(princesa, "x"),
        valor(princesa, "y")
        ));
}

void Fase::posicionarJogador(const QJsonObject &dadosFase)
{
    QJsonObject spawn = dadosFase.value("spawn").toObject();
    jogador->setPos(valor(spawn, "x", spawnPadraoX), valor(spawn, "y", spawnPadraoY));
    scene->addItem(jogador);
    jogador->setFocus();
}
