/* vim:ts=2 sw=2 sts=2 */
/*
 * EspWiFi - William Durand <will@drnd.me> - MIT License
 */
#ifndef ARDUINO_H
#define ARDUINO_H

#include <string>

#define F    String
#define LOW  0
#define HIGH 1

void initialize_timer();
unsigned long millis();
void delay(unsigned long delay);
void digitalWrite(char pin, char level);

class String : public std::string
{
  public:
    String();
    String(const char * str);
    String(const String & str);
    String(std::basic_string<char> str);

    String substring(int start);
    String substring(int start, int end);
    int indexOf(String str);
    bool equals(String str);
    bool startsWith(String str);
};

class HardwareSerial
{
  public:
    HardwareSerial();

    void begin(unsigned long baud);
    int available();
    void end();

    char read();
    void print(String data);
    void print(const int data);
    void println(String data);
    void println(const int data);

    void shouldRespondWith(std::string message);
    bool hasSent(std::string message);
    void flushBuffer();

  private:
    std::string in;
    std::string out;
    int pos;
};

#endif
