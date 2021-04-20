//
// Created by mabartos on 4/19/21.
//

#include "PowerAbleData.h"

const char *PowerAbleData::STATE = "state";

PowerAbleData::PowerAbleData(shared_ptr<PubSubDataConnector> dataConnector) :
        PubSubDataTransceiver(dataConnector) {
}

void PowerAbleData::sendData(PowerAbleCap *cap) {
    if (cap == nullptr)return;

    DynamicJsonDocument data(100);
    data[STATE] = cap->isTurnedOn();

    data.shrinkToFit();
    sendDataToDefault(data);
    data.garbageCollect();
}

void PowerAbleData::initDataHandler(PowerAbleCap *cap, long deviceID, long homeID, long roomID) {
    if (cap == nullptr) return;

    auto setValues = [&](DynamicJsonDocument doc) -> void {
        if (containKeys(doc, {STATE})) {
            const bool state = doc[STATE];
            cap->changeState(state);
        }
    };

    addSameCallbackForBasicTopics(cap, setValues, deviceID, homeID, roomID);
}