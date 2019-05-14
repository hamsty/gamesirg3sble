#include <BLEDevice.h>
#include <BLEClient.h>
#include <BLERemoteCharacteristic.h>
#include <BLEAdvertisedDevice.h>
#include <BLEUtils.h>
#include <Arduino.h>
#include <math.h>
#include <string.h>

using namespace std;

static BLEAddress address("86:55:06:68:2D:E0");
static BLEUUID serviceUUID = BLEUUID("00008650-0000-1000-8000-00805f9b34fb");
static BLEUUID charUUID = BLEUUID("00008651-0000-1000-8000-00805f9b34fb");
static boolean connected = false;
static BLERemoteCharacteristic *pRemoteCharacteristic;
static BLEAdvertisedDevice *myDevice;
static boolean doConnect = false;
static uint8_t *ddata;
static BLEScan *pBLEScan;

void JoystickClient::MyClientCallback::onConnect(BLEClient *pclient)
{

};


void JoystickClient::MyClientCallback::onDisconnect(BLEClient *pclient)
{
    connected = false;
    Serial.println("onDisconnect");
};


bool JoystickClient::connectToServer()
{
    Serial.print("Forming a connection to ");
    Serial.println(myDevice->getAddress().toString().c_str());

    BLEClient *pClient = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remove BLE Server.
    pClient->connect(myDevice); // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(" - Connected to server");

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService *pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr)
    {
        Serial.print("Failed to find our service UUID: ");
        Serial.println(serviceUUID.toString().c_str());
        pClient->disconnect();
        return false;
    }
    Serial.println(" - Found our service");

    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic == nullptr)
    {
        Serial.print("Failed to find our characteristic UUID: ");
        Serial.println(charUUID.toString().c_str());
        pClient->disconnect();
        return false;
    }
    Serial.println(" - Found our characteristic");
    connected = true;
}

void JoystickClient::MyAdvertisedDeviceCallbacks::onResult(BLEAdvertisedDevice advertisedDevice)
{
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.getAddress().equals(address))
    {

        BLEDevice::getScan()->stop();
        myDevice = new BLEAdvertisedDevice(advertisedDevice);

    } // Found our server
}     // onResult

bool JoystickClient::isFind(){
    return doConnect;
};

bool JoystickClient::isConnected(){
    return connected;
};

bool JoystickClient::update(){
    if(connected){
        pRemoteCharacteristic->readValue();
        ddata = pRemoteCharacteristic->readRawData();
    }
};

pair<int,int> JoystickClient::getXY(){
    int x = ddata[L3_X] > 128 ? 1 : (ddata[L3_X] < 128 ? -1 : 0);
    int y = ddata[L3_Y] > 128 ? -1 : (ddata[L3_Y] < 128 ? 1 : 0);
    return {x,y};
};

bool JoystickClient::aPressed(){
    return ddata[BUTTONS_PLAY]==1;
};

bool JoystickClient::bPressed(){
    return ddata[BUTTONS_PLAY]==2;
};

BLEScan* JoystickClient::scan(){
    return pBLEScan;
}

JoystickClient::JoystickClient()
{
    doConnect = false;
    connected = false;
    JoystickClient::init("JoystickClient");
    pBLEScan = JoystickClient::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setInterval(1349);
    pBLEScan->setWindow(449);
    pBLEScan->setActiveScan(true);
}
