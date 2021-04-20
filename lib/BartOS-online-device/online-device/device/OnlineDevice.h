#ifndef ONLINE_DEVICE_H
#define ONLINE_DEVICE_H

#include <ArduinoJson.h>
#include <core/device/Device.h>
#include <online-device/device/connector/DataConnector.h>
#include <online-device/device/connector/ManageConnector.h>
#include <online-device/credentials/WifiCredentials.h>
#include "core/generator/NumberGenerator.h"
#include <unordered_map>

class OnlineDevice : public Device {
private:
    long _id = -1;
    long _homeID = -1;
    long _roomID = -1;
    shared_ptr<DataConnector> _dataConnector;
    shared_ptr<ManageConnector> _manageConnector;

    shared_ptr<WifiCredentials> _wifiCredentials;
    bool _storeToFileSystem;
protected:
    DynamicJsonDocument getCreateJSON();

    shared_ptr<WifiCredentials> getCredentials();

public:
    OnlineDevice(const vector<shared_ptr<Capability>> &capabilities,
                 shared_ptr<ManageConnector> manageConn,
                 shared_ptr<DataConnector> dataConn,
                 const string name = "Dev_" + NumberGenerator::generateIntToString(2000, 9999),
                 bool storeToFileSystem = false
    );

    ~OnlineDevice() = default;

    void init() override;

    //TODO
    void setUpOnline();

    bool createDevice();

    bool connectDevice();

    bool disconnectDevice();

    shared_ptr<ManageConnector> getManageConnector();

    shared_ptr<DataConnector> getDataConnector();

    void setUpCapabilities(const JsonObject &capsData);

    void setCredentials(shared_ptr<WifiCredentials> credentials);

    long getID();

    void setID(const long &id);

    long getHomeID();

    void setHomeID(const long &homeID);

    long getRoomID();

    void setRoomID(const long &roomID);
};

#endif  //ONLINE_DEVICE_H