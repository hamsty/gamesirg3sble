#include <BLEDevice.h>
#include <Arduino.h>
#include <math.h>
#include <string.h>
#include "gamesirg3s.h"

using namespace std;

BLEAddress JoystickClient::address = BLEAddress("86:55:06:68:2D:E0");
BLEUUID JoystickClient::serviceUUID = BLEUUID("00008650-0000-1000-8000-00805f9b34fb");
BLEUUID JoystickClient::charUUID = BLEUUID("00008651-0000-1000-8000-00805f9b34fb");
boolean JoystickClient::connected = false;
boolean JoystickClient::doConnect = false;
BLEAdvertisedDevice *JoystickClient::myDevice;


bool JoystickClient::connectToServer()
{
    BLEClient *pClient = BLEDevice::createClient();
    pClient->connect(myDevice); 
    delete [] myDevice;
    BLERemoteService *pRemoteService = pClient->getService(serviceUUID);
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    delete [] pRemoteService;
    connected = true;
}

void JoystickClient::MyAdvertisedDeviceCallbacks::onResult(BLEAdvertisedDevice advertisedDevice)
{
    if (advertisedDevice.getAddress().equals(address))
    {
        BLEDevice::getScan()->stop();
        myDevice = new BLEAdvertisedDevice(advertisedDevice);
        doConnect = true;
    }
}

bool JoystickClient::isFind(){
    delete [] pBLEScan;
    return doConnect;
};

bool JoystickClient::isConnected(){
    return connected;
};

bool JoystickClient::update(){
    if(connected){
        delete [] ddata;
        pRemoteCharacteristic->readValue();
        ddata = pRemoteCharacteristic->readRawData();
    }
};

pair<int,int> JoystickClient::getXY(){
    int x = ddata[L3_X] > 128 || (ddata[DIGITAL]>=2 && ddata[DIGITAL]<=4) ? 1 : (ddata[L3_X] < 128 || (ddata[DIGITAL]>=6 && ddata[DIGITAL]<=8) ? -1 : 0);
    int y = ddata[L3_Y] > 128 || (ddata[DIGITAL]>=4 && ddata[DIGITAL]<=6) ? -1 : (ddata[L3_Y] < 128|| ddata[DIGITAL]== 1 || ddata[DIGITAL]== 8 || ddata[DIGITAL]== 2 ? 1 : 0);
    return {x,y};
};

bool JoystickClient::aPressed(){
    return ddata[BUTTONS_PLAY]==1;
};

bool JoystickClient::bPressed(){
    return ddata[BUTTONS_PLAY]==2;
};

bool JoystickClient::startPressed(){
    return ddata[BUTTONS_CENTER]==8;
};

JoystickClient::JoystickClient()
{
    doConnect = false;
    connected = false;
    BLEDevice::init("JoystickClient");
    pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setInterval(1349);
    pBLEScan->setWindow(449);
    pBLEScan->setActiveScan(true);
    pBLEScan->start(5, false);
}

JoystickClient::~JoystickClient(){
    delete [] pBLEScan;
    delete [] pRemoteCharacteristic;
    delete [] myDevice;
    delete [] ddata;
}