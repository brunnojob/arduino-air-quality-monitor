#include <Arduino.h>

constexpr uint8_t sensorPin=35, ledPin=2;
constexpr int alertLevel=1800;
constexpr uint8_t windowSize=8;
int readings[windowSize]={0}; uint8_t cursor=0, count=0; long sum=0;

void setup() {
  pinMode(ledPin,OUTPUT); Serial.begin(115200);
  for(auto &v:readings)v=analogRead(sensorPin);
  sum=0;
}
void loop() {
  int raw=analogRead(sensorPin);
  sum-=readings[cursor]; readings[cursor]=raw; sum+=raw;
  cursor=(cursor+1)%windowSize; if(count<windowSize)count++;
  int avg=sum/count; bool alert=avg>=alertLevel;
  digitalWrite(ledPin,alert?HIGH:LOW);
  Serial.printf("{\"adc\":%d,\"average\":%d,\"alert\":%s}\n",raw,avg,alert?"true":"false");
  delay(1000);
}