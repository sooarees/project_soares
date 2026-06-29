#ifndef MENU_H
#define MENU_H

#include <QWidget>

class QLabel;
class QKeyEvent;
class QPaintEvent;
class QPushButton;

class Menu : public QWidget
{
    Q_OBJECT

public:
    Menu();

protected:
    void keyPressEvent(QKeyEvent *event) override;
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

#endif // MENU_H
