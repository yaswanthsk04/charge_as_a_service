#include <Wire.h>
#include <SPI.h>
#include <mcp_can.h>
#include <MFRC522.h>
constexpr uint8_t RST_PIN = 9;
constexpr uint8_t SS_PIN = 8; 
MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
MCP_CAN CAN(10);  
byte nuidPICC[4];   
String req_soc="SOC",req_st="START",req_end="STOP",mod="",request,State="",Stopped="",Charge="",ftid="id",id="",na="NA";
char state[10],charge[10],stopped[10];
bool avl=false;
unsigned char len=0;
int req=0;
void setup() {
  Wire.begin(0x8);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requireEvent);
  Serial.begin(115200);
 SPI.begin();
  rfid.PCD_Init();
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  START_INIT:
    if(CAN_OK == CAN.begin(CAN_500KBPS))
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
  delay(100);
}
String ret(int rqst){
    if(rqst==2){
      if(Charge==""){
  char soc[]="SOC";
  CAN.sendMsgBuf(0x10,0,3,soc);
  delay(11000);
  if(CAN_MSGAVAIL == CAN.checkReceive()) 
    {
        CAN.readMsgBuf(&len, charge);    
    }
    return(charge);
    }}
    if(rqst==3){
      if(State==""){
  char start[]="START";
  CAN.sendMsgBuf(0x8,0,5,start);
  delay(1000);
  if(CAN_MSGAVAIL == CAN.checkReceive()) 
    {
        CAN.readMsgBuf(&len, state);    
    }
    return(state);
    }}
    if(rqst==4){
      if(Stopped==""){
  char stp[]="STOP";
  CAN.sendMsgBuf(0x06,0,4,stp);
  delay(1000);
  if(CAN_MSGAVAIL == CAN.checkReceive()) 
    {
        CAN.readMsgBuf(&len, stopped);    
    }
    return(stopped);
    }}}
void receiveEvent(int howMany) {
  while (Wire.available()) { // loop through all but the last
       request+=char(Wire.read());
  }
  if(request==ftid){
      if (rfid.PICC_IsNewCardPresent())
  {  
  if (rfid.PICC_ReadCardSerial())
   { 
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
      id+=nuidPICC[i];
    }}
    Serial.println(id);
    avl=true;    
    }
    else{
      avl=false;
    }
    req=1;
    }
    if(request==req_soc){
   Serial.print("REQUEST FROM PI:");
   Serial.println(request);
    req=2;
    Charge=(String)ret(req);
    Serial.print("Received from canNode:");
    Serial.println(charge);
    }
    if(request==req_st){
   Serial.print("REQUEST FROM PI:");
   Serial.println(request);
    req=3;
    State=(String)ret(req);
    Serial.print("Received from canNode:");
    Serial.println(state);
    }
    if(request==req_end){
   Serial.print("REQUEST FROM PI:");
   Serial.println(request);
    req=4;
    Stopped=(String)ret(req);
    Serial.print("Received from canNode:");
    Serial.println(stopped);
    }}    
void requireEvent(){
  if(req==1){
    if(avl){
     char fid[12];
     id.toCharArray(fid,12);
     Wire.write(fid); 
    }
    if(!avl){
      char NA[3];
      na.toCharArray(NA,3);
      Serial.println(NA);
      Wire.write(NA);
    }}
  if(req==2){
  char ch[6];
  Charge.toCharArray(ch,6);   
  Wire.write(ch);
  }
  if(req==3){
  char ch[8];
  State.toCharArray(ch,8);   
  Wire.write(ch);
  }
  if(req==4){
  char st[8];
  Stopped.toCharArray(st,8);   
  Wire.write(st);
  }
  Charge="";
  State="";
  req="";
  Stopped="";
  request="";
  id="";
  }
