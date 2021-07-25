#include "utli.h"
#include  <Ticker.h>

static Ticker blinker;
static int g_pin;

void changeState()
{
    digitalWrite(g_pin, !(digitalRead(g_pin))); 
}

void led_blink_slow(int pin)
{
    pinMode(pin,OUTPUT);
    g_pin = pin;
    blinker.attach(0.5, changeState);
}

void led_blink_fast(int pin)
{
    pinMode(pin,OUTPUT);
    g_pin = pin;
    blinker.attach(0.2, changeState);
}

void led_light(int pin)
{
    blinker.detach();
    digitalWrite(pin, LOW);
}
