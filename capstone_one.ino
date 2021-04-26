#include <Wire.h>
#include <VL6180X.h>
#include <ShiftRegister74HC595.h>

VL6180X sensor_A, sensor_B;

#define GPIO_A 5
#define GPIO_B 6

// 이 데이터 핀을 통해 시프트 레지스터에 원하는 동작을 수행하게 합니다.
int datapin = 4;

// 클럭 입력이 활성화되면. 입력을 받아 저장하고,
// 클럭 입력이 비활성화되면 출력 측으로 전달합니다.
// 출력은 클럭이 다시 활성화 될 때 까지 그 값을 유지할 것입니다.
int latchpin = 2;

// 클럭 펄스를 발생시켜 데이터 값이 레지스터에 저장되도록 합니다.
int clockpin = 3;

ShiftRegister74HC595<2> sr(4, 3, 2);

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

void led_on(int turn_on)
{
    digitalWrite(latchpin, LOW);

    // shiftOut 함수 사용시 인자 값
    // 데이터로 입력된 값을 클럭펄스로 동기화하여 시프트레지스터에 저장합니다.
    // DS핀넘버, CLock핀넘버, MSBFIRST or LSBFIRST, 옮길자료(byte)
    shiftOut(datapin, clockpin, MSBFIRST, 1 << turn_on);

    // latch를 활성화하여 저장된 입력된 데이터를 출력핀으로 전달합니다.
    digitalWrite(latchpin, HIGH);
}

void loop()
{

    int centimeters_1 = sensor_A.readRangeSingleMillimeters() / 10;
    int centimeters_2 = sensor_B.readRangeSingleMillimeters() / 10;

    if (centimeters_1 > 15)
    {
        //        servo_off(99);
    }
    else
    {
        if (centimeters_1 < 5)
        {
            led_on(0);
            Serial.print("SENSOR A ");
            Serial.println("1층");
            delay(1000);
        }
        else if (centimeters_1 < 8)
        {
            led_on(1);
            Serial.print("SENSOR A ");
            Serial.println("2층");
            delay(1000);
        }
        else if (centimeters_1 < 11)
        {
            led_on(2);
            Serial.print("SENSOR A ");
            Serial.println("3층");
            delay(1000);
        }
        else if (centimeters_1 < 13)
        {
            led_on(3);
            Serial.print("SENSOR A ");
            Serial.println("4층");
            delay(1000);
        }
    }

    if (centimeters_2 > 15)
    {
        //        servo_off(99);
    }
    else
    {
        if (centimeters_2 < 5)
        {
            led_on(4);
            Serial.print("SENSOR B ");
            Serial.println("5층");
            delay(1000);
        }
        else if (centimeters_2 < 8)
        {
            led_on(5);
            Serial.print("SENSOR B ");
            Serial.println("6층");
            delay(1000);
        }
        else if (centimeters_2 < 11)
        {
            led_on(6);
            Serial.print("SENSOR B ");
            Serial.println("7층");
            delay(1000);
        }
        else if (centimeters_2 < 13)
        {
            led_on(7);
            Serial.print("SENSOR B ");
            Serial.println("8층");
            delay(1000);
        }
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