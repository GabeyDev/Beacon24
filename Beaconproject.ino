#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEUUID.h>
#include <BLEAdvertisedDevice.h>
#include <BLEBeacon.h>

// Definição das variáveis globais
BLEScan* pBLEScan;
int scanTime = 10; // Tempo de escaneamento em segundos

// UUID para limpar o resultado
#define CLEAR_RESULT "00000000-0000-0000-0000-000000000000"
#define TARGET_UUID "74278bda-b644-4520-8f0c-720eaf059935"

// Callback para dispositivos anunciados
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        // Verifica se o dispositivo possui Manufacturer Data antes de continuar
        if (advertisedDevice.haveManufacturerData()) {
            BLEBeacon id;
            id.setData(advertisedDevice.getManufacturerData());
            BLEUUID uuid = id.getProximityUUID();

            // Verifica se o dispositivo encontrado possui o UUID especificado
            if (uuid.equals(BLEUUID(TARGET_UUID))) {
                // Print UUID
                Serial.print("UUID : ");
                Serial.println(uuid.toString().c_str());

                // Print RSSI
                Serial.print("RSSI : ");
                Serial.println(advertisedDevice.getRSSI());

                // Print Major
                Serial.print("Major : ");
                Serial.println(id.getMajor(), HEX);

                // Print Minor
                Serial.print("Minor : ");
                Serial.println(id.getMinor(), HEX);

                // Calcula a distância com base no RSSI
                int rssi = advertisedDevice.getRSSI();
                float txPower = -59; // Potência de TX calibrada em dB
                float distance = pow(10, ((txPower - rssi) / 20.0));
                Serial.print("Distance : ");
                Serial.println(distance, 2);
                Serial.println("");
            }
        }
    }
};

void setup() {
    Serial.begin(115200);
    BLEDevice::init("");
    pBLEScan = BLEDevice::getScan(); // Cria uma nova varredura
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true);
    Serial.println("Scanning...");
}

void loop() {
    BLEScanResults foundDevices = pBLEScan->start(scanTime);
    Serial.print("Devices found: ");
    Serial.println(foundDevices.getCount());
    Serial.println("Scan done!");
    pBLEScan->clearResults(); // Limpa os resultados do buffer de BLEScan para liberar memória
    delay(1000);
}
