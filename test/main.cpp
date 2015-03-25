#include <Arduino.h>
#include <iostream>

#include "../EspWiFi.h"

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#define RESET_PIN 4

HardwareSerial serial;
EspWiFi wifi(serial, RESET_PIN);

TEST_CASE("begin()")
{
  serial.shouldRespondWith("OK");
  serial.flushBuffer();

  wifi.begin();

  REQUIRE(serial.hasSent("AT\r\n"));
}

TEST_CASE("connect()")
{
  serial.shouldRespondWith("OK");
  serial.flushBuffer();

  wifi.connect("foo", "bar");

  REQUIRE_FALSE(serial.hasSent("AT\r\n"));
  REQUIRE(serial.hasSent("AT+CWMODE=1\r\nAT+RST\r\n"));
  // Because of how the Serial mock is written it is not possible to specify
  // more than one response (yet)... So we can't test the complete behavior
  // of this method :-(
}

TEST_CASE("close()")
{
  serial.shouldRespondWith("OK");
  serial.flushBuffer();

  bool res = wifi.close(123);

  REQUIRE(serial.hasSent("AT+CIPCLOSE=123\r\n"));
  REQUIRE(res);
}

TEST_CASE("read()")
{
  unsigned int mux_id = 0, len = 0;

  serial.shouldRespondWith("+IPD,123,16:babar is awesome");

  String response = wifi.read(mux_id, len);

  REQUIRE(mux_id == 123);
  REQUIRE(len == 16);
  REQUIRE(response.equals("babar is awesome"));
}

TEST_CASE("read() empty content")
{
  unsigned int mux_id = 0, len = 0;

  serial.shouldRespondWith("+IPD,123,0:<this should not be taken into account>");

  String response = wifi.read(mux_id, len);

  REQUIRE(mux_id == 123);
  REQUIRE(len == 0);
  REQUIRE(response.equals(""));
}

TEST_CASE("write()")
{
  serial.shouldRespondWith(">");
  serial.flushBuffer();

  bool res = wifi.write(123, "I <3 Babar");

  REQUIRE(serial.hasSent("AT+CIPSEND=123,10\r\nI <3 Babar"));
}

TEST_CASE("write() empty data")
{
  serial.shouldRespondWith(">");
  serial.flushBuffer();

  bool res = wifi.write(123, "");

  REQUIRE(serial.hasSent("AT+CIPSEND=123,0\r\n"));
}

int main( int argc, char* const argv[] )
{
  initialize_timer();

  int result = Catch::Session().run( argc, argv );

  return result;
}
