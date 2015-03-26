#include <Arduino.h>
#include <EspWiFi.h>

// Wired to `RESET` pin on the ESP8266
#define RESET_PIN 6

#define SSID     "Your-SSID"
#define PASSWORD "P4ssw0rd"
#define TCP_PORT 80

EspWiFi wifi(Serial, RESET_PIN);

void setup() {
  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(RESET_PIN, HIGH); // active low
  
  if (wifi.connect(SSID, PASSWORD)) {
    if (! wifi.listen(TCP_PORT)) {
      // TODO: error
    }
  } else {
    // TODO: error
  }
}

void loop() {
  unsigned int mux_id, len;
  String data = wifi.read(mux_id, len);
  
  // send received data back
  if (data.length() > 0) {
    wifi.write(mux_id, data);
  }
}
