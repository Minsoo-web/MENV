// 2021 / 05 / 04 테스트 중

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

int floor_delay = 200;
int servo_delay = 500;

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
    sensor_A.setScaling(2);
    sensor_A.setTimeout(500);
    sensor_A.setAddress(0x30); // set Tof_A address

    

    delay(50);
    digitalWrite(GPIO_B, HIGH); //Turn sensor_B on
    delay(50);

    // Start Sensor_B
    sensor_B.init();
    sensor_B.configureDefault();
    sensor_B.setScaling(2);
    sensor_B.setTimeout(500);
    sensor_B.setAddress(0x31); // set Tof_A address
}

void servo_off(int servo_number) 
{
  if(servo_number == 99) {
    low_open_close(4);
  }

  if(servo_number == 98) {
    low_open_close(1);
  }
  
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
    low_open_close(1);
    low_open_close(4);
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

void led_off(int pin_number)
{
  sr.set(pin_number, LOW);
}


// 열림, 닫힘
void high_open_close(int pin_number){
  sr.set(pin_number, HIGH); 
  
  servo_list[pin_number].attach(pin_number + 7);
  delay(100);
  servo_list[pin_number].write(60);
}

void low_open_close(int pin_number){
  if(pin_number == 1){
    sr.set(0, LOW);

    servo_list[pin_number].attach(7);
    delay(100);
    servo_list[pin_number].write(0);
  }else {
    sr.set(pin_number, LOW);
  
    servo_list[pin_number].attach(pin_number + 7);
    delay(100);
    servo_list[pin_number].write(0);  
  }
}


//
void loop()
{
    double range_status_A = sensor_A.readRangeSingleMillimeters();
    double range_status_B = sensor_B.readRangeSingleMillimeters();


    double centimeters_1 = range_status_A / 10;
    double centimeters_2 = range_status_B / 10;
  
    if (centimeters_1 > 29)
    {
        servo_off(99);
    }
    else
    {
        delay(floor_delay);
      
        range_status_A = sensor_A.readRangeSingleMillimeters();
        centimeters_1 = range_status_A / 10;
      
        // 3층 -> 7번 핀     
        if (centimeters_1 < 7.2)
        {
            led_on(7);
            Serial.print("SENSOR A ");
            Serial.println("3층");
            Serial.println(centimeters_1);
            delay(servo_delay);
        }
        // 2층 -> 6번 핀
        else if (centimeters_1 < 14.4)
        {
            led_on(6);
            Serial.print("SENSOR A ");
            Serial.println("2층");
            Serial.println(centimeters_1);
            delay(servo_delay);
        }
        // 1층 -> 5번 핀
        else if (centimeters_1 < 22.6)
        {
            led_on(5);
            Serial.print("SENSOR A ");
            Serial.println("1층");
            Serial.println(centimeters_1);
            delay(servo_delay);
        }
        // 열림 -> 4번 핀
        else if (centimeters_1 < 29)
        {
            high_open_close(4);
            Serial.print("SENSOR A ");
            Serial.println("열림");
            Serial.println(centimeters_1);
            delay(servo_delay);
        }
    }

    if (centimeters_2 > 29)
    {
       servo_off(98);
    }
    else
    {
        delay(floor_delay);
        
        range_status_B = sensor_B.readRangeSingleMillimeters();
        centimeters_2 = range_status_B / 10;
      
        // 6층 -> 3번 핀
        if (centimeters_2 < 7.2)
        {
            led_on(3);
            Serial.print("SENSOR B ");
            Serial.println("6층");
            delay(servo_delay);
        }
        // 5층 -> 2번 핀
        else if (centimeters_2 < 14.4)
        {
            led_on(2);
            Serial.print("SENSOR B ");
            Serial.println("5층");
            delay(servo_delay);
        }
        // 4층 -> 1번 핀
        else if (centimeters_2 < 22.6)
        {
            led_on(1);      
            Serial.print("SENSOR B ");
            Serial.println("4층");
            delay(servo_delay);
        }
        // 닫힘 -> 0번 핀
        else if (centimeters_2 < 29.8)
        {
            high_open_close(0);
            Serial.print("SENSOR B ");
            Serial.println("닫힘");
            delay(servo_delay);
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