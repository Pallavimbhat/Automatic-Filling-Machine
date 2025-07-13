// --- Pin Configuration ---
#define limit_s A0       // Limit switch input
#define relayf 10        // Forward relay output (moves bottles forward)
#define relayb 9         // Backward relay output (resets position)

#include <Servo.h>
Servo s;                // Servo for opening/closing the funnel

void setup() 
{
  Serial.begin(9600);                       // Start serial monitor at 9600 baud
  pinMode(limit_s, INPUT_PULLUP);          // Limit switch with pull-up
  pinMode(relayf, OUTPUT);                 // Forward relay pin
  pinMode(relayb, OUTPUT);                 // Backward relay pin

  s.attach(5);                             // Attach servo to pin 5
  s.write(0);                              // Keep servo closed initially

  Serial.println("System Initialized. Waiting for bottle...");
}

void loop() 
{
  limitday(); // Continuously check for bottle and start process
}

void limitday()
{
  int limit_state = digitalRead(limit_s); // Read limit switch
  Serial.print("Limit Switch State: ");
  Serial.println(limit_state);

  if (limit_state == 0) // Bottle detected
  {
    Serial.println("Bottle Detected. Starting filling process...");
    delay(200); // Debounce delay

    // --- Bottle 1 ---
    moveForward(600);
    fillBottle(1);

    // --- Bottle 2 ---
    moveForward(2250);
    fillBottle(2);

    // --- Bottle 3 ---
    moveForward(2300);
    fillBottle(3);

    // --- Reset ---
    Serial.println("Resetting to initial position...");
    digitalWrite(relayb, HIGH);
    delay(6000); // Move backward fully
    digitalWrite(relayb, LOW);

    Serial.println("Cycle Complete. Waiting for next bottle...");
  }
  else
  {
    // Idle state
    s.write(0); // Keep funnel closed
    digitalWrite(relayf, LOW);
    digitalWrite(relayb, LOW);
  }
}

void moveForward(int duration)
{
  Serial.print("Moving forward for ");
  Serial.print(duration);
  Serial.println(" ms");
  digitalWrite(relayf, HIGH);
  delay(duration);
  digitalWrite(relayf, LOW);
}

void fillBottle(int number)
{
  Serial.print("Filling Bottle ");
  Serial.println(number);

  s.write(60);    // Open funnel
  delay(1500);    // Wait to pour liquid
  s.write(0);     // Close funnel
  delay(3000);    // Wait before next bottle
}
