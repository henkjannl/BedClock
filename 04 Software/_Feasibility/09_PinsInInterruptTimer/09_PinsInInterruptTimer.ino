/*
 Repeat timer example

 This example shows how to use hardware timer in ESP32. The timer calls onTimer
 function every second. The timer can be stopped with button attached to PIN 0
 (IO0).

 This example code is in the public domain.
 */


hw_timer_t * timer = NULL;
volatile SemaphoreHandle_t timerSemaphore;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

volatile uint32_t isrCounter = 0;
volatile uint32_t lastval = 0;
volatile uint32_t lastIsrAt = 0;
volatile bool touch1[3];

void IRAM_ATTR onTimer(){

  // Increment the counter and set the time of ISR
  portENTER_CRITICAL_ISR(&timerMux);

  lastval=touchRead(T5);
  touch1[0]=touch1[1];
  touch1[1]=touch1[2];  
  touch1[2]=(lastval<50);

  if( !touch1[0] && touch1[1] && touch1[2] ) {
    isrCounter++;
    lastIsrAt = millis();
  }
  portEXIT_CRITICAL_ISR(&timerMux);

  // Give a semaphore that we can check in the loop
  xSemaphoreGiveFromISR(timerSemaphore, NULL);
  // It is safe to use digitalRead/Write here if you want to toggle an output
}

void setup() {
  Serial.begin(115200);

  // Create semaphore to inform us when the timer has fired
  timerSemaphore = xSemaphoreCreateBinary();

  // Use 1st timer of 4 (counted from zero).
  // Set 80 divider for prescaler (see ESP32 Technical Reference Manual for more
  // info).
  timer = timerBegin(0, 80, true);

  // Attach onTimer function to our timer.
  timerAttachInterrupt(timer, &onTimer, true);

  // Set alarm to call onTimer function every millisecond (value in microseconds).
  // Repeat the alarm (third parameter)
  timerAlarmWrite(timer, 10000, true);

  // Start an alarm
  timerAlarmEnable(timer);
}

void loop() {
    portENTER_CRITICAL(&timerMux);
    uint32_t isrCount = isrCounter;
    uint32_t isrTime = lastIsrAt;
    portEXIT_CRITICAL(&timerMux);

    Serial.print("onTimer no. ");
    Serial.print(isrCount);
    Serial.print(" touch value ");
    Serial.print(lastval);
    Serial.print(touch1[0]);
    Serial.print(touch1[1]);
    Serial.print(touch1[2]);
    Serial.print(" at ");
    Serial.print(isrTime);
    Serial.println(" ms");

    delay(500);
}
