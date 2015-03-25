/* vim:ts=2 sw=2 sts=2 */
/*
 * EspWiFi - William Durand <will@drnd.me> - MIT License
 *
 * WIFI driver for ESP8266 ESP-01 (well, at least tested with it)
 */
#ifndef ESP_WIFI_H
#define ESP_WIFI_H

class String;
class HardwareSerial;
class SoftwareSerial;

class EspWiFi
{
  public:
    EspWiFi(HardwareSerial & serial, const unsigned int resetPin, const unsigned long baud = 115200);
#ifdef DEBUG
    EspWiFi(HardwareSerial & serial, const unsigned int resetPin, SoftwareSerial & debug, const unsigned long baud = 115200);
#endif

    void begin();
    bool connect(String ssid, String password);
    bool startAccessPoint(String ssid, String password);
    bool listen(int port);
    bool reset();
    void end();
    void hardReset();

    String read(unsigned int mux_id, int length, unsigned long timeout = 2000);
    bool write(const unsigned int mux_id, String data);
    bool close(const unsigned int mux_id);

  private:
    bool hasBegun_ = false;
    const unsigned long baud_;
    const unsigned int resetPin_;
    HardwareSerial *serial_;
#ifdef DEBUG
    SoftwareSerial *debug_;
#endif

    bool check_esp_response(unsigned long timeout, String response);
    bool check_esp_response(unsigned long timeout, String response, String altResponse);

    void debug(String content);
};

#endif
