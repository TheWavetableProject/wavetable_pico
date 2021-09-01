// proof-of-concept arduino sketch for The Wavetable Project
const int OUTPUT_PIN = 13;
const int DELAY_MICROSECONDS = 200; 

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(OUTPUT_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(OUTPUT_PIN, HIGH); 
  digitalWrite(LED_BUILTIN, HIGH);
  delayMicroseconds(DELAY_MICROSECONDS);  
  digitalWrite(OUTPUT_PIN, LOW);
  digitalWrite(LED_BUILTIN, LOW);
  delayMicroseconds(DELAY_MICROSECONDS);
}
