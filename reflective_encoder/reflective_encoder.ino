const int encoderPinA = 2;  // Encoder output A connected to Arduino pin 2
const int encoderPinB = 3;  // Encoder output B connected to Arduino pin 3

volatile long encoderCount = 0;   // Current encoder count
volatile int encoderLastA = LOW;  // Previous state of encoder output A

void setup() {
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  digitalWrite(encoderPinA, HIGH);
  digitalWrite(encoderPinB, HIGH);
  attachInterrupt(digitalPinToInterrupt(encoderPinA), updateEncoder, CHANGE);  // Attach interrupt to encoder pin A
  Serial.begin(9600);
}

void loop() {
  long count = encoderCount;
  Serial.println(count);
  delay(100);
}

void updateEncoder() {
  int encoderA = digitalRead(encoderPinA);
  int encoderB = digitalRead(encoderPinB);

  if (encoderA != encoderLastA) {  // Check if encoder output A has changed
    if (encoderB != encoderA) {    // Check if encoder output B is different from encoder output A
      encoderCount++;              // Increment encoder count for clockwise rotation
    } else {
      encoderCount--;  // Decrement encoder count for counter-clockwise rotation
    }
  }
  encoderLastA = encoderA;  // Update the previous state of encoder output A
}