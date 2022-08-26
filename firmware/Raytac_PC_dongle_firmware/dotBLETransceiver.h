#include <Arduino.h>
#include <bluefruit.h>

namespace rath {

#define NLSM_END 0x0AU
#define NLSM_ESC 0x0BU
#define NLSM_ESC_END 0x1AU
#define NLSM_ESC_ESC 0x1BU

    
BLEConnection* connection;  // connection to hub


class BLETransceiverNodeClass {
  public:
    void init(const char *node_name) {
      // Setup the BLE LED to be enabled on CONNECT
      // Note: This is actually the default behavior, but provided
      // here in case you want to control this LED manually via PIN 19
      Bluefruit.autoConnLed(true);
    
      // Config the peripheral connection with maximum bandwidth
      // more SRAM required by SoftDevice
      // Note: All config***() function must be called before begin()
      Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);
    
      Bluefruit.begin();
      Bluefruit.setTxPower(4);    // Check bluefruit.h for supported values
      Bluefruit.setName(node_name); // useful testing with multiple central connections
    
      Bluefruit.Periph.setConnectCallback(onConnectHandler);
      Bluefruit.Periph.setDisconnectCallback(onDisconnectHandler);
    
      ble_dfu.begin();
    
      ble_device_info.setManufacturer("Rath Robotics");
      ble_device_info.setModel("nRF52840");
      ble_device_info.begin();
    
      ble_battery_info.begin();
      ble_battery_info.write(100);
      
      ble_uart.begin();
    }

    void setTimeout(int timeout) {
      ble_uart.setTimeout(timeout);
    }
  
    void startAdvertising(void) {
      // Advertising packet
      Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
      Bluefruit.Advertising.addTxPower();
    
      // Include bleuart 128-bit uuid
      Bluefruit.Advertising.addService(ble_uart);
    
      // Secondary Scan Response packet (optional)
      // Since there is no room for 'Name' in Advertising packet
      Bluefruit.ScanResponse.addName();
    
      /* Start Advertising
         - Enable auto advertising if disconnected
         - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
         - Timeout for fast mode is 30 seconds
         - Start(timeout) with timeout = 0 will advertise forever (until connected)
    
         For recommended advertising interval
         https://developer.apple.com/library/content/qa/qa1931/_index.html
      */
      Bluefruit.Advertising.restartOnDisconnect(true);
      Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
      Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
      Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds
    }

    static void onConnectHandler(uint16_t conn_handle) {
      // Get the reference to current connection
      connection = Bluefruit.Connection(conn_handle);
    
      char central_name[32] = {0};
      connection->getPeerName(central_name, sizeof(central_name));
    }
    
    static void onDisconnectHandler(uint16_t conn_handle, uint8_t reason) {
    }

    uint16_t receive(uint8_t *buffer, uint16_t size, uint8_t timeout) {
      if (timeout == 0 && !ble_uart.available()) return 0;
      while (!ble_uart.available()) {}
      
      uint16_t buffer_size = ble_uart.readBytesUntil(NLSM_END, buffer, size);
      return buffer_size;
    }
    
    void transmit(uint8_t *buffer, uint16_t size) {
      ble_uart.write(buffer, size);
      ble_uart.write(NLSM_END);
    }
    
    
  private:
    BLEDfu  ble_dfu;            // OTA DFU service
    BLEDis  ble_device_info;    // device information
    BLEBas  ble_battery_info;   // battery
    BLEUart ble_uart;           // uart over ble
};

BLETransceiverNodeClass BLETransceiverNode;



class BLESerializerNodeClass {
 public:
  void init(const char *node_name) {
    // Setup the BLE LED to be enabled on CONNECT
    // Note: This is actually the default behavior, but provided
    // here in case you want to control this LED manually via PIN 19
    Bluefruit.autoConnLed(true);
  
    // Config the peripheral connection with maximum bandwidth
    // more SRAM required by SoftDevice
    // Note: All config***() function must be called before begin()
    Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);
  
    Bluefruit.begin();
    Bluefruit.setTxPower(4);    // Check bluefruit.h for supported values
    Bluefruit.setName(node_name); // useful testing with multiple central connections
  
    Bluefruit.Periph.setConnectCallback(onConnectHandler);
    Bluefruit.Periph.setDisconnectCallback(onDisconnectHandler);
  
    ble_dfu.begin();
  
    ble_device_info.setManufacturer("Rath Robotics");
    ble_device_info.setModel("nRF52840");
    ble_device_info.begin();
  
    ble_battery_info.begin();
    ble_battery_info.write(100);
    
    ble_uart.begin();
  }

  void setTimeout(int timeout) {
    ble_uart.setTimeout(timeout);
  }

  void startAdvertising(void) {
    // Advertising packet
    Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
    Bluefruit.Advertising.addTxPower();
  
    // Include bleuart 128-bit uuid
    Bluefruit.Advertising.addService(ble_uart);
  
    // Secondary Scan Response packet (optional)
    // Since there is no room for 'Name' in Advertising packet
    Bluefruit.ScanResponse.addName();
  
    /* Start Advertising
       - Enable auto advertising if disconnected
       - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
       - Timeout for fast mode is 30 seconds
       - Start(timeout) with timeout = 0 will advertise forever (until connected)
  
       For recommended advertising interval
       https://developer.apple.com/library/content/qa/qa1931/_index.html
    */
    Bluefruit.Advertising.restartOnDisconnect(true);
    Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
    Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
    Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds
  }

  static void onConnectHandler(uint16_t conn_handle) {
    // Get the reference to current connection
    connection = Bluefruit.Connection(conn_handle);
  
    char central_name[32] = {0};
    connection->getPeerName(central_name, sizeof(central_name));
  }
  
  static void onDisconnectHandler(uint16_t conn_handle, uint8_t reason) {
  }

  uint16_t receive(uint8_t *buffer, uint16_t size) {
    uint8_t c;
    uint16_t index = 0;

    while (!ble_uart.available()) {}

    c = ble_uart.read();

    while (c != NLSM_END) {
      if (c == NLSM_ESC) {
        while (!ble_uart.available()) {}

        c = ble_uart.read();

        if (c == NLSM_ESC_END) {
          buffer[index] = NLSM_END;
        }
        else if (c == NLSM_ESC_ESC) {
          buffer[index] = NLSM_ESC;
        }
        else {
          buffer[index] = c;
        }
      }
      else {
        buffer[index] = c;
      }
      index += 1;
      while (!ble_uart.available()) {}
      c = ble_uart.read();
    }
    return index;
  }
  
  void transmit(uint8_t *buffer, uint16_t size) {
  
    if (size == 0) return;
  
    uint8_t c;
    uint16_t index = 0;
    while (index < size) {
      c = buffer[index];
      if (c == NLSM_END) {
        ble_uart.write(NLSM_ESC);
        ble_uart.write(NLSM_ESC_END);
      }
      else if (c == NLSM_ESC) {
        ble_uart.write(NLSM_ESC);
        ble_uart.write(NLSM_ESC_ESC);
      }
      else {
        ble_uart.write(c);
      }
      index += 1;
    }
    ble_uart.write(NLSM_END);
  }
  
  
private:
  BLEDfu  ble_dfu;            // OTA DFU service
  BLEDis  ble_device_info;    // device information
  BLEBas  ble_battery_info;   // battery
  BLEUart ble_uart;           // uart over ble
};

BLESerializerNodeClass BLESerializerNode;




typedef struct {
  char name[16+1];
  uint16_t conn_handle;
  BLEClientBas client_battery_info;
  BLEClientDis client_device_info;
  BLEClientUart client_uart;
} prph_info_t;

prph_info_t prphs[BLE_MAX_CONNECTION];

/**
 * Find the connection handle in the peripheral array
 * @param conn_handle Connection handle
 * @return array index if found, otherwise -1
 */
uint8_t findConnHandle(uint16_t conn_handle) {
  for(uint8_t id=0; id<BLE_MAX_CONNECTION; id+=1) {
    if (conn_handle == prphs[id].conn_handle) {
      return id;
    }
  }
  return -1;  
}

class BLETransceiverHubClass {
  public:
    void init(const char *hub_name) {
      Bluefruit.begin(0, 8);    // Peripheral = 0, Central = 4
    
      Bluefruit.setName(hub_name);
    
      for (uint8_t id=0; id<BLE_MAX_CONNECTION; id+=1) {
        prphs[id].conn_handle = BLE_CONN_HANDLE_INVALID;
    
        prphs[id].client_battery_info.begin();
    
        prphs[id].client_device_info.begin();
        
        prphs[id].client_uart.begin();
      }
    
      Bluefruit.Central.setConnectCallback(onConnectHandler);
      Bluefruit.Central.setDisconnectCallback(onDisconnectHandler);
      
      Bluefruit.Scanner.setRxCallback(onScanHandler);
      Bluefruit.Scanner.restartOnDisconnect(true);
      Bluefruit.Scanner.setInterval(160, 80);       // in units of 0.625 ms
      Bluefruit.Scanner.filterUuid(BLEUART_UUID_SERVICE);
      Bluefruit.Scanner.useActiveScan(false);       // Don't request scan response data
      Bluefruit.Scanner.start(0);                   // 0 = Don't stop scanning after n seconds
    }
    
    
    /**
     * Callback invoked when scanner picks up an advertising packet
     * @param report Structural advertising data
     */
    static void onScanHandler(ble_gap_evt_adv_report_t* report) {
      // Since we configure the scanner with filterUuid()
      // Scan callback only invoked for device with bleuart service advertised  
      // Connect to the device with bleuart service in advertising packet
      Bluefruit.Central.connect(report);
    }
    
    /**
     * Callback invoked when an connection is established
     * @param conn_handle
     */
    static void onConnectHandler(uint16_t conn_handle) {
      // Find an available ID to use
      int id = findConnHandle(BLE_CONN_HANDLE_INVALID);
    
      // Eeek: Exceeded the number of connections !!!
      if (id < 0) return;
      
      prph_info_t* peer = &prphs[id];
      peer->conn_handle = conn_handle;
      
      Bluefruit.Connection(conn_handle)->getPeerName(peer->name, sizeof(peer->name)-1);
    
      // battery information
      if (peer->client_battery_info.discover(conn_handle)) {
        int battery = peer->client_battery_info.read();
      }
      
      if (peer->client_uart.discover(conn_handle)) {
        peer->client_uart.setTimeout(100);
        peer->client_uart.enableTXD();
    
        Bluefruit.Scanner.start(0);
      }
      else {
        Bluefruit.disconnect(conn_handle);
      }
    }
    
    /**
     * Callback invoked when a connection is dropped
     * @param conn_handle
     * @param reason is a BLE_HCI_STATUS_CODE which can be found in ble_hci.h
     */
    static void onDisconnectHandler(uint16_t conn_handle, uint8_t reason) {
      // Mark the ID as invalid
      int id = findConnHandle(conn_handle);
    
      if (id < 0) return;
    
      prphs[id].conn_handle = BLE_CONN_HANDLE_INVALID;
    }

    uint16_t receiveAny(uint8_t *buffer, uint16_t size) {
      for (uint8_t id=0; id<BLE_MAX_CONNECTION; id+=1) {
        if (prphs[id].conn_handle == BLE_CONN_HANDLE_INVALID) {
          continue;
        }
    
        if (prphs[id].client_uart.available()) {
          uint16_t buffer_size = prphs[id].client_uart.readBytesUntil(NLSM_END, buffer, size);
          if (buffer_size != 0) {
            return buffer_size;
          }
        }
      }
      return 0;
    }
    
    /**
     * Helper function to send a string to all connected peripherals
     */
    void transmitAll(uint8_t *buffer, uint16_t size) {
      for(uint8_t id=0; id < BLE_MAX_CONNECTION; id+=1) {
        prph_info_t* peer = &prphs[id];
    
        if (peer->client_uart.discovered()) {
          peer->client_uart.write(buffer, size);
          peer->client_uart.write(NLSM_END);
        }
      }
    }
  };
  
  BLETransceiverHubClass BLETransceiverHub;

}
