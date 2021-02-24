#include "WifiUtils.h"

#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <LittleFS.h>

#include "ESP8266TrueRandom.h"
#include "FS.h"
#include "mqtt/MessageForwarder.h"
#include "mqtt/MqttClient.h"

#define BROKER_URL_SIZE 60
#define SERVER_URL_SIZE 60

#define ID_SIZE 60
#define UUID_SIZE 50
#define NAME_SIZE 60

extern MqttClient client;
extern Device device;
extern const char *CONFIG_FILE;

char brokerURL[BROKER_URL_SIZE];
char serverURL[SERVER_URL_SIZE];
char homeID[ID_SIZE];
char UUID[UUID_SIZE];
char name[NAME_SIZE];
bool readActionProvided = false;

WifiUtils::WifiUtils(WiFiManager &wifiManager) : _wifiManager(wifiManager) {
}

void WifiUtils::begin() {
    readSaved();
    setWifiManager();
    writeSaved();

    device.setHomeID(_homeID);
}

void WifiUtils::readSaved() {
    if (LittleFS.begin() && LittleFS.exists(CONFIG_FILE)) {
        File configFile = LittleFS.open(CONFIG_FILE, "r");
        if (configFile) {
            size_t size = configFile.size();
            std::unique_ptr<char[]> buf(new char[size]);
            configFile.readBytes(buf.get(), size);

            StaticJsonDocument<1024> configDoc;

            DeserializationError err = deserializeJson(configDoc, buf.get());
            if (err != DeserializationError::Ok)
                return;

            serializeJson(configDoc, Serial);
            const JsonObject object = configDoc.as<JsonObject>();

            vector<string> keys{"brokerURL",
                                "serverURL",
                                "homeID",
                                "uuid",
                                "name"};
            if (MessageForwarder::containKeys(object, keys)) {
                strcpy(brokerURL, configDoc["brokerURL"]);
                strcpy(serverURL, configDoc["serverURL"]);
                strcpy(homeID, configDoc["homeID"]);
                strcpy(UUID, configDoc["uuid"]);
                strcpy(name, configDoc["name"]);

                _homeID = strtol(homeID, nullptr, 10);

                if (configDoc.containsKey("roomID")) {
                    long roomID = configDoc["roomID"];
                    device.setRoomID(roomID);
                }

                device.setName(string(name));
                device.setHomeID(_homeID);
                device.setBrokerURL(brokerURL);
                device.setServerURL(serverURL);

                client.setUUID(string(UUID));
                _alreadyCreated = true;
                readActionProvided = true;
            }

            if (configDoc.containsKey("deviceID")) {
                long devID = configDoc["deviceID"];
                device.setID(devID);
            }

            configFile.close();
        }
    }
}

void WifiUtils::writeSaved() {
    if (_shouldSaveConfig) {
        StaticJsonDocument<1024> configDoc;
        configDoc["brokerURL"] = brokerURL;
        configDoc["serverURL"] = serverURL;
        configDoc["homeID"] = homeID;
        configDoc["name"] = device.getName().c_str();

        // SAVE UUID
        byte uuidNumber[UUID_SIZE];
        ESP8266TrueRandom.uuid(uuidNumber);
        String uuidStr = ESP8266TrueRandom.uuidToString(uuidNumber);
        const char *uuid = uuidStr.c_str();
        configDoc["uuid"] = uuid;
        strcpy(UUID, uuid);
        client.setUUID(string(uuid));

        File configFile = LittleFS.open(CONFIG_FILE, "w");
        if (!configFile) {
            reset();
            return;
        }

        if (serializeJson(configDoc, configFile) == 0) {
            reset();
            return;
        }

        configFile.close();
    }
}

void WifiUtils::setWifiManager() {
    if (readActionProvided)
        return;

    string label("<h3> Device : " + device.getName() + "</h3>");
    WiFiManagerParameter label_parameter(label.c_str());
    WiFiManagerParameter brokerURL_parameter("broker", "MQTT Broker URL", brokerURL, BROKER_URL_SIZE);
    WiFiManagerParameter serverURL_parameter("server", "Server URL", serverURL, SERVER_URL_SIZE);
    WiFiManagerParameter homeID_parameter("homeID", "Home ID", homeID, ID_SIZE);

    _wifiManager.addParameter(&label_parameter);
    _wifiManager.addParameter(&brokerURL_parameter);
    _wifiManager.addParameter(&serverURL_parameter);
    _wifiManager.addParameter(&homeID_parameter);

    string WIFI_NAME("SmartHome-" + device.getName());

    if (!_wifiManager.autoConnect(WIFI_NAME.c_str())) {
        reset();
    }

    if (!readActionProvided) {
        strcpy(brokerURL, brokerURL_parameter.getValue());
        strcpy(serverURL, serverURL_parameter.getValue());
        strcpy(homeID, homeID_parameter.getValue());
        readActionProvided = false;
    }

    _brokerURL = brokerURL;
    _serverURL = serverURL;
    _homeID = strtol(homeID, nullptr, 10);
}

long WifiUtils::getHomeID() {
    return _homeID;
}

WiFiManager &WifiUtils::getWifiManager() {
    return _wifiManager;
}

void WifiUtils::setShouldSaveConfig(const bool &state) {
    _shouldSaveConfig = state;
}

bool WifiUtils::shouldSaveConfig() {
    return _shouldSaveConfig;
}

void WifiUtils::reset() {
    LittleFS.format();
    _wifiManager.resetSettings();
}

void WifiUtils::shouldClearStates(const bool &state) {
    if (state) {
        reset();
    }
}

bool WifiUtils::alreadyDeviceCreated() {
    return _alreadyCreated;
}