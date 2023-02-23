#include "relogio.h"
#include <Wire.h>
#include <MAX30100_PulseOximeter.h>
#include <RTClib.h>            //biblioteca para RTC module
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire, OLED_RESET);

// .... Variáveis - MENU ....
int button = 12;
int count = 0;

// .... Variáveis - RTCMODULE ....
RTC_DS1307 rtc;  //Objeto do tipo RTC_DS1307
char daysOfTheWeek [7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sabado"};

#define ENABLE_MAX30100 1

#if ENABLE_MAX30100
#define REPORTING_PERIOD_MS 5000
//PulseOximeter is the higher level interface to the sensor
//it offers:
//     * beat detection reporting 
//     * heart rate calculation
//     * SpO2 (oxidation level) calculation
PulseOximeter pox;
#endif

uint32_t tsLastReport = 0;
int xPos = 0;
int xPos1 = 0;

void onBeatDetected()
{
  Serial.println("Beat!");
  heart_beat(&xPos);
}

void onBeatDetected1()
{
  Serial.println("Beat!");
  heart_beat(&xPos1);
}

amostra* rel_env;

void relogio_init(amostra* x){
    rel_env = x;
    if(!display.begin(SSD1306_SWITCHCAPVCC,0x3C))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;);   //Don't proceed, loop forever
    }
    //Show initial display buffer contexts on the screen --
    //The library initialize this with an Adafruit splash screen.
    xPos = 0;
    display.cp437(true);

    display.clearDisplay();
    Serial.print("Initializing pulse oximeter..");

    #if ENABLE_MAX30100
    //Initialize the PulseOximeter instance
    //Failure are generally due to as improper I2C wiring, missing power supply
    //or wrong target chip
    if(!pox.begin()){
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCESS");
    }
    pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
    //Register a callback for the beat detection
    pox.setOnBeatDetectedCallback(onBeatDetected);
    display_data(0,0);
    #endif 

    //  :::::::::::: RTCMODULE  ::::::::::::
    if(!rtc.begin())                          //Se o RTC não for inicializado, faz:
    {
        Serial.println("DS1307 não encontrado!"); 
        while(1);  //Entra em loop
    }
    if(!rtc.isrunning()) //Se o RTC estiver a ser executado, faz:
    {
        Serial.println("DS1307 a funcionar!");
    }
    delay(100);

    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //Captura a data e hora em que o sketch é compilado
    rtc.adjust(DateTime(2023, 2, 10, 18, 13, 0));   //(Ano), (Mês), (Dia), (Hora), (Minutos), (Segundos)

    //Clear the buffer
    display.clearDisplay();
    display.display();
}

void getbpm_spO2()
{
  uint32_t tsLastReport = 0;
  #if ENABLE_MAX30100
  //Make sure to call uspdate as fast as possible
  pox.update();
  int bpm = 0;
  int spo2 = 0;
  //Asynchronously dump heart rate and oxidation levels to the serial
  //For both, a value of 0 means "invalid"
  if(millis() - tsLastReport > REPORTING_PERIOD_MS){
    //Serial.print("Heart rate:");
    bpm = pox.getHeartRate();
    rel_env[0].valor = bpm; rel_env[0].lido = true;
    spo2 = pox.getSpO2();
    rel_env[1].valor = spo2; rel_env[1].lido = true;
    //todo:
    //Serial.print(bpm);
    tsLastReport = millis();
    display_data(bpm, spo2);
  }
  #endif
  drawLine(&xPos);
}

void display_data(int bpm, int spo2)
{
  display.fillRect(0, 18, 127, 15, SSD1306_BLACK);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(20,25);
  //Display static text
  display.print("BPM ");
  display.print(bpm);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(70, 25);
  //Display static text
  display.print("SpO2 ");
  display.print(spo2);
  display.display();
}

void drawLine(int *x_pos)
{
  //Draw a single pixel in white
  display.drawPixel(*x_pos, 8, SSD1306_WHITE);
  display.drawPixel((*x_pos)++, 8, SSD1306_WHITE);
  display.drawPixel((*x_pos)++, 8, SSD1306_WHITE);
  display.drawPixel((*x_pos)++, 8, SSD1306_WHITE);
  display.drawPixel((*x_pos), 8, BLACK);   //-----
  //Serial.println(*x_pos);
  display.fillRect(*x_pos, 0, 31, 16, SSD1306_BLACK);
  display.display();
  delay(1);
  if(*x_pos >= 128){
    *x_pos = 0;
  }
}

void heart_beat(int *x_pos){
  /************************************************/
  //display.clearDisplay();
  display.fillRect(*x_pos, 0, 30, 15, SSD1306_BLACK);
  //Draw a single pixel in white
  display.drawPixel(*x_pos + 0, 8, SSD1306_WHITE);
  display.drawPixel(*x_pos + 1, 8, SSD1306_WHITE);
  display.drawPixel(*x_pos + 2, 8, SSD1306_WHITE);
  display.drawPixel(*x_pos + 3, 8, SSD1306_WHITE);
  display.drawPixel(*x_pos + 4, 8, BLACK); // -----
  //display.display();
  //delay(1);
  display.drawPixel(*x_pos + 5, 7, SSD1306_WHITE);
  display.drawPixel(*x_pos + 6, 6, SSD1306_WHITE);
  display.drawPixel(*x_pos + 7, 7, SSD1306_WHITE); // .~.
  //display.display();
  //delay(1);
  display.drawPixel(*x_pos + 8, 8, SSD1306_WHITE);
  display.drawPixel(*x_pos + 9, 8, SSD1306_WHITE); // --
  //display.display();
  //delay(1);
  /******************************************/
  display.drawPixel(*x_pos + 10, 8, SSD1306_WHITE);
  display.drawPixel(*x_pos + 10, 9, SSD1306_WHITE);
  display.drawPixel(*x_pos + 11, 10, SSD1306_WHITE);
  display.drawPixel(*x_pos + 11, 11, SSD1306_WHITE);
  //display.display();
  //delay(1);
  /******************************************/
  display.drawPixel(*x_pos + 12, 10, SSD1306_WHITE);
  display.drawPixel(*x_pos + 12, 9, SSD1306_WHITE);
  display.drawPixel(*x_pos + 12, 8, SSD1306_WHITE);
  display.drawPixel(*x_pos + 12, 7, SSD1306_WHITE);
  //display.display();
  //delay(1);
  display.drawPixel(*x_pos + 13, 6, SSD1306_WHITE);
  display.drawPixel(*x_pos + 13, 5, SSD1306_WHITE);
  display.drawPixel(*x_pos + 13, 4, SSD1306_WHITE);
  display.drawPixel(*x_pos + 13, 3, SSD1306_WHITE);
  //display.display();
  //delay(1);
  display.drawPixel(*x_pos + 14, 2, SSD1306_WHITE);
  display.drawPixel(*x_pos + 14, 1, SSD1306_WHITE);
  display.drawPixel(*x_pos + 14, 0, SSD1306_WHITE);
  display.drawPixel(*x_pos + 14, 0, SSD1306_WHITE);
  //display.display();
  //delay(1);
  /******************************************/
  display.drawPixel(*x_pos + 15, 0, SSD1306_WHITE);
  display.drawPixel(*x_pos + 15, 1, SSD1306_WHITE);
  display.drawPixel(*x_pos + 15, 2, SSD1306_WHITE);
  display.drawPixel(*x_pos + 15, 3, SSD1306_WHITE);
  //display.display();
  //delay(1);
  display.drawPixel(*x_pos + 15, 4, SSD1306_WHITE);
  display.drawPixel(*x_pos + 15, 5, SSD1306_WHITE);
  display.drawPixel(*x_pos + 16, 6, SSD1306_WHITE);
  display.drawPixel(*x_pos + 16, 7, SSD1306_WHITE);
  //display.display();
  //delay(1);
  display.drawPixel(*x_pos + 16, 8, SSD1306_WHITE);
  display.drawPixel(*x_pos + 16, 9, SSD1306_WHITE);
  display.drawPixel(*x_pos + 16, 10, SSD1306_WHITE);
  display.drawPixel(*x_pos + 16, 11, SSD1306_WHITE);
  //display.display();
  //delay(1);
  display.drawPixel(*x_pos + 17, 12, SSD1306_WHITE);
  display.drawPixel(*x_pos + 17, 13, SSD1306_WHITE);
  display.drawPixel(*x_pos + 17, 14, SSD1306_WHITE);
  display.drawPixel(*x_pos + 17, 15, SSD1306_WHITE);
  //display.display();
  //delay(1);
  display.drawPixel(*x_pos + 18, 15, SSD1306_WHITE);
  display.drawPixel(*x_pos + 18, 14, SSD1306_WHITE);
  display.drawPixel(*x_pos + 18, 13, SSD1306_WHITE);
  display.drawPixel(*x_pos + 18, 12, SSD1306_WHITE);
  //display.display();
  //delay(1);
  display.drawPixel(*x_pos + 19, 11, SSD1306_WHITE);
  display.drawPixel(*x_pos + 19, 10, SSD1306_WHITE);
  display.drawPixel(*x_pos + 19, 9, SSD1306_WHITE);
  display.drawPixel(*x_pos + 19, 8, SSD1306_WHITE);
  //display.display();
  //delay(1);
  /****************************************************/
  display.drawPixel(*x_pos + 20, 8, SSD1306_WHITE);
  display.drawPixel(*x_pos + 21, 8, SSD1306_WHITE);
  //display.display();
  //delay(1);
  /****************************************************/
  display.drawPixel(*x_pos + 22, 7, SSD1306_WHITE);
  display.drawPixel(*x_pos + 23, 6, SSD1306_WHITE);
  display.drawPixel(*x_pos + 24, 6, SSD1306_WHITE);
  display.drawPixel(*x_pos + 25, 7, SSD1306_WHITE);
  //display.display();
  //delay(1);
  /************************************************/
  display.drawPixel(*x_pos + 26, 8, SSD1306_WHITE);
  display.drawPixel(*x_pos + 27, 8, SSD1306_WHITE);
  display.drawPixel(*x_pos + 28, 8, SSD1306_WHITE);
  display.drawPixel(*x_pos + 29, 8, SSD1306_WHITE);
  display.drawPixel(*x_pos + 30, 8, SSD1306_WHITE); // -----
  *x_pos = *x_pos + 30;
  display.display();
  delay(1);
}




void refresh(){
  display.display();
  delay(100);
  display.clearDisplay();
}

// :::::::::::: RTCMODULE  ::::::::::::
void rtcModule()
{
  DateTime now = rtc.now(); 
  //Display Text - Dia da semana
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print(daysOfTheWeek[now.dayOfTheWeek()]);

  //Display Text - Dia do ano
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(77, 0);
  display.print(now.day(), DEC);

  //Display Text - Mês 
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(87, 0);
  display.print("/");
  display.print(now.month(), DEC);

  //Display Text - Ano
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(97, 0);
  display.print("/");
  display.print(now.year(), DEC);

  //Display Text - Hora
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(17, 17);
  display.print(now.hour(), DEC);

  //Display Text - Minutos
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(43, 17);
  display.print(":");
  display.print(now.minute(), DEC);

 //Display Text - Segundos
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(80, 17);
  display.print(":");
  display.print(now.second(), DEC);

  //exibe no display
  //display.display();

  //limpa o display
  //display.clearDisplay();
}


void relogio_main(){
    int touch = digitalRead(button);
    if(touch == HIGH)
    {
    count = count + 1;
    }

    switch(count){
        case 0: 
            getbpm_spO2();
        break;
        case 1:
            rtcModule();
            refresh();
        break;
        default:
            count = 0;
            getbpm_spO2();
        break;
    }
}
