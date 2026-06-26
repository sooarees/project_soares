#ifndef MENU_H
#define MENU_H

#include <QWidget>

class QPushButton;
class QLabel;
class QPaintEvent;


class Menu : public QWidget
{
    Q_OBJECT

public:
    Menu();

protected:
    void paintEvent(QPaintEvent *event) override;

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
