
#define SWIPE_L 20
#define SWIPE_R 21

#define r 7
#define g 6
#define b 5
#define in  2
#define out 3

float rangeR, rangeL;
float alpha = 0.0001;

int notif[10][3];
int cur = 0;
int tot = 0;

String dup = "";

void setup() {
  Serial.begin(9600);
  Serial.println("calibrating...");
  
  pinMode(r, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(in,  INPUT);
  pinMode(out, INPUT);  
 
  ultra_setup();
 randomSeed(analogRead(0));  

  Serial.println("done calibrating");
}

void loop() {
  addNotif();
  remNotif();

  analogWrite(r, notif[cur][0]);
  analogWrite(g, notif[cur][1]);  
  analogWrite(b, notif[cur][2]);  

  if (tot > 1) {
    Serial.println("detecting sweep");
    int swipe = readSwipe();

    if (swipe == SWIPE_R) {
      Serial.println("SWIPE RIGHT");
      cur = (cur + 1) % tot;
    } 
    else if (swipe == SWIPE_L) {
      Serial.println("SWIPE LEF");      
      cur = (cur - 1) % tot;
    }
  }
  printdup(String(cur) + " out of " + String(tot) + " : " + String(notif[cur][0]) + "\t" + String(notif[cur][1]) + "\t" + String(notif[cur][2]));
}

void addNotif() {
  if (digitalRead(in) == HIGH && tot < 9) {
    int red = random(255);
    int gre = random(255);
    int blu = random(255);
    notif[tot][0] = red;
    notif[tot][1] = gre;
    notif[tot][2] = blu;
    cur = tot;
    tot += 1;
    Serial.print("new notification: \t");
    Serial.print(red);
    Serial.print("\t");
    Serial.print(gre);
    Serial.print("\t");
    Serial.print(blu);
    Serial.println("\n");
  }
  delay(1000);
}

void remNotif() {
  if (digitalRead(out) == HIGH && tot > 0) {
    Serial.print("remove notification: \t");
    Serial.print(notif[0][0]);
    Serial.print("\t");
    Serial.print(notif[0][1]);
    Serial.print("\t");
    Serial.print(notif[0][2]);
    Serial.println("\n");
    for (int i = 0; i < tot; i++) {
      notif[i][0] = notif[i+1][0];
      notif[i][1] = notif[i+1][1];
      notif[i][2] = notif[i+1][2];      
    }
    for (int j = tot; j < 10; j++) {
      notif[j][0] = 0;
      notif[j][1] = 0;
      notif[j][2] = 0;
    }
  }
  delay(1000);  
}

/** Returns:
 *    -1	if   no  swipe 
 *  SWIPE_R	if right swipe
 *  SWIPE_L	if  left swipe
 */
int readSwipe() {
  boolean detectR, detectL;
  long start, duration;

  detectR =  readDuration(SWIPE_R) < rangeR;
  detectL =  readDuration(SWIPE_L) < rangeL;
  start = millis();

  if (detectR && !detectL) {
    delay(DELAY);
    while(detectR || detectL) {
      detectR =  readDuration(SWIPE_R) < rangeR;
      detectL =  readDuration(SWIPE_L) < rangeL;
    }
    return SWIPE_R;
  } 
  else if (!detectR && detectL) {
    delay(DELAY);
    while(detectR || detectL) {
      detectR =  readDuration(SWIPE_R) < rangeR;
      detectL =  readDuration(SWIPE_L) < rangeL;
    }
    return SWIPE_L;  
  }

  return -1;
}

/* Returns the reading from the @side sensor. */
int readDuration(int side) {
  int trig, echo;
  if (side == SWIPE_L) {
    trig = trigL;
    echo = echoL;
  } 
  else if (side == SWIPE_R) {
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

  calibrate();
}

/* Sample the average reading for maximum sensor range. */
void calibrate() {
  rangeR = readDuration(SWIPE_R);
  rangeL = readDuration(SWIPE_L);
  for (int i = 0; i < 5; i++) {
    if (i % 2 == 0) {
      digitalWrite(r, HIGH);
    } else {
      digitalWrite(r, LOW);      
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

