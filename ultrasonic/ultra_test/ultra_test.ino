#define trigR 13
#define echoR 12
#define trigL 11
#define echoL 10

#define SWIPE_L 27
#define SWIPE_R 26

#define DELAY 250

#define r 7
#define g 6
#define b 5
#define in  2
#define out 3


long rangeR = 1000;
long rangeL = 1000;
float alpha = 0.0001;

String dup = "";

int cur = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("calibrating..");
  pinMode(SWIPE_R, OUTPUT);
  pinMode(SWIPE_L, OUTPUT);  
  
  pinMode(r, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(in,  INPUT);
  pinMode(out, INPUT);  
 
  ultra_setup();

  randomSeed(analogRead(0));  
    
  Serial.println("calibration complete.");
}
  
void loop() {
  int swipe = readSwipe();
  if (swipe == SWIPE_R) {
    Serial.println(">>>>>>>>>>>>>>>>>>>RIGHT>>>>>>>>>>>>>>>>>>>");
    digitalWrite(r, LOW);
    digitalWrite(g, LOW);
    digitalWrite(b, LOW);
    delay(DELAY);
  } else if (swipe == SWIPE_L) {
    int red = random(100, 255);
    int gre = random(100, 255);
    int blu = random(100, 255);
    Serial.println("<<<<<<<<<<<<<<<<<<<<<LEFT<<<<<<<<<<<<<<<<<<<<<");
    //Serial.println("RED: " + String(red) + "\tGREEN: " + String(gre) + "\tBLUE: " + String(blu));
    analogWrite(r, red);
    analogWrite(g, gre);    
    analogWrite(b, blu);  
    delay(DELAY);
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
  long last_R;
  long last_L;
  detectR =  (right < rangeR) && (right > 0);
  detectL =   (left < rangeL) && (left > 0);
  
  start = millis();

  
  if (detectR && !detectL) {
    while(detectR || detectL) {
      Serial.println("Left: \t" + String(left) + "\tRight:\t" + String(right));  
      last_R = right;
      last_L = left;
      right = readDuration(SWIPE_R); 
      left  = readDuration(SWIPE_L);   
      detectR =  right < rangeR && (right > 0);
      detectL =  left  < rangeL && (left > 0); 
    }
    detectR =  last_R < rangeR && (last_R > 0);
    detectL =  last_L < rangeL && (last_L > 0); 
    if (!detectR && detectL) {
      return SWIPE_R;
    } else {
      Serial.println("INCONSISTENT READING");
    }      
  } else if (!detectR && detectL) {
    while(detectR || detectL) {
      Serial.println("Left: \t" + String(left) + "\tRight:\t" + String(right));  
      last_R = right;
      last_L = left;
      right = readDuration(SWIPE_R); 
      left  = readDuration(SWIPE_L);       
      detectR =  right < rangeR && (right > 0);
      detectL =  left  < rangeL && (left > 0); 
    }
    detectR =  last_R < rangeR && (last_R > 0);
    detectL =  last_L < rangeL && (last_L > 0); 
    if (detectR && !detectL) {
      return SWIPE_L;  
    } else {
      Serial.println("INCONSISTENT READING");
    }
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
  return pulseIn(echo, HIGH, 10000);
}

/* Setup IO pins for each of the sensors. */
void ultra_setup() {
  pinMode(trigR, OUTPUT);
  pinMode(echoR, INPUT);
  pinMode(trigL, OUTPUT);
  pinMode(echoL, INPUT);

  //calibrate();
}

/* Sample the average reading for maximum sensor range. */
void calibrate() {
  rangeR = readDuration(SWIPE_R);
  rangeL = readDuration(SWIPE_L);
  for (int i = 0; i < 50; i++) {
    rangeR = (readDuration(SWIPE_R) * (1 - alpha)) + (rangeR  *  alpha);
    rangeL = (readDuration(SWIPE_L)  * (1 - alpha)) + (rangeL  *  alpha);   
    Serial.println("read: \t" + String(rangeL) + "\t" + String(rangeR));    
  }
  rangeR = rangeR * 0.1; // reduce gesture detection to 
  rangeL = rangeL * 0.1; //  ten percent of maximum range
  Serial.println("final: \t" + String(rangeL) + "\t" + String(rangeR));
}

void printdup(String msg) {
  if (msg != dup) {
    Serial.println(msg);
    dup = msg;
  }
}
