#include <Wire.h>
#include <VL6180X.h>

#include <Servo.h> // Servo library

Servo servo1, servo2, servo3, servo4;

VL6180X sensor_A, sensor_B;

#define GPIO_A 3
#define GPIO_B 4

Servo servo_list[2][2] = {{servo1, servo2}, {servo3, servo4}}; // 10, 11, 12, 13
int led_pin_list[2][2] = {{6, 7}, {8, 9}};
bool floor_state[2][2] = {{false, false}, {false, false}};

void setup()
{

    // set the pin mode to OUTPUT for led pin
    for (int i = 0; i < 4; i++)
    {
        pinMode(i + 6, OUTPUT);
    }

    //Set the pin mode to output
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

void turn_on(int row_number, int pin_number_of_on)
{
    // 해당 층수가 켜져있으면
    if (floor_state[row_number][pin_number_of_on])
    {
        floor_state[row_number][pin_number_of_on] = false;
        digitalWrite(led_pin_list[row_number][pin_number_of_on], LOW); // led off
        servo_on(row_number, pin_number_of_on);                        // activate servo
    }
    // 해당 층수가 꺼져있으면
    else
    {
        floor_state[row_number][pin_number_of_on] = true;
        digitalWrite(led_pin_list[row_number][pin_number_of_on], HIGH); // led on
        servo_on(row_number, pin_number_of_on);                         // activate servo
    }
}

void servo_off(int servo_number)
{
    for (int j = 0; j < 2; j++)
    {
        for (int i = 0; i < 2; i++)
        {
            if (i != servo_number)
            {
                servo_list[j][i].detach();
            }
        }
    }
}

void servo_on(int row_number, int level_number)
{
    int index = 0;
    if (row_number == 1)
    {
        index = 2;
    }
    servo_off(level_number);
    delay(100);
    servo_list[row_number][level_number].attach(level_number + index + 10);
    delay(100);
    servo_list[row_number][level_number].write(60);
    delay(500);
    servo_list[row_number][level_number].write(0);
}

void loop()
{
    int centimeters_1 = sensor_A.readRangeSingleMillimeters() / 10;
    int centimeters_2 = sensor_B.readRangeSingleMillimeters() / 10;

    if (centimeters_1 > 15)
    {
        servo_off(99);
    }
    else
    {
        if (centimeters_1 < 5)
        {
            turn_on(0, 0);
            Serial.print("SENSOR A ");
            Serial.println("1층");
            delay(1000);
        }
        else if (centimeters_1 < 8)
        {
            turn_on(0, 1);
            Serial.print("SENSOR A ");
            Serial.println("2층");
            delay(1000);
        }
    }

    if (centimeters_2 > 15)
    {
        servo_off(99);
    }
    else
    {
        if (centimeters_2 < 5)
        {
            turn_on(1, 0);
            Serial.print("SENSOR B ");
            Serial.println("1층");
            delay(1000);
        }
        else if (centimeters_2 < 8)
        {
            turn_on(1, 1);
            Serial.print("SENSOR B ");
            Serial.println("2층");
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