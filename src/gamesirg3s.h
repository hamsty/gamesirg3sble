#include <BLEDevice.h>
#include <Arduino.h>
#include <math.h>
#include <string.h>

#define LSB_X 0x2
#define LSB_Y 0x3
#define RSB_X 0x4
#define RSB_Y 0x5
#define LB 0x8
#define RB 0x8
#define BUTTONS_PLAY 0x8
#define BUTTONS_CENTER 0x9
#define DPAD 0xA

class JoystickClient
{
public:
    JoystickClient();
    ~JoystickClient();
    bool isFind();
    void connectToServer();
    bool isConnected();
    void update();
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
    static BLEClient *pClient;
    BLERemoteCharacteristic *pRemoteCharacteristic;
    uint8_t *ddata;
    BLEScan *pBLEScan;
    

    class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
    {
    public:
        void onResult(BLEAdvertisedDevice advertisedDevice);
    };
};