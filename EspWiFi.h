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

    /**
     * Open the serial connection.
     */
    void begin();
    /**
     * Set the module as "client" and connects to `ssid` with `password`.
     */
    bool connect(String ssid, String password);
    /**
     * Set the module as "access point" and starts a WiFi network with `ssid`
     * and `password` to join it.
     *
     * IP address of the AP is: 192.168.4.1
     */
    bool startAccessPoint(String ssid, String password);
    /**
     * Start a TCP server on port `port`.
     */
    bool listen(int port);
    /**
     * Soft reset the module (`AT+RST` command).
     */
    bool reset();
    /**
     * Close the serial connection.
     */
    void end();
    /**
     * Send a `HIGH` signal to `resetPin` so that it hard resets the module.
     */
    void hardReset();
    /**
     * Read incoming data from the TCP server and return them. Note that
     * `mux_id` and `length` are updated with the right values.
     */
    String read(unsigned int mux_id, int length, unsigned long timeout = 2000);
    /**
     * Send data to a TCP channel (`mux_id`).
     */
    bool write(const unsigned int mux_id, String data);
    /**
     * Close the TCP connection with a client, not the serial connection (see
     * `end()` method for that).
     */
    bool close(const unsigned int mux_id);

  private:
    bool begun_ = false;
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
