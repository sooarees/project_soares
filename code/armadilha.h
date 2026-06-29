#ifndef ARMADILHA_H
#define ARMADILHA_H

#include "elementofase.h"

class Armadilha : public ElementoFase
{
public:

    Armadilha(qreal x, qreal y, qreal width, qreal height);
    bool causaDano() const;

};

#endif // ARMADILHA_H
