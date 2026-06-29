#ifndef TEMPO_H
#define TEMPO_H

#include <QChar>
#include <QString>

inline QString formatarTempo(qint64 milissegundos)
{
    qint64 minutos = milissegundos / 60000;
    qint64 segundos = (milissegundos % 60000) / 1000;
    qint64 milesimos = milissegundos % 1000;

    return QString("%1:%2:%3")
        .arg(minutos, 2, 10, QChar('0'))
        .arg(segundos, 2, 10, QChar('0'))
        .arg(milesimos, 3, 10, QChar('0'));
}

#endif // TEMPO_H
