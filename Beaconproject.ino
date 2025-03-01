#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEUUID.h>
#include <BLEAdvertisedDevice.h>
#include <BLEBeacon.h>

BLEScan* pBLEScan;
int scanTime = 15; // Tempo de escaneamento em segundos

#define TARGET_UUID "74278bda-b644-4520-8f0c-720eaf059935"

const int referenceRSSI = -65;
const float pathLossExponent = 2.0;

uint16_t swapEndian16(uint16_t value);

float calculateDistance(int rssi) {
    if (rssi == 0) {
        return -1.0;
    }
    return pow(10, ((referenceRSSI - rssi) / (10 * pathLossExponent)));
}

typedef struct {
    uint16_t manufacturerId;
    uint8_t subType;
    uint8_t subTypeLength;
    uint8_t proximityUUID[16];
    uint16_t major;
    uint16_t minor;
    int8_t signalPower;
} __attribute__((packed)) m_beaconData;

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) override {
        if (advertisedDevice.haveManufacturerData()) {
            String strManufacturerData = advertisedDevice.getManufacturerData();
            uint8_t cManufacturerData[100];
            int dataLength = strManufacturerData.length();

            if (dataLength >= 25) {
                BLEBeacon oBeacon;
                oBeacon.setData(strManufacturerData);
                uint16_t major = oBeacon.getMajor(); // Obter Major corretamente
                uint16_t minor = oBeacon.getMinor(); // Obter Minor corretamente
                int rssi = advertisedDevice.getRSSI();
                float distance = calculateDistance(rssi) * 10;

                Serial.printf("Distância Estimada: %.2f cm\n", distance);
                String macAddress = advertisedDevice.getAddress().toString();
                Serial.printf("Endereço MAC: %s\n", macAddress.c_str());
                Serial.printf("UUID: %s\n", oBeacon.getProximityUUID().toString().c_str());
                Serial.printf("Major: %d (0x%04X)\n", major, major);
                Serial.printf("Minor: %d (0x%04X)\n", minor, minor);
                Serial.printf("RSSI: %d\n", rssi);

                if (minor == 30000) {
                    String deviceName = advertisedDevice.haveName() ? advertisedDevice.getName() : "Desconhecido";

                    Serial.println("====================================");
                    Serial.printf("Endereço MAC: %s\n", macAddress.c_str());
                    Serial.printf("Nome do Dispositivo: %s\n", deviceName.c_str());
                    Serial.printf(
                        "ID: %04X \nMajor: %d (0x%04X) \nMinor: %d (0x%04X) \nUUID: %s \nSignal Power: %d\n",
                        oBeacon.getManufacturerId(),
                        major, major,
                        minor, minor,
                        oBeacon.getProximityUUID().toString().c_str(),
                        oBeacon.getSignalPower()
                    );

                    if (advertisedDevice.getServiceUUID().equals(BLEUUID(TARGET_UUID))) {
                        Serial.print("UUID : ");
                        Serial.println(oBeacon.getProximityUUID().toString().c_str());
                        Serial.print("RSSI : ");
                        Serial.println(rssi);
                        Serial.print("Major : ");
                        Serial.println(major, HEX);
                        Serial.print("Minor : ");
                        Serial.println(minor, HEX);
                        Serial.print("Distance : ");
                        Serial.println(distance, 2);
                        Serial.println("");

                        if (dataLength > sizeof(cManufacturerData)) {
                            dataLength = sizeof(cManufacturerData);
                        }
                        memcpy(cManufacturerData, strManufacturerData.c_str(), dataLength);
                    }

                    if (advertisedDevice.haveServiceData()) {
                        String serviceDataStr = advertisedDevice.getServiceData();
                        std::string serviceData(serviceDataStr.c_str());
                        uint8_t* serviceDataBytes = (uint8_t*)serviceData.c_str();
                        int serviceDataLength = serviceData.length();

                        if (serviceDataLength > 0) {
                            uint8_t batteryLevel = serviceDataBytes[0];
                            Serial.printf("Porcentagem da Bateria: %d%%\n", batteryLevel);
                        } else {
                            Serial.println("Dados de bateria não encontrados.");
                        }
                    } else {
                        Serial.println("Serviço de bateria não encontrado.");
                    }

                    Serial.println("====================================");
                }
            }
        }
    }
};

uint16_t swapEndian16(uint16_t value) {
    return (value >> 8) | (value << 8);
}

void setup() {
    Serial.begin(115200);
    BLEDevice::init("");
    pBLEScan = BLEDevice::getScan(); 
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true);
}

void loop() {
    BLEScanResults* foundDevices = pBLEScan->start(scanTime, false);
    pBLEScan->clearResults();
    delay(100);
}
