#include <BLEDevice.h>
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

class JoystickClient
{
public:
    JoystickClient();
    ~JoystickClient();
    bool isFind();
    bool connectToServer();
    bool isConnected();
    bool update();
    std::pair<int, int> getXY();
    bool aPressed();
    bool bPressed();
    bool startPressed();

private:
    static BLEAddress address;
    static BLEUUID serviceUUID;
    static BLEUUID charUUID;
    static boolean connected;
    static BLEAdvertisedDevice *myDevice;
    static boolean doConnect;
    BLERemoteCharacteristic *pRemoteCharacteristic;
    uint8_t *ddata;
    BLEScan *pBLEScan;
    BLEClient *pCLient;

    class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
    {
    public:
        void onResult(BLEAdvertisedDevice advertisedDevice);
    };
};