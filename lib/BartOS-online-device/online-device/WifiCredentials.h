#ifndef WIFI_CREDENTIALS_H
#define WIFI_CREDENTIALS_H

#include <string>

using namespace std;

class WifiCredentials {
private:
    string _ssid;
    string _pass;

public:
    WifiCredentials();

    WifiCredentials(string ssid, string pass);

    ~WifiCredentials() = default;

    string getSSID();

    void setSSID(const string &ssid);

    string getPassword();

    void setPassword(const string &pass);
};

#endif  //WIFI_CREDENTIALS_H