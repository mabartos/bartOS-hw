//
// Created by mabartos on 4/11/21.
//

#include "ButtonCap.h"

ButtonCap::ButtonCap(const uint8_t &pin) : Capability(pin, CapabilityType::BUTTON, DEFAULT_DELAY_MILLIS) {
}

bool ButtonCap::isOn() {
    return _state;
}

bool ButtonCap::isOff() {
    return !_state;
}

void ButtonCap::init() {
    associateInterrupt(pin);

    updateState();
    _prevState = _state;
}

void ButtonCap::loop() {
    /*
    updateState();
    if (isChanged()) {
        executeOnChangeCallbacks();
        isOn() ? executeOnStateOnCallbacks() : executeOnStateOffChangeCallbacks();
    }
    */
}

void ButtonCap::onChange(Callback callback) {
    _onChangeCallbacks.push_back(callback);
}

void ButtonCap::onStateOn(Callback callback) {
    _onOnCallbacks.push_back(callback);
}

void ButtonCap::onStateOff(Callback callback) {
    _onOffCallbacks.push_back(callback);
}

void ButtonCap::executeOnChangeCallbacks() {
    for (auto callback : _onChangeCallbacks) {
        callback();
    }
}

void ButtonCap::executeOnStateOnCallbacks() {
    for (auto callback : _onOnCallbacks) {
        callback();
    }
}

void ButtonCap::executeOnStateOffChangeCallbacks() {
    for (auto callback : _onOffCallbacks) {
        callback();
    }
}

bool ButtonCap::isChanged() {
    updateState();
    bool changed = _state != _prevState;
    _prevState = _state;
    return changed;
}