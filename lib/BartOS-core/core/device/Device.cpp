
#include "Device.h"

#include <Arduino.h>

#include <string>

#include "DeviceFields.h"
#include "core/generator/NumberGenerator.h"
#include "core/storage/FsManager.h"

FsManager fsManager;

Device::Device() : StateConnection(ConnectionType::OFFLINE) {
    setName("Dev_" + NumberGenerator::generateIntToString(2000, 9999));
}

string Device::getName() {
    return _name;
}

void Device::setName(const string &name) {
    _name = name;
}

long Device::getID() {
    return _ID;
}
void Device::setID(const long &id) {
    _ID = id;
}

long Device::getHomeID() {
    return _homeID;
}
void Device::setHomeID(const long &homeID) {
    _homeID = homeID;
}

long Device::getRoomID() {
    return _roomID;
}

void Device::setRoomID(const long &roomID) {
    _roomID = roomID;
}

bool Device::isInitialized() {
    return _initialized;
}
void Device::setInitialized(bool initialized) {
    _initialized = initialized;
}

/* CAPS */
vector<shared_ptr<Capability>> Device::getCapabilities() {
    return _capabilities;
}

void Device::setCapabilities(vector<shared_ptr<Capability>> &caps) {
    _capabilities = caps;
}

auto Device::getCapByPin(const uint8_t &pin) -> shared_ptr<Capability> {
    for (auto &item : getCapabilities()) {
        if (item->getPin() == pin) {
            return item;
        }
    }
    return nullptr;
}

void Device::addCapability(shared_ptr<Capability> cap) {
    _capabilities.push_back(cap);
}

void Device::removeCapability(long id) {
    vector<shared_ptr<Capability>> caps = getCapabilities();
    for (unsigned i = 0; i < caps.size(); i++) {
        if (caps[i]->getID() == id) {
            _capabilities.erase(caps.begin() + i);
        }
    }
}

void Device::removeCapabilityByPin(const uint8_t &pin) {
    vector<shared_ptr<Capability>> caps = getCapabilities();
    for (unsigned i = 0; i < caps.size(); i++) {
        if (caps[i]->getPin() == pin) {
            _capabilities.erase(caps.begin() + i);
        }
    }
}

void Device::initAllCapabilities() {
    for (auto &item : getCapabilities()) {
        item->init();
    }
}

void Device::executeAllCapabilities() {
    for (auto &item : getCapabilities()) {
        item->execute();
    }
}

void Device::eraseAll() {
    Serial.println("Reset device");
    delay(2000);
    ESP.restart();
    delay(3000);
}

unsigned long Device::getDeviceMillis() {
    return millis();
}