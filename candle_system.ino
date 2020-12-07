#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); //0x27 I2C Address -> 각자의 I2C 주소를 찾아서 수정필요
Servo servo;

int count[2] = {0, 0};
int visited[2] = {0, 0};
int btn_value[2] = {0, 0};
int time_btn_pin[2] = {2, 4};
int reset_btn_pin = 7;
int start_btn_pin = 10;
int sec = 0;
int servoPin = 9;
int angle = 0;
boolean is_start = false;
boolean is_end = false;
unsigned long start_time;
unsigned long current_time;

boolean state = false; //타이머 동작 제어

//unsigned long timeVal = 0;
unsigned long millis_time = 0; //현재시간
//unsigned long countTime = 0;  //카운트시작시간

void delay_time(unsigned long data_time)
{
  start_time = millis();
  while (millis() - start_time <= data_time){}
}

void setup()
{
  servo.attach(servoPin);
  servo.write(0);

  lcd.init(); // LiquidCrystal_I2C 1.1.2v
  lcd.backlight();
  for (int i = 0; i < sizeof(time_btn_pin); i++)
  {
    pinMode(time_btn_pin[i], INPUT_PULLUP);
  }
  pinMode(reset_btn_pin, INPUT_PULLUP);
  pinMode(start_btn_pin, INPUT_PULLUP);
 
  
}

void loop()
{

  // Setting time
  for (int i = 0; i < sizeof(time_btn_pin); i++)
  {
    btn_value[i] = digitalRead(time_btn_pin[i]);
    if (visited[i] == 0 && btn_value[i] == 0)
    {
      visited[i] = 1;
      count[i]++;
    }
    else if (visited[i] == 1 && btn_value[i] == 1)
    {
      visited[i] = 0;
    }
  }

  lcd.setCursor(0, 0);
  lcd.print("CNADY_JELLY_LOVE");
  lcd.setCursor(0, 1);
  char tbs[16];
  sprintf(tbs, "    %2dH %2dM %2ds ", count[0], count[1], sec);
  lcd.print(tbs);
  delay_time(20);
  lcd.clear();
  delay_time(20);

  /* Press Start Button
   * 
   */
  int start_state = digitalRead(start_btn_pin);
  if (start_state == 0)
  {
    is_start = true;
    millis_time = count[0] * 3600 + count[1] * 60;
    current_time = millis();
  }

  //    current_time = millis()/1000-60*countTime;

  if (is_start)
  {
    unsigned long minus_sec = 0;
    if (millis() - current_time >= 1000) {
      current_time = millis();
      minus_sec = 1;
    }

    if (millis_time > 0) {
      millis_time = millis_time - minus_sec;
      sec = millis_time % 60;
      count[1] = (millis_time / 60) % 60;
      count[0] = millis_time / 3600;
    }

    if (count[0] == 0 && count[1] == 0 && sec == 0)
    {
      servo.write(90);
//      delay(1000);
    }
  }

  //리셋
  int reset_state = digitalRead(reset_btn_pin);
  if (reset_state == 0)
  {
    servo.write(0);
//    count[0] = 0;
//    count[1] = 0;
//    sec = 0;
    state = false;
    loop();
  }
}
