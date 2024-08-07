#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEUUID.h>
#include <BLEAdvertisedDevice.h>
#include <BLEBeacon.h>

BLEScan* pBLEScan;
BLEBeacon id;

int scanTime = 10; //In seconds

#define CLEAR_RESULT "00000000-0000-0000-0000-000000000000"

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        id.setData(advertisedDevice.getManufacturerData());
        String identity = id.getProximityUUID().toString().c_str();
        
        if (identity == "74278bda-b644-4520-8f0c-720eaf059935") {
            //Print UUID
            Serial.print("UUID :");
            Serial.println(id.getProximityUUID().toString().c_str());
            
            //Print RSSI
            Serial.print("RSSI :");
            Serial.println(advertisedDevice.getRSSI());
            
            //Print Major
            Serial.print("Major :");
            Serial.println(id.getMajor(), HEX);
            
            //Print Minor
            Serial.print("Minor :");
            Serial.println(id.getMinor(), HEX);
            
            //Calculate range based on RSSI
            int rssi = advertisedDevice.getRSSI();
            float txPower = -59; //Calibrated TX power in dB
            float distance = pow(10, ((txPower - rssi) / 20.0));
            Serial.print("Distance :");
            Serial.println(distance, 2);
            Serial.println("");
        }
        id.setProximityUUID(BLEUUID(CLEAR_RESULT)); //Clear scan history
        id.setMajor(0);
        id.setMinor(0);
    }
};

void setup() {
    Serial.begin(115200);
    BLEDevice::init("");
    pBLEScan = BLEDevice::getScan(); //create new scan
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true);
    Serial.println("Scanning...");
}

void loop() {
    BLEScanResults foundDevices = pBLEScan->start(scanTime);
    Serial.print("Devices found: ");
    Serial.println(foundDevices.getCount());
    Serial.println("Scan done! ");
    pBLEScan->clearResults(); // delete results from BLEScan buffer to release memory
    delay(1000);
}