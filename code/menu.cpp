#include "Menu.h"
#include "Game.h"

#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QFont>
#include <QGuiApplication>
#include <QScreen>


Menu::Menu()
{
    configurarInterface();
}


void Menu::configurarInterface()
{

    setFixedSize(800,600);

    move(QGuiApplication::primaryScreen()->geometry().center() - rect().center());

    titulo = new QLabel("ROYAL KNIGHT");

    titulo->setAlignment(Qt::AlignCenter);


    QFont fonteTitulo;
    fonteTitulo.setPointSize(36);
    fonteTitulo.setBold(true);

    titulo->setFont(fonteTitulo);



    melhorTempo = new QLabel(
        "Melhor tempo:\n--:--:---"
        );

    melhorTempo->setAlignment(Qt::AlignCenter);


    QFont fonteTempo;
    fonteTempo.setPointSize(18);

    melhorTempo->setFont(fonteTempo);



    botaoJogar = new QPushButton("JOGAR");

    botaoSair = new QPushButton("SAIR");



    botaoJogar->setFixedSize(250,70);
    botaoSair->setFixedSize(250,70);



    QFont fonteBotao;
    fonteBotao.setPointSize(20);

    botaoJogar->setFont(fonteBotao);
    botaoSair->setFont(fonteBotao);



    QVBoxLayout *layout = new QVBoxLayout();


    layout->addStretch();

    layout->addWidget(titulo);

    layout->addSpacing(50);

    layout->addWidget(botaoJogar,
                      0,
                      Qt::AlignCenter);

    layout->addWidget(botaoSair,
                      0,
                      Qt::AlignCenter);


    layout->addSpacing(40);

    layout->addWidget(melhorTempo);

    layout->addStretch();



    setLayout(layout);



    connect(botaoJogar,
            &QPushButton::clicked,
            this,
            &Menu::iniciarJogo);


    connect(botaoSair,
            &QPushButton::clicked,
            this,
            &QWidget::close);

}



void Menu::iniciarJogo()
{
    Game *game = new Game();

    game->show();

    close();
}