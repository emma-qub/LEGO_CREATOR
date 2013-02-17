#ifndef LEGOFACTORY_H
#define LEGOFACTORY_H

#include <QString>
#include <QMap>
#include <QDebug>

#include "BrickDialog.h"
#include "WheelGeode.h"


template<class Object, class Key = QString> class LegoFactory {

public:
    static LegoFactory<Object, Key>* instance(void);
    static void kill(void);
    void registerLego(Key key, Object* object);
    Object* create(const Key& key);

private:
    static LegoFactory<Object, Key>* _self;
    static QMap<Key, Object*> _map;
};

template<class Object, class Key>
LegoFactory<Object, Key>* LegoFactory<Object, Key>::_self = NULL;

template<class Object, class Key>
QMap<Key, Object*> LegoFactory<Object, Key>::_map = QMap<Key, Object*>();

template<class Object, class Key>
LegoFactory<Object, Key>* LegoFactory<Object, Key>::instance(void) {
    if (!_self)
        _self = new LegoFactory<Object, Key>();

    return _self;
}

template<class Object, class Key>
void LegoFactory<Object, Key>::kill(void) {
    delete _self;
    _self = NULL;
}

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
