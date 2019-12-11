#ifndef DESIGNPATTERN_H
#define DESIGNPATTERN_H

#include <QSharedPointer>
#include "DesignPattern_global.h"

namespace DesignPattern
{
template<class T, class ... Params>
QSharedPointer<T> factory(Params... parameters)
{
    return QSharedPointer<T>::create(parameters...);
}
};

#endif // DESIGNPATTERN_H
