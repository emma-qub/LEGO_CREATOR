#ifndef LEGOFACTORY_H
#define LEGOFACTORY_H

#include <QString>
#include <QMap>
#include <QDebug>

#include "BrickDialog.h"


template<class Object, class Key = QString> class LegoFactory {

public:
    static void registerLego(Key key, Object* object);
    static Object* create(const Key& key);

public:
    static QMap<Key, Object*> _map;
};


template<class Object, class Key>
QMap<Key, Object*> LegoFactory<Object, Key>::_map = QMap<Key, Object*>();

template<class Object, class Key>
void LegoFactory<Object, Key>::registerLego(Key key, Object* object) {
    if (!_map.contains(key)) {
        _map.insert(key, object);
    }
}

template<class Object, class Key>
Object* LegoFactory<Object, Key>::create(const Key& key) {
    Object* tmp = NULL;

    typename QMap<Key, Object*>::iterator it = _map.find(key);

    if (it != _map.end()) {
        tmp = (it.value())->cloning();
    }

    return tmp;
}

#endif // LEGOFACTORY_H
