#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <WiFi.h>
#include <Math.h>
using namespace std;

#define LED_PIN 27

U8G2_SSD1306_128X80_NONAME_F_HW_I2C u8g2(U8G2_R1, /* reset=*/U8X8_PIN_NONE, /* clock=*/22, /* data=*/21);
void connect_wifi(String ssid, String password);
void display(string msg);

bool blinkState = false;

void setup(void)
{
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    u8g2.begin();
    u8g2.enableUTF8Print();                     // enable UTF8 support for the Arduino print() function
    u8g2.setFont(u8g2_font_unifont_t_chinese2); // use chinese2 for all the glyphs of "你好世界"
    u8g2.setFontDirection(0);
    connect_wifi("熊竹的iPhone", "11111111*&");
}

void loop(void)
{
    delay(1000);
}

void connect_wifi(String ssid, String password)
{
    display("scanning wifi...");
    int n = WiFi.scanNetworks();
    display("scan done");
    if (n == 0)
    {
        display("no networks found");
    }
    else
    {
        Serial.print(n);
        display("networks found");
        for (int i = 0; i < n; ++i)
        {
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
            delay(10);
        }
    }
    display((String("connecting ") + ssid).c_str());

    WiFi.begin(ssid.c_str(), password.c_str());
    int c = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
        String ss[] = {"/", "-", "\\", "|"};
        display((String("connecting ") + ssid + ss[c % 4]).c_str());
        blinkState = !blinkState;
        digitalWrite(LED_PIN, blinkState);
        c++;
    }
    digitalWrite(LED_PIN, false);
    display(("WiFi connected\nIP address:\n" + WiFi.localIP().toString()).c_str());
}

void display(string msg)
{
    int cursor = 0;
    size_t pos = 0;
    string token;
    u8g2.clearBuffer();
    do
    {
        pos = msg.find("\n");
        if (pos != std::string::npos && pos < 16)
        {
            token = msg.substr(0, pos);
            msg.erase(0, pos + 1);
        }
        else
        {
            token = msg.substr(0, msg.length() < 16 ? msg.length() : 16);
            msg.erase(0, msg.length() < 16 ? msg.length() : 16);
        }
        u8g2.setCursor(0, 16 * (cursor + 1) - 1);
        u8g2.print(token.c_str());
        cursor++;
    } while (msg.length() > 0);
    u8g2.sendBuffer();
}
