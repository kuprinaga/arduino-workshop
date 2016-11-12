int val = LOW;
int buttonId = 5;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT); 
  pinMode(buttonId, INPUT_PULLUP); 
}

void loop() {
  // put your main code here, to run repeatedly:
  val = digitalRead(buttonId);
  digitalWrite(LED_BUILTIN, val);
}
