#include "Arduino.h"
#include <sys/timeb.h>

timeb t_start;

void initialize_timer()
{
  ftime(&t_start);
}

unsigned long millis()
{
  timeb t_now;
  ftime(&t_now);
  return (t_now.time  - t_start.time) * 1000 + (t_now.millitm - t_start.millitm);
}

void delay(unsigned long ms)
{
  unsigned long start = millis();
  while(millis() - start < ms) {}
}

void digitalWrite(char pin, char level)
{
}

/* String */

String::String() : std::string() {}

String::String(const String & str) : std::string(str) {}

String::String(const char * str) : std::string(str) {}

String::String(std::basic_string<char> str) : std::string(str) {}

String String::substring(int start)
{
  return String(substr(start, length()));
}

String String::substring(int start, int end)
{
  return String(substr(start, end - start));
}

int String::indexOf(String str)
{
  return find(str);
}

bool String::equals(String str)
{
  return compare(str) == 0 ? true : false;
}

bool String::startsWith(String str)
{
  return find(str) == 0 ? true : false;
}

/* HardwareSerial */

HardwareSerial::HardwareSerial() {}

void HardwareSerial::begin(unsigned long baud)
{
}

int HardwareSerial::available()
{
  return out.length() - pos;
}

void HardwareSerial::end()
{
}

char HardwareSerial::read()
{
  return out.at(pos++);
}

void HardwareSerial::print(String data)
{
  in.append(data);
}

void HardwareSerial::print(const int data)
{
  print(std::to_string(data));
}

void HardwareSerial::println(String data)
{
  in.append(data);
  in.append("\r\n");
}

void HardwareSerial::println(const int data)
{
  println(std::to_string(data));
}

void HardwareSerial::shouldRespondWith(std::string message)
{
  out = message;
  pos = 0;
}

bool HardwareSerial::hasSent(std::string message)
{
  return 0 == in.compare(message);
}

void HardwareSerial::flushBuffer()
{
  in.clear();
}
