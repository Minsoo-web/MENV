#include <Wire.h>
#include <VL6180X.h>
#include <ShiftRegister74HC595.h>
#include <Servo.h> // Servo library

VL6180X sensor_A, sensor_B;

#define GPIO_A 5
#define GPIO_B 6

ShiftRegister74HC595<1> sr(2, 3, 4);

Servo servo0,servo1,servo2,servo3,servo4,servo5,servo6,servo7; 
Servo servo_list[8] = {servo0, servo1,servo2,servo3,servo4,servo5,servo6,servo7};

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

void servo_off(int servo_number) 
{
  for(int j=0; j <8; j++)
  {  
    if(j != servo_number)
    {
        servo_list[j].detach();
    }
  }
}

void servo_on(int pin_number)
{

  servo_off(pin_number);
  delay(100);
  if(pin_number == 7) {
  //    
    servo_list[pin_number].attach(A0);
  }else {
    servo_list[pin_number].attach(pin_number + 7);
  }
  delay(100);
  servo_list[pin_number].write(60);
  delay(500);
  servo_list[pin_number].write(0);
}


uint8_t read_led_state(int pin_number)
{
    return sr.get(pin_number);
}

void led_on(int pin_number)
{
    int floor_state = read_led_state(pin_number);
    
    if (floor_state == 0)
    {
        // 꺼져있었으면
        sr.set(pin_number, HIGH);
        
    }
    else
    {
        // 켜져있었으면
        sr.set(pin_number, LOW);
    }
    servo_on(pin_number);
}


void loop()
{
    int range_status_A = sensor_A.readRangeSingleMillimeters();
    int range_status_B = sensor_B.readRangeSingleMillimeters();


    if (range_status_A == 255)
    {
        servo_off(99);
    }
    else
    {
        int centimeters_1 = range_status_A / 10;
        // 3층 -> 7번 핀     
        if (centimeters_1 < 6)
        {
            led_on(7);
            Serial.print("SENSOR A ");
            Serial.println("3층");
            delay(1000);
        }
        // 2층 -> 6번 핀
        else if (centimeters_1 < 12)
        {
            led_on(6);
            Serial.print("SENSOR A ");
            Serial.println("2층");
            delay(1000);
        }
        // 1층 -> 5번 핀
        else if (centimeters_1 < 18)
        {
            led_on(5);
            Serial.print("SENSOR A ");
            Serial.println("1층");
            delay(1000);
        }
        // 열림 -> 4번 핀
        else if (centimeters_1 < 24)
        {
            led_on(4);
            Serial.print("SENSOR A ");
            Serial.println("열림");
            delay(1000);
        }
    }

    if (range_status_B == 255)
    {
       servo_off(99);
    }
    else
    {
        int centimeters_2 = range_status_B / 10;
        // 6층 -> 3번 핀
        if (centimeters_2 < 6)
        {
            led_on(3);
            Serial.print("SENSOR B ");
            Serial.println("6층");
            delay(1000);
        }
        // 5층 -> 2번 핀
        else if (centimeters_2 < 12)
        {
            led_on(2);
            Serial.print("SENSOR B ");
            Serial.println("5층");
            delay(1000);
        }
        // 4층 -> 1번 핀
        else if (centimeters_2 < 18)
        {
            led_on(1);      
            Serial.print("SENSOR B ");
            Serial.println("4층");
            delay(1000);
        }
        // 닫힘 -> 0번 핀
        else if (centimeters_2 < 24)
        {
            led_on(0);            
            Serial.print("SENSOR B ");
            Serial.println("닫힘");
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