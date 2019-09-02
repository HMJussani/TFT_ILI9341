#include <Adafruit_GFX.h>   
#include <Adafruit_TFTLCD.h>
#include <Adafruit_Sensor.h>
#include <dht.h> 
 
dht DHT; // Cria um objeto da classe dht

#include "graphics.c"
#include "RTClib.h"

RTC_DS1307 rtc;
uint32_t timer = 0;
uint32_t timer1 = 0;

#define LCD_CS A0 // Chip Select goes to Analog 3
#define LCD_CD A1 // Command/Data goes to Analog 2
#define LCD_WR A2 // LCD Write goes to Analog 1
#define LCD_RD A3 // LCD Read goes to Analog 0
#define LCD_RESET 13 // Can alternately just connect to Arduino's reset pin

// Assign human-readable names to some common 16-bit color values:
#define preto      0xFFFF
#define amarelo    0x001F 
#define amarelo_1  0x00f0
#define larannja   0x0bff
#define azul       0xFFE0
#define vermelho   0x07FF    
#define verde      0xF81F
#define turquesa   0xF800
#define lilas      0x07E0
#define branco     0x0000
#define cinza      0x8410

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
uint16_t identifier = 0x9341;
uint16_t largMax;  //width();
uint16_t altMax; //height();
int raio = 10;
String umidade = "00,0", temperatura= "00,0", temperaturaMin= "00,0", temperaturaMax= "00,0";
float tempMin=0, tempMax=0, umiMax=0, umiMin=0;

void setup() {
   Serial.begin(9600);    
   rtc.begin();
   //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
   Serial.println(mostrarCompilacao()); 
   tft.reset();
   tft.begin(identifier);
   tft.fillScreen(branco);
   largMax = tft.width();
   altMax = tft.height();
   tft.setRotation(2);
   iniTemp();
   tela_0();
   
}

void loop() {
  if(millis() - timer1>= 1000) //tempo 1seg
  {
   mostraHora(25,20);
    timer1 = millis();
  }
  if(millis() - timer>= 5000) //tempo 5seg
  {
   mostraTemp(80,120); 
   mostraTempMinMax(55, 180, 150,180);//xy min, xy max
   mostraUmiMinMax(80,290,170,290); //xy min, xy max
   mostraUmi(80, 220);
   getValues();
   timer = millis(); // Atualiza a referência
  }              
}

void iniTemp(){
  DHT.read22(11);
   tempMin = DHT.temperature;
   umiMin = DHT.humidity;
   tempMax = tempMin; 
   temperatura =String(tempMin, 1);
   temperaturaMin =String(tempMin, 1);
   temperaturaMax =String(tempMin, 1);
   umidade =String(umiMin, 1) ; 
   umiMax = umiMin;
}

void mostraHora(int16_t x, int16_t y){  
  DateTime now = rtc.now();
   char buf1[] = "hh:mm:ss";
   tft.fillRoundRect(x-1,y-1,200,50,raio, cinza);
   texto(x,y,branco,4,(now.toString(buf1)));
   char buf3[] = "DDD, DD/MM/YYYY.";
   tft.fillRoundRect(x-1,y+49,200,30,raio, cinza);
   texto(x,y+50,branco,2,(now.toString(buf3)));
  }
void getValues(){  
    DHT.read22(11); // chama método de leitura da classe dht,
                   // com o pino de transmissão de dados ligado no pino 11
    temperatura =String(DHT.temperature, 1);
    umidade =String(DHT.humidity, 1) ; 
   if(tempMin>DHT.temperature){             
           tempMin=DHT.temperature; 
           temperaturaMin =String(tempMin, 1);
           }
   if(tempMax<DHT.temperature){ 
      tempMax=DHT.temperature;
      temperaturaMax =String(tempMax, 1);
       }
   if(umiMin>DHT.humidity){             
           umiMin=DHT.humidity;            
           }
   if(umiMax<DHT.humidity){ 
      umiMax=DHT.humidity;      
       }
}

void mostraTemp(int16_t x, int16_t y){ 
   tft.fillRoundRect(x-1,y-1,200,40,raio, cinza);
   texto(x,y,verde,4,temperatura);
   texto(x+100,y,verde,4,"C");    
    }
    
void mostraTempMinMax(int16_t xMin, int16_t yMin,int16_t xMax, int16_t yMax){   
   tft.fillRoundRect(xMin-1,yMin-1,200,30,raio, cinza);
   texto(xMin,yMin,azul,2,temperaturaMin);
   texto(xMin+50,yMin,azul,2,"C"); 

   tft.fillRoundRect(xMax-1,yMax-1,200,30,raio, cinza);
   texto(xMax,yMax,vermelho,2,temperaturaMax);
   texto(xMax+50,yMax,vermelho,2,"C");  
    }
void mostraUmi(int16_t x, int16_t y){ 
   tft.fillRoundRect(x,y,200,40,raio, cinza);
   texto(x,y,verde,4,umidade);
   texto(x+100,y,verde,4,"%");    
}

void mostraUmiMinMax(int16_t xMin, int16_t yMin,int16_t xMax, int16_t yMax){   
   tft.fillRoundRect(xMin-1,yMin-1,200,30,raio, cinza);
   texto(xMin,yMin,azul,2,String(umiMin, 1));
   texto(xMin+50,yMin,azul,2,"%"); 

   tft.fillRoundRect(xMax-1,yMax-1,200,30,raio, cinza);
   texto(xMax,yMax,vermelho,2,String(umiMax, 1));
   texto(xMax+50,yMax,vermelho,2,"%");  
    }
void tela_1(){
   tft.drawRoundRect(1,1,largMax,altMax,raio,preto);
   tft.fillRoundRect(2,2,largMax-2,altMax-2,raio, branco);
   retangulo(20,10,200,100,preto);
   retangulo(20,130,200,100,amarelo);
  // icone(13,245,preto); //icone rodapé
   icone(86,245,preto); //icone rodapé
   //icone(159,245,preto); //icone rodapé
     }

void tela_0(){
  tft.drawRoundRect(1,1,largMax,altMax,raio,preto);
  tft.fillRoundRect(2,2,largMax-2,altMax-2,raio, cinza);
  tft.drawFastHLine(0, 106, tft.width(), preto);
  texto(60,160,azul,2,"Min");
  texto(160,160,vermelho,2,"Max");
  tft.drawBitmap(15,120,termo,33,80,branco);
  tft.drawFastHLine(0, 212, tft.width(), preto); 
  texto(85,270,azul,2,"Min");
  texto(180,270,vermelho,2,"Max");
  tft.drawBitmap(15,220,umi,60,79,branco);         
 }
 
void retangulo(int16_t x0, int16_t y0,int16_t larg, int16_t alt,uint16_t cor){
  tft.drawRoundRect(x0,y0,larg,alt,raio,preto);
  tft.fillRoundRect(x0+1,y0+1,larg-2,alt-2,raio, branco);
  tft.drawRoundRect(x0+20,y0+30,80,50,raio,preto);  
  tft.fillRoundRect(x0+21,y0+31,78,48,raio, verde);
  tft.drawRoundRect(x0+110,y0+5,86,87,raio,preto);
  texto(x0+45,y0+42,branco,3,"ON");  
  tft.fillRoundRect(x0+111,y0+6,84,85,raio, branco);
  tft.drawBitmap(x0+112,y0+7,lamp,87,84,cor);
}

void icone(int16_t x0, int16_t y0,uint16_t cor){
  tft.drawRoundRect(x0,y0,61,61,raio,preto);
  tft.fillRoundRect(x0+1,y0+1,59,59,raio, branco);
  tft.drawBitmap(x0+2,y0+2,configIcon,60,60,cor);
}
void texto(int16_t x, int16_t y,uint16_t cor, int16_t tam, String texto){
  tft.setCursor(x, y);
  tft.setTextColor(cor); 
  tft.setTextSize(tam);
  tft.println(texto);
}

String mostrarCompilacao(){
   String compilado = "Compilado em: ";
   compilado += F(__DATE__);
   compilado += " -- ";
   compilado += F(__TIME__);
   return compilado;
}

     
