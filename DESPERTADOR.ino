#include <DHT11.h>
#include <LiquidCrystal.h>
#include <TimerOne.h>


LiquidCrystal lcd(6, 7, 5, 4, 3, 2);
int Ringer = 8;
int Button0 = 13;
int Button1 = 10;
int Button2 = 11;
int Button3 = 12;
DHT11 dht11(9);

// entries

// variables
int count, sec, min, hour, asec, amin, ahour, day, month, year = 0;
int tempC, temperature, humidity = 0;
bool alarm_control, time_set = false;
long measurement_timestamp;

byte Bell[8] = {
  0b00000,
  0b00100,
  0b01110,
  0b01110,
  0b11111,
  0b00000,
  0b00100,
  0b00000,
};

void setup() {

  pinMode(Ringer, OUTPUT);
  pinMode(Button0, INPUT);
  pinMode(Button1, INPUT);
  pinMode(Button2, INPUT);
  pinMode(Button3, INPUT);

  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.createChar(1, Bell);

  Timer1.initialize(100000); // Start timer
  Timer1.attachInterrupt(Stopwatch);
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.clear();
  lcd.print(String(formatDigits(hour)) + ":" + String(formatDigits(min)) + ":" + String(formatDigits(sec)) + " | " + String(formatDigits(ahour)) + ":" + String(formatDigits(amin)));
  lcd.setCursor(0,1);
  lcd.print(String(formatDigits(day)) + "/" + String(formatDigits(month)) + "/" + String(formatDigits(year)) + " | " + String());
  Alarm_On_Off();
  delay(200);

  if (digitalRead(Button1) == HIGH)
  {
    time_set = true;
    Time_Set();
  }

  if (digitalRead(Button3) == HIGH)
  {
    alarm_control = !alarm_control;
  }

  if (hour == ahour && min == amin && alarm_control == true)
  {
    Alarm_Ring();
  }
}


void Time_Set()
{
  int i = 1;
  lcd.clear();
  lcd.print("----TIME SET----");
  lcd.setCursor(0,1);
  lcd.print("----DATE SET----");
  delay(2000);
  while (time_set == true && i <= 5)
  {
    if (digitalRead(Button1) == HIGH)
    {
      i++;
    }
    switch(i) 
    {
      case 1:
        lcd.clear();
        lcd.print("  > " + String(formatDigits(hour)) + " : " + String(formatDigits(min)));
        lcd.setCursor(0,1);
        lcd.print(" " + String(formatDigits(day)) + " / " + String(formatDigits(month)) + " / " + String(formatDigits(year)));
        delay(200);
        if (digitalRead(Button2) == HIGH)
        {
          hour++;
        }
        if (digitalRead(Button3) == HIGH)
        {
          min++;
        }
        break;

      case 2:
        lcd.clear();
        lcd.print("    " + String(formatDigits(hour)) + " : " + String(formatDigits(min)));
        lcd.setCursor(0,1);
        lcd.print(">" + String(formatDigits(day)) + " / " + String(formatDigits(month)) + " / " + String(formatDigits(year)));
        delay(200);
        if (digitalRead(Button2) == HIGH)
        {
          day++;
        }
        if (digitalRead(Button3) == HIGH)
        {
          day--;
        }
        break;
      
      case 3:
        lcd.clear();
        lcd.print("    " + String(formatDigits(hour)) + " : " + String(formatDigits(min)));
        lcd.setCursor(0,1);
        lcd.print(" " + String(formatDigits(day)) + " />" + String(formatDigits(month)) + " / " + String(formatDigits(year)));
        delay(200);
        if (digitalRead(Button2) == HIGH)
        {
          month++;
        }
        if (digitalRead(Button3) == HIGH)
        {
          month--;
        }
        break;
      
      case 4:
        lcd.clear();
        lcd.print("    " + String(formatDigits(hour)) + " : " + String(formatDigits(min)));
        lcd.setCursor(0,1);
        lcd.print(" " + String(formatDigits(day)) + " / " + String(formatDigits(month)) + " />" + String(formatDigits(year)));
        delay(200);
        if (digitalRead(Button2) == HIGH)
        {
          year++;
        }
        if (digitalRead(Button3) == HIGH)
        {
          year--;
        }
        break;
    }
  }
  time_set = false; 
}

void Alarm_On_Off()
{
  if (alarm_control == true)
  {
    lcd.setCursor(10,0);
    lcd.write(1);
  }
  else{}
}

void Alarm_Set()
{
  alarm_control = true;
  if (digitalRead(Button2) == HIGH)
  {
    ahour++;
  }
  if (digitalRead(Button3) == HIGH)
  {
    amin++;
  }
}

void Alarm_Ring()
{
  while (digitalRead(Button3) == LOW)
  {
    digitalWrite(Ringer, HIGH);
  }
  digitalWrite(Ringer, LOW);
  alarm_control = false;
}

void Stopwatch()
{
  count ++;
  if(count == 10)
  {
    count = 0;
    sec++;
    if(sec >= 60)
    {
      sec = 0;
      min++;
    }
    if(min == 60)
    {
      min = 0;
      hour++;
    }
    if(hour == 24)
    {
      hour = 0;
      day ++;
    }
    if(day == 31 || day <= 0)
    {
      day = 1;
      month++;
    }
    if(month == 12 || month <= 0) 
    {
      month = 1;
      year++;
    }
    if(year == 99)
    {
      year = 0;
    }
    if(amin == 60)
    {
      amin = 0;
      ahour++;
    }
    if(ahour <= 24)
    {
      ahour = 0;
    }
  }
}

String formatDigits(int digits) {
  // Add leading zero if the number is less than 10
  if (digits < 10) {
    return "0" + String(digits);
  } else {
    return String(digits);
  }
}