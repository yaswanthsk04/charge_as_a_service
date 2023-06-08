#include <SPI.h>
#include <mcp_can.h>
MCP_CAN CAN(10);
String request="";
char buf[10];
bool start=false;
bool stp=!start;
int s=0;
char soc[6];
unsigned char len=0;
int analogInput = A0;
float vout = 0.0;
float vin = 0.0;
float R1 = 30000.0;  
float R2 = 7500.0;
int value = 0;
String x;
void setup(){
  pinMode(8,OUTPUT);
  digitalWrite(8,LOW);
  pinMode(analogInput, INPUT);
  Serial.begin(115200);
  SPI.begin();
  START_INIT:
    if(CAN_OK == CAN.begin(CAN_500KBPS))      //setting CAN baud rate to 500Kbps
    {
        Serial.println("CAN BUS Shield init ok!");
    }
    else
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println("Init CAN BUS Shield again");
        delay(100);
        goto START_INIT;
    }}
void loop() {
  if(CAN_MSGAVAIL == CAN.checkReceive())
    {
        CAN.readMsgBuf(&len, buf); 
       request=(String)buf;
    Serial.println("request:");
    Serial.println(request);
    if(request=="SOC"){
      if(start){
        delay(5000);
        digitalWrite(8,LOW);
        delay(4000);
       value = analogRead(analogInput);
        vout = (value * 5.0) / 1024.0;
        vin = vout / (R2/(R1+R2));
        digitalWrite(8,HIGH);
    }
    else{
      value = analogRead(analogInput);
        vout= (value * 5.0) / 1024.0;
        vin= vout / (R2/(R1+R2));
    }
        Serial.print("BATTERY VOLTAGE:");
        Serial.println(vin);
        x=(String)((float)((vin-2)*10));
        Serial.print("BATTERY CHARGE:");
        Serial.println(x);
        x.toCharArray(soc,8);  
    CAN.sendMsgBuf(0x09,0,8,soc);
    }
    if(request=="START"){
      unsigned char started[]="STARTED";
     CAN.sendMsgBuf(0x07, 0, 7, started);
     delay(200);
     s=2;
     start=true;
     stp=false;
     digitalWrite(8,HIGH);
   }
    if(request=="STOP"){
     unsigned char stopped[]="STOPPED";
     CAN.sendMsgBuf(0x05, 0, 7, stopped);
     delay(200);
     stp=true;
     start=false;
     digitalWrite(8,LOW);
    }
   }
   request="";
    memset(buf,0,sizeof(buf));
    memset(soc,0,sizeof(soc));
    }
