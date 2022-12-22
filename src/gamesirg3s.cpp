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
BLEClient *JoystickClient::pClient;

void JoystickClient::connectToServer()
{
    pClient = BLEDevice::createClient();
    pClient->connect(myDevice);
    BLERemoteService *pRemoteService = pClient->getService(serviceUUID);
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
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

bool JoystickClient::isFind()
{
    return doConnect;
};

bool JoystickClient::isConnected()
{
    return connected;
};

void JoystickClient::update()
{
    connected = pClient->isConnected();
    if (connected)
    {
        pRemoteCharacteristic->readValue();
        ddata = pRemoteCharacteristic->readRawData();
        if (!(ddata[0] == 0xA1 && ddata[1] == 0xC4))
        {
            ddata[0x00] = 0xA1;
            ddata[0x01] = 0xC4;
            ddata[LSB_X] = 0x80;
            ddata[LSB_Y] = 0x80;
            ddata[RSB_X] = 0x80;
            ddata[RSB_Y] = 0x80;
            ddata[LB] = 0x00;
            ddata[BUTTONS_CENTER] = 0x00;
            ddata[DPAD] = 0x00;
        }
    }
};

pair<int, int> JoystickClient::getXY()
{
    int x = ddata[LSB_X] > 0x80 || ddata[DPAD] == 0x03 ? 1 : (ddata[LSB_X] < 0x80 || ddata[DPAD] == 0x07 ? -1 : 0);
    int y = ddata[LSB_Y] > 0x80 || ddata[DPAD] == 0x01 ? 1 : (ddata[LSB_Y] < 0x80 || ddata[DPAD] == 0x05 ? -1 : 0);
    return {x, y};
};

bool JoystickClient::aPressed()
{
    return ddata[BUTTONS_PLAY] == 0x01;
};

bool JoystickClient::bPressed()
{
    return ddata[BUTTONS_PLAY] == 0x02;
};

bool JoystickClient::startPressed()
{
    return ddata[BUTTONS_CENTER] == 0x08;
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

JoystickClient::~JoystickClient()
{
    delete[] pBLEScan;
    delete[] pRemoteCharacteristic;
    delete[] myDevice;
    delete[] ddata;
}