/* vim:ts=2 sw=2 sts=2 */
/*
 * EspEspWiFi - William Durand <will@drnd.me> - MIT License
 */
#include "EspWiFi.h"

#include <Arduino.h>
#include <stdio.h>
#ifdef DEBUG
#include <SoftwareSerial.h>
#endif

EspWiFi::EspWiFi(
  HardwareSerial & serial,
  const unsigned int resetPin,
  const unsigned long baud
) : serial(& serial), resetPin(resetPin), baud(baud), begun(false)
{
}

#ifdef DEBUG
EspWiFi::EspWiFi(
  HardwareSerial & serial,
  const unsigned int resetPin,
  SoftwareSerial & debug,
  const unsigned long baud
) :  serial(& serial), resetPin(resetPin), debugSerial(& debug), baud(baud), begun(false)
{
}
#endif

void EspWiFi::begin()
{
  serial->begin(baud);

  serial->println(F("AT"));

  if (! check_esp_response(1000, F("OK"))) {
    debug("Error AT");
    return;
  }

  begun = true;
}

bool EspWiFi::connect(String ssid, String password)
{
  if (! begun) {
    begin();
  }

  // 1 = client
  serial->println(F("AT+CWMODE=1"));

  if (! check_esp_response(1000, F("OK"))) {
    debug("Error AT+CWMODE");
    return false;
  }

  if (! reset()) {
    debug("Error reset() in connect()");
    return false;
  }

  serial->println(F("AT+CWJAP?"));

  if (! check_esp_response(1000, ssid)) {
    serial->print(F("AT+CWJAP=\""));
    serial->print(ssid);
    serial->print(F("\",\""));
    serial->print(password);
    serial->println(F("\""));

    if (! check_esp_response(5000, F("OK"))) {
      debug("Error AT+CWJAP");
      return false;
    }
  }

  return true;
}

bool EspWiFi::startAccessPoint(String ssid, String password)
{
  if (! begun) {
    begin();
  }

  // 3 = client + AP
  serial->println(F("AT+CWMODE=3"));

  if (! check_esp_response(1000, F("OK"))) {
    debug("Error AT+CWMODE");
    return false;
  }

  if (! reset()) {
    debug("Error reset() in startAccessPoint()");
    return false;
  }

  serial->print(F("AT+CWSAP=\""));
  serial->print(ssid);
  serial->print(F("\",\""));
  serial->print(password);
  serial->println(F("\",3,0"));

  if (! check_esp_response(1000, F("OK"))) {
    debug("Error AT+CWSAP");
    return false;
  }

  return true;
}

bool EspWiFi::listen(int port)
{
  serial->println(F("AT+CIPMUX=1"));

  if (! check_esp_response(1000, F("OK"))) {
    debug("Error AT+CIPMUX");
    return false;
  }

  serial->print(F("AT+CIPSERVER=1,"));
  serial->println(port);

  if (! check_esp_response(1000, F("OK"), F("no change"))) {
    debug("Error AT+CIPSERVER");
    return false;
  }

  return true;
}

String EspWiFi::read(unsigned int & mux_id, unsigned int & length, unsigned long timeout)
{
  String data;

  bool isBlank    = false;
  unsigned long t = millis();

  char c;
  while (millis() - t < timeout) {
    if (serial->available()) {
      c = serial->read();

      if (c == '\n' && isBlank) {
        break;
      }

      if (c == '\n') {
        isBlank = true;
      }
      else if (c != '\r') {
        isBlank = false;
      }

      data += c;
    }
  }

  if (data.startsWith("+IPD,")) {
    // request: +IPD,ch,len:data
    sscanf(data.substring(5, data.indexOf(":")).c_str(), "%u,%u", & mux_id, & length);
    data = data.substring(data.indexOf(":") + 1);

    t = millis();
    while (data.length() < length && millis() - t < timeout) {
      if (serial->available()) {
        c = serial->read();
        data += c;
      }
    }

    data = data.substring(0, length);

    // empty buffer
    while (serial->available()) {
      serial->read();
    }

    return data;
  }

  return "";
}

bool EspWiFi::write(const unsigned int mux_id, String data)
{
  serial->print(F("AT+CIPSEND="));
  serial->print(mux_id);
  serial->print(",");
  serial->println(data.length());

  if (check_esp_response(2000, F(">"))) {
    serial->print(data);

    return check_esp_response(1000, F("SEND OK"));
  }

  return close(mux_id);
}

bool EspWiFi::close(const unsigned int mux_id)
{
  serial->print(F("AT+CIPCLOSE="));
  serial->println(mux_id);

  return check_esp_response(1000, F("OK"));
}

void EspWiFi::end()
{
  serial->end();
  begun = false;
}

bool EspWiFi::reset()
{
  serial->println(F("AT+RST"));

  if (! check_esp_response(1000, F("OK"))) {
    debug("Error AT+RST");
    return false;
  }

  unsigned long t = millis();
  while (millis() - t < 10000) {
    serial->println(F("AT"));

    if (check_esp_response(1000, F("OK"))) {
      delay(1000);
      return true;
    }

    delay(500);
  }

  return false;
}

void EspWiFi::hardReset()
{
  end();

  digitalWrite(resetPin, LOW);
  delay(1000);
  digitalWrite(resetPin, HIGH);
  delay(3000);
}

bool EspWiFi::check_esp_response(unsigned long timeout, String response)
{
  char c;
  String buffer;
  bool found = false;
  unsigned long t = millis();

  while (millis() - t < timeout) {
    if (serial->available() > 0) {
      c = serial->read();
      buffer += c;

      if (buffer.indexOf(response) != -1 || buffer.equals(response)) {
        found = true;

        break;
      }
    }
  }

  while (serial->available() > 0) {
    c = serial->read();
    buffer += c;
  }

  debug(buffer);

  return found;
}

bool EspWiFi::check_esp_response(unsigned long timeout, String response, String altResponse)
{
  char c;
  String buffer;
  bool found = false;
  unsigned long t = millis();

  while (millis() - t < timeout) {
    if (serial->available() > 0) {
      c = serial->read();
      buffer += c;

      if (buffer.indexOf(response) != -1 || buffer.equals(response)
        || buffer.indexOf(altResponse) != -1 || buffer.equals(altResponse)) {
        found = true;

        break;
      }
    }
  }

  while (serial->available() > 0) {
    c = serial->read();
    buffer += c;
  }

  debug(buffer);

  return found;
}

void EspWiFi::debug(String content)
{
#ifdef DEBUG
  debugSerial->println(content);
  delay(100);
#endif
}
