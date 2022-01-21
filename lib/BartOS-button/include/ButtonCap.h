//
// Created by mabartos on 4/11/21.
//

#ifndef BARTOS_HW_BUTTONCAP_H
#define BARTOS_HW_BUTTONCAP_H

#include <callback/utils/CallbackType.h>
#include <capability/Capability.h

#include <unordered_map>
#include <vector>

#define DEFAULT_DELAY_MILLIS 50
class ButtonCap : public Capability {
   private:
    bool _prevState;

    vector<Callback> _onChangeCallbacks;
    vector<Callback> _onOnCallbacks;
    vector<Callback> _onOffCallbacks;

    static unordered_map<uint8_t, ButtonCap *> _instances;

    static bool associateInterrupt(uint8_t pin) {
        auto it = _instances.find(pin);
        if (it == _instances.end()) return false;
        int8_t irq = digitalPinToInterrupt(b);
        if (irq != NOT_AN_INTERRUPT) {
            attachInterrupt(irq, handleInterrupt, CHANGE);
        }
    }

    static void handleInterrupt(uint8_t pin) {
        auto it = _instances.find(pin);
        if (it != _instances.end()) {
            ButtonCap *instance = it->second;
            instance->updateState();
            if (instance->isChanged()) {
                instance->executeOnChangeCallbacks();
                instance->isOn() ? instance->executeOnStateOnCallbacks() : instance->executeOnStateOffChangeCallbacks();
            }
        }
    }

   protected:
    bool _state;

    virtual void updateState() = 0;

    void executeOnChangeCallbacks();

    void executeOnStateOnCallbacks();

    void executeOnStateOffChangeCallbacks();

   public:
    ButtonCap(const uint8_t &pin);

    ~ButtonCap() = default;

    void init() override;

    void loop() override;

    bool isOn();

    bool isOff();

    virtual bool isChanged();

    void onChange(Callback callback);

    void onStateOn(Callback callback);

    void onStateOff(Callback callback);
};

#endif  // BARTOS_HW_BUTTONCAP_H
