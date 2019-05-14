#ifndef MAIN_BLEDEVICE_H_
#define MAIN_BLEDEVICE_H_

#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include <BLEDevice.h>
#include <BLEClient.h>
#include <BLERemoteCharacteristic.h>
#include <BLEAdvertisedDevice.h>
#include <BLEUtils.h>
#include <Arduino.h>
#include <math.h>
#include <string.h>

#define L3_X 2
#define L3_Y 3
#define R3_X 4
#define R3_Y 5
#define L2 6
#define R2 7
#define BUTTONS_PLAY 8
#define BUTTONS_CENTER 9
#define DIGITAL 10

class JoystickClient : BLEDevice
{
public:
    JoystickClient();
    bool isFind();
    bool connectToServer();
    bool isConnected();
    bool update();
    std::pair<int, int> getXY();
    bool aPressed();
    bool bPressed();
    BLEScan *scan();

private:
    static BLEAddress address;
    static BLEUUID serviceUUID;
    static BLEUUID charUUID;
    static boolean connected;
    static BLERemoteCharacteristic *pRemoteCharacteristic;
    static BLEAdvertisedDevice *myDevice;
    static boolean doConnect;
    static uint8_t *ddata;
    static BLEScan *pBLEScan;

    class MyClientCallback : public BLEClientCallbacks
    {
    public:
        void onDisconnect(BLEClient *pclient);
        void onConnect(BLEClient *pclient);
    };

    class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
    {
    public:
        void onResult(BLEAdvertisedDevice advertisedDevice);
    };
};


#endif // CONFIG_BT_ENABLED
#endif /* MAIN_BLEDEVICE_H_ */