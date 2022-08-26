
#include "dotBLETransceiver.h"
#include "rath_SHT35.h"
#include "rath_PMSA003I.h"
#include "rath_MMC5983MA.h"

#include "rath_TSL2591.h"

#define NODE_NAME  "node2"

using namespace rath;

rath::SHT35 sensor_temperature = rath::SHT35(rath::I2C0, 0x44);  // I2C address for SHT35 is 0x44 or 0x45
rath::PMSA003I sensor_airquality = rath::PMSA003I();  // I2C address for PMSA003I is 0x12
rath::MMC5983MA sensor_magnetic = rath::MMC5983MA();
rath::TSL2591 sensor_light = rath::TSL2591();

void setup() {
  Serial.begin(115200);

  sensor_temperature.init();
  sensor_temperature.disableHeater();
  sensor_airquality.init();
  sensor_magnetic.init();
  sensor_light.init();
  
  BLESerializerNode.init(NODE_NAME);
  
  Serial.setTimeout(100);
  BLESerializerNode.setTimeout(100);
  
  BLESerializerNode.startAdvertising();
}

void loop() {

//  // BLE -> Serial
//  uint8_t buffer[128];
//  uint16_t size = BLESerializerNode.receive(buffer, 128);
//  if (size != 0) {
//    Serial.write(buffer, size);
//    Serial.write(NLSM_END);
//  }
//  


  float temperature = sensor_temperature.getTemperatureK();
  float humidity = sensor_temperature.getHumidity();

  
  rath::PMSA003I::DataFrame data_air_quality = sensor_airquality.get();

  int32_t num_particles_0_3_um = -1;
  int32_t num_particles_1_um = -1;
  int32_t num_particles_10_um = -1;
  if (data_air_quality.error_code == 0) {
    num_particles_0_3_um = data_air_quality.num_particles_0_3_um;
    num_particles_1_um = data_air_quality.num_particles_1_um;
    num_particles_10_um = data_air_quality.num_particles_10_um;
  }
  
  rath::MMC5983MA::DataFrame data_magnetic = sensor_magnetic.get();


  float magnetic_x = -1;
  float magnetic_y = -1;
  float magnetic_z = -1;
  
  if (data_magnetic.status == rath::OK) {
    magnetic_x = data_magnetic.x;
    magnetic_y = data_magnetic.y;
    magnetic_z = data_magnetic.z;
  }
  
  float lux = sensor_light.get();

  
  char str[256];

  sprintf(str, "[\"%s\", %f, %f, %d, %d, %d, %f, %f, %f, %f]", 
    NODE_NAME,
    temperature, 
    humidity,
    num_particles_0_3_um,
    num_particles_1_um,
    num_particles_10_um,
    magnetic_x,
    magnetic_y,
    magnetic_z,
    lux
    );

  Serial.println(str);
  BLESerializerNode.transmit((uint8_t *)str, strlen(str));

  delay(15000);
}
