#include <string.h>
#define IBUS_BUFFSIZE 32    // Max iBus packet size (2 byte header, 14 channels x 2 bytes, 2 byte checksum)
#define IBUS_MAXCHANNELS 10 // My TX only has 10 channels, no point in polling the rest
int Channel_1;
int Channel_2;
int Channel_3;
int Channel_4;
int Pot_5;
int Pot_6;
int Switch_A;
int Switch_B;
int Switch_C;
int Switch_D;
bool IsFlying = false;
bool Condition1 = false;
bool Condition2 = false;
static uint8_t ibusIndex = 0;
static uint8_t ibus[IBUS_BUFFSIZE] = {0};
static uint16_t rcValue[IBUS_MAXCHANNELS];
static boolean rxFrameDone;



#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
const char* ssid      = "TELLO-5EA8AA";
const char* password  = "";
IPAddress TelloIp(192, 168, 10, 1);
const int TelloPort = 8889;      // local port to listen on
WiFiUDP Udp;
const int localUdpPort = 9002;  // local port to listen on
const char  cmdCommandPacekt[] = "command"; // command
const char  cmdTakeoffPacekt[] = "takeoff"; // takeoff
const char  cmdLandPacekt[] = "land";       // land
char  cmdRcPacekt[128];
const char  cmdFlipForwardPacekt[] = "flip f";
char  cmdSpeedPacekt[128];

void setup() 
{
  Serial.begin(115200); 
   Serial.println();
  Serial.println("setup done!");




  
{
  /////////////////////////////////////Serial.begin(115200);
  
//----------------------Connecting to Tello------------------

  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  { 
    delay(500);
    Serial.print(".");
  } 
  Serial.println(" connected");
  
//----------------------Enter SDK Mode-----------------------

  Udp.beginPacket(TelloIp, TelloPort);
  Udp.write(cmdCommandPacekt);
  Udp.endPacket();  
  delay(100);
  Udp.begin(localUdpPort);
  Serial.printf ("Now listening at IP %s, UDP port %d\n", 
                  WiFi.localIP().toString().c_str(), localUdpPort);
//-----------------------------------------------------------
}
}

void loop() 
{
  {
  rxFrameDone = false;
  
  if (Serial.available())
  {
    uint8_t val = Serial.read();
    // Look for 0x2040 as start of packet
    if (ibusIndex == 0 && val != 0x20)
    {
      ibusIndex = 0;
      return;
    }
    if (ibusIndex == 1 && val != 0x40) 
    {
      ibusIndex = 0;
      return;
    }

    if (ibusIndex == IBUS_BUFFSIZE)
    {
      ibusIndex = 0;
      int high=3;
      int low=2;
      for(int i=0;i<IBUS_MAXCHANNELS; i++)
      {
        //left shift away the first 8 bits of the first byte and add the whole value of the previous one
        rcValue[i] = (ibus[high] << 8) + ibus[low];
//    Serial.print(rcValue[i]);    //values between 1000 and 2000
//    Serial.print("     ");
        high += 2;
        low += 2;
      }
  Switch_A = map(rcValue[6], 1000, 2000, 0, 1); 
  Switch_B = map(rcValue[7], 1000, 2000, 0, 1); 
  Switch_C = map(rcValue[8], 1000, 2000, 0, 1); 
  Switch_D = map(rcValue[9], 1000, 2000, 0, 1);
  Pot_5 = map(rcValue[4], 1000, 2000, 10, 100);
  sprintf(cmdSpeedPacekt,"speed %d", Pot_5);
 Serial.println(cmdSpeedPacekt);
  Udp.beginPacket(TelloIp, TelloPort);
  Udp.write(cmdSpeedPacekt);
  Udp.endPacket();
//    Serial.print(Channel_3);
//    Serial.println();
      rxFrameDone = true;
      return;
    }
    else
    {
      ibus[ibusIndex] = val;
      ibusIndex++;
    }
  }
}  
  
  if (Switch_A==1 && IsFlying==false) // To _take_off
      {
          Serial.println("TELLO_Takeoff");
          Udp.beginPacket(TelloIp, TelloPort);
          Udp.write(cmdTakeoffPacekt);
          Udp.endPacket();
          IsFlying=true;
      }
    
   if  (Switch_A==0 && IsFlying==true)  // To_Land
      {
         Serial.println("TELLO_Landing");
         Udp.beginPacket(TelloIp, TelloPort);
         Udp.write(cmdLandPacekt);
         Udp.endPacket();
         IsFlying=false;
      }
 /*   
   if  (Switch_B==1 && Condition1==false)  // Fliptest
      {
         Serial.println("fliping_forward");
         Udp.beginPacket(TelloIp, TelloPort);
         Udp.write(cmdFlipForwardPacekt);
         Udp.endPacket();
      }
  Condition1=true;
        
*/
  
   if (Switch_D==1) // To_Control
      {
{
  rxFrameDone = false;
  
  if (Serial.available())
  {
    uint8_t val = Serial.read();
    // Look for 0x2040 as start of packet
    if (ibusIndex == 0 && val != 0x20)
    {
      ibusIndex = 0;
      return;
    }
    if (ibusIndex == 1 && val != 0x40) 
    {
      ibusIndex = 0;
      return;
    }

    if (ibusIndex == IBUS_BUFFSIZE)
    {
      ibusIndex = 0;
      int high=3;
      int low=2;
      for(int i=0;i<IBUS_MAXCHANNELS; i++)
      {
        //left shift away the first 8 bits of the first byte and add the whole value of the previous one
        rcValue[i] = (ibus[high] << 8) + ibus[low];
//    Serial.print(rcValue[i]);    //values between 1000 and 2000
//    Serial.print("     ");
        high += 2;
        low += 2;
      }
      
  Channel_1 = map(rcValue[0], 1000, 2000, -100, 100); 
  Channel_2 = map(rcValue[1], 1000, 2000, -100, 100);
  Channel_3 = map(rcValue[2], 1000, 2000, -100, 100);
  Channel_4 = map(rcValue[3], 1000, 2000, -100, 100);
  Pot_5 = map(rcValue[4], 1000, 2000, 0, 100);
  Pot_6 = map(rcValue[5], 1000, 2000, 0, 100);
  sprintf(cmdRcPacekt,"rc %d %d %d %d", Channel_1,Channel_2,Channel_3,Channel_4);
  //  snprintf(cmdRcPacekt, sizeof cmdRcPacekt, "rc %d %d %d %d", Channel_1,Channel_2,Channel_3,Channel_4);
         Serial.print("         ");
         Serial.print(cmdRcPacekt);
         Udp.beginPacket(TelloIp, TelloPort);
         Udp.write(cmdRcPacekt);
         Udp.endPacket();
//    Serial.print(Channel_3);
//    Serial.println();
      rxFrameDone = true;
      return;
    }
    else
    {
      ibus[ibusIndex] = val;
      ibusIndex++;
    }
  }
}
    // Read_Stick_Pot();
    }   
}



//void Read_Rx_Switches()


//void Read_Stick_Pot()
