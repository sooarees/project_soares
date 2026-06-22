
#ifndef PORTAL_H
#define PORTAL_H

#include "elementofase.h"

class Portal : public ElementoFase
{
public:
    Portal(qreal x, qreal y, int destino);

    int getDestino() const;

private:
    int faseDestino;
};

#endif