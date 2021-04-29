//
// Created by mabartos on 4/11/21.
//

#include "DefaultIRCap.h"
#include <core/utils/ConvertUtils.h>

DefaultIRCap::DefaultIRCap(const uint8_t &pin, const string &name) :
        InfraRedCap(pin, name) {
    _irRecv = make_shared<IRrecv>(pin);
    _results = make_shared<decode_results>();
}

void DefaultIRCap::init() {
    _irRecv->enableIRIn();
}

void DefaultIRCap::execute() {
    if (_irRecv->decode(_results.get())) {
        const unsigned int tmp = _results->value;
        string hexValue = ConvertUtils::convertIntToHexString(tmp);

        if (shouldPrintCodes()) {
            Serial.println(hexValue.c_str());
        }

        InfraRedCap::callbacks()->execute(hexValue);
        _irRecv->resume();
        delayExecution(100);
    }
    yield();
}