//
// Created by mabartos on 3/31/21.
//

#include "CallbackMap.h"
#include <Arduino.h>

void CallbackMap::execute(const string &name) {
    auto it = _callbacks.find(name);
    if (it != _callbacks.end()) {
        it->second();
    }
}

void CallbackMap::executeAll() {
    for (auto item:_callbacks) {
        item.second();
    }
}

void CallbackMap::clearAll() {
    _callbacks.clear();
}

void CallbackMap::add(const string &name, Callback callback) {
    _callbacks.insert({name, callback});
}

void CallbackMap::remove(const string &name) {
    _callbacks.erase(name);
}

uint32_t CallbackMap::getSize() {
    return _callbacks.size();
}