#include "menu.h"
#include "Game.h"
#include "tempo.h"

#include <QPushButton>
#include <QLabel>
#include <QColor>
#include <QCursor>
#include <QEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFont>
#include <QFontDatabase>
#include <QFontMetrics>
#include <QGuiApplication>
#include <QImage>
#include <QLinearGradient>
#include <QPainter>
#include <QPainterPath>
#include <QPaintEvent>
#include <QPen>
#include <QPixmap>
#include <QPoint>
#include <QScreen>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QtGlobal>

namespace
{
// Componentes visuais usados apenas pelo menu.
class TituloLabel : public QLabel
{
public:
    explicit TituloLabel(const QString &texto, QWidget *parent = nullptr)
        : QLabel(texto, parent)
    {
        setAttribute(Qt::WA_TranslucentBackground);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        Q_UNUSED(event);

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        const int contorno = 5;
        QFontMetrics fonteMedidas(font());
        QRect areaTexto = fonteMedidas.boundingRect(
            rect().adjusted(contorno, contorno, -contorno, -contorno),
            alignment(),
            text()
            );

        QPainterPath texto;
        texto.addText(
            areaTexto.left(),
            areaTexto.top() + fonteMedidas.ascent(),
            font(),
            text()
            );

        QLinearGradient corTitulo(areaTexto.topLeft(), areaTexto.bottomLeft());
        corTitulo.setColorAt(0.00, QColor("#46f3ff"));
        corTitulo.setColorAt(0.42, QColor("#46f3ff"));
        corTitulo.setColorAt(0.43, QColor("#fff7d8"));
        corTitulo.setColorAt(0.68, QColor("#fff7d8"));
        corTitulo.setColorAt(0.69, QColor("#ffb13b"));
        corTitulo.setColorAt(0.84, QColor("#ffb13b"));
        corTitulo.setColorAt(0.85, QColor("#e7473f"));
        corTitulo.setColorAt(1.00, QColor("#e7473f"));

        painter.setPen(QPen(Qt::black, contorno, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawPath(texto);
        painter.fillPath(texto, corTitulo);
    }
};

// Botao baseado em sprites, com area clicavel seguindo pixels visiveis.
class BotaoImagem : public QPushButton
{
public:
    BotaoImagem(
        const QString &normal,
        const QString &hover,
        const QString &pressed,
        const QString &icone,
        int tamanhoIcone,
        QWidget *parent = nullptr
        )
        : QPushButton(parent),
        imagemNormal(normal),
        imagemHover(hover),
        imagemPressed(pressed),
        imagemIcone(icone),
        tamanhoDoIcone(tamanhoIcone)
    {
        setText("");
        setCursor(Qt::PointingHandCursor);
        setFocusPolicy(Qt::NoFocus);
        setMouseTracking(true);
    }

protected:
    bool event(QEvent *event) override
    {
        if(event->type() == QEvent::Enter ||
            event->type() == QEvent::Leave ||
            event->type() == QEvent::MouseMove)
        {
            update();
        }

        return QPushButton::event(event);
    }

    bool hitButton(const QPoint &pos) const override
    {
        return pontoVisivel(pos);
    }

    void paintEvent(QPaintEvent *event) override
    {
        Q_UNUSED(event);

        QPainter painter(this);
        painter.setRenderHint(QPainter::SmoothPixmapTransform, false);

        QPixmap imagemBotao = imagemNormal;

        if(isDown())
            imagemBotao = imagemPressed;
        else if(underMouse() && pontoVisivel(mapFromGlobal(QCursor::pos())))
            imagemBotao = imagemHover;

        painter.drawPixmap(rect(), imagemBotao);

        QPixmap icone = imagemIcone.scaled(
            tamanhoDoIcone,
            tamanhoDoIcone,
            Qt::KeepAspectRatio,
            Qt::FastTransformation
            );

        QRect areaIcone(
            (width() - icone.width()) / 2,
            (height() - icone.height()) / 2,
            icone.width(),
            icone.height()
            );

        painter.drawPixmap(areaIcone, icone);
    }

private:
    bool pontoVisivel(const QPoint &pos) const
    {
        if(!rect().contains(pos) || width() <= 0 || height() <= 0)
            return false;

        QImage imagem = imagemNormal.toImage();

        int x = pos.x() * imagem.width() / width();
        int y = pos.y() * imagem.height() / height();

        x = qBound(0, x, imagem.width() - 1);
        y = qBound(0, y, imagem.height() - 1);

        return QColor::fromRgba(imagem.pixel(x,y)).alpha() > 10;
    }

    QPixmap imagemNormal;
    QPixmap imagemHover;
    QPixmap imagemPressed;
    QPixmap imagemIcone;
    int tamanhoDoIcone;
};

// Label customizada para desenhar o melhor tempo com contorno.
class MelhorTempoLabel : public QLabel
{
public:
    explicit MelhorTempoLabel(QWidget *parent = nullptr)
        : QLabel(parent)
    {
        setAttribute(Qt::WA_TranslucentBackground);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        Q_UNUSED(event);

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        QStringList linhas = text().split('\n');
        if(linhas.size() < 2)
        {
            QLabel::paintEvent(event);
            return;
        }

        const int contorno = 3;
        QFontMetrics fonteMedidas(font());
        QRect linhaTitulo(0,2,width(),34);
        QRect linhaTempo(0,50,width(),34);

        QPainterPath textoTitulo;
        QRect areaTitulo = fonteMedidas.boundingRect(linhaTitulo, Qt::AlignCenter, linhas.at(0));
        textoTitulo.addText(
            areaTitulo.left(),
            areaTitulo.top() + fonteMedidas.ascent(),
            font(),
            linhas.at(0)
            );

        QPainterPath textoTempo;
        QRect areaTempo = fonteMedidas.boundingRect(linhaTempo, Qt::AlignCenter, linhas.at(1));
        textoTempo.addText(
            areaTempo.left(),
            areaTempo.top() + fonteMedidas.ascent(),
            font(),
            linhas.at(1)
            );

        painter.setPen(QPen(Qt::black, contorno, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawPath(textoTitulo);
        painter.drawPath(textoTempo);

        painter.fillPath(textoTitulo, QColor("#ffdf00"));
        painter.fillPath(textoTempo, QColor("#ffdf00"));
    }
};

}

// Implementacao do Menu.
Menu::Menu()
{
    configurarInterface();
}

void Menu::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    QPixmap background(":/Sprites/Game Images/Royal/Menu/background.png");

    painter.drawPixmap(
        rect(),
        background,
        background.rect()
        );
}

void Menu::configurarInterface()
{
    setFixedSize(800,600);
    setWindowFlags(Qt::FramelessWindowHint);

    move(QGuiApplication::primaryScreen()->geometry().center() - rect().center());

    // Titulo
    titulo = new TituloLabel("ROYAL KNIGHT");

    titulo->setAlignment(Qt::AlignCenter);
    titulo->setMinimumHeight(80);

    int idFonteTitulo = QFontDatabase::addApplicationFont(":/Sprites/Game Images/Royal/Menu/GravityBold8.ttf");
    QString familiaTitulo = "Georgia";

    if(idFonteTitulo != -1)
    {
        QStringList familias = QFontDatabase::applicationFontFamilies(idFonteTitulo);

        if(!familias.isEmpty())
        {
            familiaTitulo = familias.first();
        }
    }

    QFont fonteTitulo(familiaTitulo);
    fonteTitulo.setPointSize(44);
    fonteTitulo.setLetterSpacing(QFont::AbsoluteSpacing, 2);

    titulo->setFont(fonteTitulo);

    // Melhor tempo
    QSettings configuracoes("Royal Knight", "Royal Knight");
    qint64 melhorTempoMs = configuracoes.value("melhorTempoMs", -1).toLongLong();

    melhorTempo = new MelhorTempoLabel();
    if(melhorTempoMs >= 0)
        melhorTempo->setText("Melhor tempo:\n" + formatarTempo(melhorTempoMs));
    else
        melhorTempo->setText("Melhor tempo:\n--:--:---");

    melhorTempo->setAlignment(Qt::AlignCenter);
    melhorTempo->setMinimumHeight(100);

    QFont fonteTempo(familiaTitulo);
    fonteTempo.setPointSize(17);
    fonteTempo.setBold(true);

    melhorTempo->setFont(fonteTempo);

    // Botoes
    botaoJogar = new BotaoImagem(
        ":/Sprites/Game Images/Royal/Menu/botao/Button_2_normal.png",
        ":/Sprites/Game Images/Royal/Menu/botao/Button_02_hovered.png",
        ":/Sprites/Game Images/Royal/Menu/botao/Button_02_pressed.png",
        ":/Sprites/Game Images/Royal/Menu/botao/Play_Icon.png",
        160
        );

    botaoSair = new BotaoImagem(
        ":/Sprites/Game Images/Royal/Menu/botao/Button_01_normal.png",
        ":/Sprites/Game Images/Royal/Menu/botao/Button_01_hovered.png",
        ":/Sprites/Game Images/Royal/Menu/botao/Button_01_pressed.png",
        ":/Sprites/Game Images/Royal/Menu/botao/Exit_Icon.png",
        160
        );

    botaoJogar->setFixedSize(192,192);
    botaoSair->setFixedSize(192,230);

    QFont fonteBotao;
    fonteBotao.setPointSize(20);

    botaoJogar->setFont(fonteBotao);
    botaoSair->setFont(fonteBotao);

    // Layout
    QVBoxLayout *layout = new QVBoxLayout();
    QHBoxLayout *layoutBotoes = new QHBoxLayout();

    layoutBotoes->addWidget(botaoSair);
    layoutBotoes->addSpacing(35);
    layoutBotoes->addWidget(botaoJogar);

    layout->addStretch();
    layout->addWidget(titulo);
    layout->addSpacing(50);
    layout->addLayout(layoutBotoes);
    layout->setAlignment(layoutBotoes, Qt::AlignCenter);
    layout->addSpacing(40);
    layout->addWidget(melhorTempo);
    layout->addStretch();

    setLayout(layout);

    // Eventos
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
