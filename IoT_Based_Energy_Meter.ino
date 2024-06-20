#include <ModbusMaster.h>


float V=0;
float A=0;
float W=0;
float Wh=0;
float PF=0;
float F=0;

/*!
  We're using a MAX485-compatible RS485 Transceiver.
  Rx/Tx is hooked up to the hardware serial port at 'Serial'.
  The Data Enable and Receiver Enable pins are hooked up as follows:
*/

#define MAX485_DE      3
#define MAX485_RE_NEG  2

ModbusMaster node;

void preTransmission()
{
  digitalWrite(MAX485_RE_NEG, 1);
  digitalWrite(MAX485_DE, 1);
}

void postTransmission()
{
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
}

void setup()
{
  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  // Init in receive mode
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
bool state = true;

  Serial.begin(9600);

  node.begin(1, Serial);
  
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
}

float reform_uint16_2_float32(uint16_t u1, uint16_t u2)
{  
  uint32_t num = ((uint32_t)u1 & 0xFFFF) << 16 | ((uint32_t)u2 & 0xFFFF);
    float numf;
    memcpy(&numf, &num, 4);
    return numf;
}

float getRTU(uint16_t m_startAddress){
  uint8_t m_length =2;
  uint16_t result;
  float x;

  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
  result = node.readInputRegisters(m_startAddress, m_length);
  if (result == node.ku8MBSuccess) {
     return reform_uint16_2_float32(node.getResponseBuffer(0),node.getResponseBuffer(1));
  }
  delay(500);
}


void loop()
{
  delay(500);
  V = getRTU(0x0015);
  Serial.println("Voltage : "+ String(V,2));
  delay(1000);
  A = getRTU(0x0017);
  Serial.println("Current : " + String(A,2));
  delay(500);
  W = getRTU(0x000F);
  Serial.println("Active Power : " + String(W,2));
  delay(500);
  Wh = getRTU(0x0001);
  Serial.println("Total Active Energy : " + String(Wh,2));
  delay(500);
  PF = getRTU(0x0019);
  Serial.println("Power Factor : " + String(PF,2));
  delay(500);
  F = getRTU(0x001B);
  Serial.println("Frequency : " + String(F,2));
  delay(1000);
  Serial.println("==================================");
}

