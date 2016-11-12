int m=64, c = 0, r = 0, s = 1;

int buttonId = 5;

void setup() {
  // put your setup code here, to run once:
  pinMode(buttonId, INPUT_PULLUP); 
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  r = digitalRead(buttonId);
  c += r;
  c = c > m ? m : c;
  if(c == m && r == 0)
  {
    s ^= 1;
    c = 0;
    digitalWrite(LED_BUILTIN, s);
    Serial.println(millis());
  }
}
