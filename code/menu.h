#ifndef MENU_H
#define MENU_H

#include <QWidget>

class QPushButton;
class QLabel;


class Menu : public QWidget
{
    Q_OBJECT

public:
    Menu();

private slots:
    void iniciarJogo();

private:

    QLabel *titulo;
    QLabel *melhorTempo;

    QPushButton *botaoJogar;
    QPushButton *botaoSair;

    void configurarInterface();
};


#endif