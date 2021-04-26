#include <Wire.h>
#include <VL6180X.h>
#include <ShiftRegister74HC595.h>

VL6180X sensor_A, sensor_B;

#define GPIO_A 3
#define GPIO_B 4

// create a global shift register object
// parameters: <number of shift registers> (data pin, clock pin, latch pin)
ShiftRegister74HC595<2> sr(0, 1, 2);

void setup()
{
    // GPIO Pin
    pinMode(GPIO_A, OUTPUT);
    pinMode(GPIO_B, OUTPUT);

    // Turn all TOF's off
    digitalWrite(GPIO_A, LOW);
    digitalWrite(GPIO_B, LOW);

    Serial.begin(9600);
    Wire.begin();

    delay(50);
    digitalWrite(GPIO_A, HIGH); //Turn sensor_A on
    delay(50);

    // Start Sensor_A
    sensor_A.init();
    sensor_A.configureDefault();
    sensor_A.setTimeout(500);
    sensor_A.setAddress(0x30); // set Tof_A address

    delay(50);
    digitalWrite(GPIO_B, HIGH); //Turn sensor_B on
    delay(50);

    // Start Sensor_B
    sensor_B.init();
    sensor_B.configureDefault();
    sensor_B.setTimeout(500);
    sensor_B.setAddress(0x31); // set Tof_A address
}

int read_led_state(int pin_number)
{
    return digitalRead(pin_number);
}

void led_on(int pin_number)
{
    int floor_state = read_led_state(pin_number);

    if (floor_state == 0)
    {
        // 꺼져있었으면
        sr.set(9, HIGH);
    }
    else
    {
        // 켜져있었으면
        sr.set(9, LOW);
    }
}

void roop()
{
    int centimeters_1 = sensor_A.readRangeSingleMillimeters() / 10;
    int centimeters_2 = sensor_B.readRangeSingleMillimeters() / 10;

    // 4 층 -> 9번 핀
    if (centimeters_1 < 6)
    {
        led_on(9);
    }
    // 3 층 -> 8번 핀
    else if (centimeters_1 < 12)
    {
        led_on(8);
    }
    // 2 층 -> 7번 핀
    else if (centimeters_1 < 18)
    {
        led_on(7);
    }
    // 1 층 -> 6번 핀
    else if (centimeters_1 < 24)
    {
        led_on(6);
    }
    // 열림 -> 5번 핀
    else if (centimeters_1 < 30)
    {
        led_on(5);
    }

    // 8층 -> 4번 핀
    if (centimeters_2 < 6)
    {
        led_on(4);
    }
    // 7 층 -> 3번 핀
    else if (centimeters_2 < 12)
    {
        led_on(3);
    }
    // 6 층 -> 2번 핀
    else if (centimeters_2 < 18)
    {
        led_on(2);
    }
    // 5 층 -> 1번 핀
    else if (centimeters_2 < 24)
    {
        led_on(1);
    }
    // 닫힘 -> 0번 핀
    else if (centimeters_2 < 30)
    {
        led_on(0);
    }

    // 에러 처리
    if (sensor_A.timeoutOccurred())
    {
        Serial.print(" TIMEOUT");
    }
    if (sensor_B.timeoutOccurred())
    {
        Serial.print(" TIMEOUT");
    }
}