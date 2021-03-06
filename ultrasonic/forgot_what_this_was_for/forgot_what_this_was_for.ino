#define trigR 13
#define echoR 12
#define trigL 11
#define echoL 10

#define SWIPE_L 7
#define SWIPE_R 6

#define DELAY 250


long rangeR, rangeL;
float alpha = 0.0001;

String dup = "";

void setup() {
  Serial.begin(9600);
  Serial.println("calibrating..");
  pinMode(SWIPE_R, OUTPUT);
  pinMode(SWIPE_L, OUTPUT);  
  
  
  ultra_setup();
  
  
  Serial.println("calibration complete.");
}
  
void loop() {
  int swipe = readSwipe();
  printdup(String(swipe));
  if (swipe > 0) {
    digitalWrite(swipe, HIGH);
  } else {
    digitalWrite(SWIPE_L, LOW);
    digitalWrite(SWIPE_R, LOW);
  }    

}

/** Returns:
 *    -1	if   no  swipe 
 *  SWIPE_R	if right swipe
 *  SWIPE_L	if  left swipe
 */
int readSwipe() {
  boolean detectR, detectL;
  long start, duration;
  
  long right = readDuration(SWIPE_R); 
  long left  = readDuration(SWIPE_L); 
  detectR =  right < rangeR;
  detectL =  left < rangeL;
  
  printdup("Right: \t" + String(right) + "\tLeft:\t" + String(left));
  
  start = millis();
  
  if (detectR && !detectL) {
    while(detectR || detectL) {
      detectR =  readDuration(SWIPE_R) < rangeR;
      detectL =  readDuration(SWIPE_L) < rangeL;      
    }
    duration = millis() - start;
    if (duration < DELAY) {
      delay(duration);
    }
    return SWIPE_R;
  } else if (!detectR && detectL) {
    while(detectR && detectL) {
      detectR =  readDuration(SWIPE_R) < rangeR;
      detectL =  readDuration(SWIPE_L) < rangeL;      
    }
    duration = millis() - start;
    if (duration < DELAY) {
      delay(duration);
    }
    return SWIPE_L;  
  }
  
  return -1;
}

/* Returns the reading from the @side sensor. */
long readDuration(int side) {
  int trig, echo;
  if (side == SWIPE_L) {
    trig = trigL;
    echo = echoL;
  } else if (side == SWIPE_R) {
    trig = trigR;
    echo = echoR;
  }
  digitalWrite(trig, LOW);
  delayMicroseconds(5);    
  digitalWrite(trig, HIGH);  
  delayMicroseconds(10);  
  return pulseIn(echo, HIGH);
}

/* Setup IO pins for each of the sensors. */
void ultra_setup() {
  pinMode(trigR, OUTPUT);
  pinMode(echoR, INPUT);
  pinMode(trigL, OUTPUT);
  pinMode(echoL, INPUT);

 // calibrate();
}

/* Sample the average reading for maximum sensor range. */
void calibrate() {
  rangeR = readDuration(SWIPE_R);
  rangeL = readDuration(SWIPE_L);
  for (int i = 0; i < 5; i++) {
    Serial.println(i);
    if (i % 2 == 0) {
      digitalWrite(SWIPE_R, HIGH);
    } else {
      digitalWrite(SWIPE_R, LOW);      
    }
    rangeR = (readDuration(SWIPE_R) * (1 - alpha)) + (rangeR  *  alpha);
    rangeL = (readDuration(SWIPE_L)  * (1 - alpha)) + (rangeL  *  alpha);   
  }
  rangeR = rangeR * 0.1; // reduce gesture detection to 
  rangeL = rangeL * 0.1; //  ten percent of maximum range
}

void printdup(String msg) {
  if (msg != dup) {
    Serial.println(msg);
    dup = msg;
  }
}
