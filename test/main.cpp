#include <Arduino.h>
#include <iostream>

#include "../EspWiFi.h"

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#define RESET_PIN 4

HardwareSerial serial;
EspWiFi wifi(serial, RESET_PIN);

TEST_CASE("test_begin")
{
  serial.shouldRespondWith("OK");
  serial.flushBuffer();

  wifi.begin();

  REQUIRE(serial.hasSent("AT\r\n"));
}

TEST_CASE("test_connect")
{
  serial.shouldRespondWith("OK");
  serial.flushBuffer();

  wifi.connect("foo", "bar");

  REQUIRE_FALSE(serial.hasSent("AT\r\n"));
  REQUIRE(serial.hasSent("AT+CWMODE=1\r\n"));
  REQUIRE(serial.hasSent("AT+RST\r\n"));
  REQUIRE(serial.hasSent("AT+CWJAP?\r\n"));
  REQUIRE(serial.hasSent("AT+CWJAP=\"foo\",\"bar\"\r\n"));
}

TEST_CASE("test_close")
{
  serial.shouldRespondWith("OK");
  serial.flushBuffer();

  wifi.close(123);

  REQUIRE(serial.hasSent("AT+CIPCLOSE=123"));
}

int main( int argc, char* const argv[] )
{
  initialize_timer();

  int result = Catch::Session().run( argc, argv );

  return result;
}
