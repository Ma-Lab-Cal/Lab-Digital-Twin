
#include "dotBLETransceiver.h"

using namespace rath;

void setup() {
  Serial.begin(115200);
  
  while (!Serial) {}
  
  Serial.setTimeout(100);

  BLETransceiverHub.init("Hub");
}

void loop() {
  // Serial -> BLE
  if (Serial.available()) {
    uint8_t buffer[128];
    uint16_t size = Serial.readBytesUntil(NLSM_END, buffer, 128);

    if (size != 0) {
      BLETransceiverHub.transmitAll(buffer, size);
    }
  }

  // BLE -> Serial
  uint8_t buffer[128];
  uint16_t size = BLETransceiverHub.receiveAny(buffer, 128);
  if (size != 0) {
    Serial.write(buffer, size);
    Serial.write(NLSM_END);
  }
}
