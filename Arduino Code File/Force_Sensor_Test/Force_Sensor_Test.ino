//Variables    
int forcePin = A0;
int forceReading;
 
void setup(void) 
{
  Serial.begin(115200);   // We'll send debugging information via the Serial monitor
}
 
void loop(void) 
{
  forceReading = analogRead(forcePin);
  Serial.print("Analog reading = ");
  Serial.println(forceReading);
 
  delay(500);
}
