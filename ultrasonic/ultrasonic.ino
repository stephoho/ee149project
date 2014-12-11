#define trigR 13
#define echoR 12
#define trigL 11
#define echoL 10

#define left  7
#define right 6

long durationR_prev, durationL_prev;
float alpha = 0.01;



int sensVal;           // for raw sensor values 
float filterVal;       // this determines smoothness  - .0001 is max  1 is off (no smoothing)
float smoothedVal;     // this holds the last loop value just use a unique variable for every different sensor that needs smoothing
float smoothedVal2;   // this would be the buffer value for another sensor if you needed to smooth two different sensors - not used in this sketch
int i, j;              // loop counters or demo     




void setup() {
  pinMode(trigR, OUTPUT);
  pinMode(echoR, INPUT);
  pinMode(trigL, OUTPUT);
  pinMode(echoL, INPUT);

  pinMode(left,  OUTPUT);
  pinMode(right, OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  long durationR, distanceR;
  long durationL, distanceL;  

  durationR = readDuration(right);
   for (i = 0; i < 7; i++){    // substitute some different filter values
    filterVal = i * .15; 
    for (j = 0; j< 60; j++){      
      if (j < 30){            // generate a simulated square wave
        sensVal = 1023;
      } else {
        sensVal = 0; 
      }
      sensVal = durationR;
      smoothedVal =  smooth(sensVal, filterVal, smoothedVal);   // second parameter determines smoothness  - 0 is off,  .9999 is max smooth 

      /*Serial.print(sensVal);
      Serial.print("   ");
      Serial.print(smoothedVal, DEC);
      Serial.print("      ");
      Serial.print("filterValue * 100 =  ");   // print doesn't work with floats
      Serial.println(filterVal * 100, DEC);
      delay(30); */
    }
  }
  
  
  
  durationL = readDuration(left);


  Serial.print("Right: ");
  Serial.print(durationR);
  Serial.print("\t");
  Serial.print("Left:  ");
  Serial.print(durationL);
  Serial.println("\n");
  delayMicroseconds(1000);  

}

int readDuration(int side) {
  int trig, echo;
  if (side == left) {
    trig = trigL;
    echo = echoL;
  } else if (side == right) {
    trig = trigR;
    echo = trigR;
  }
  digitalWrite(trig, LOW);
  delayMicroseconds(5);    
  digitalWrite(trig, HIGH);  
  delayMicroseconds(10);  
  return pulseIn(echo, HIGH);
}

int smooth(int data, float filterVal, float smoothedVal){


  if (filterVal > 1){      // check to make sure param's are within range
    filterVal = .99;
  }
  else if (filterVal <= 0){
    filterVal = 0;
  }

  smoothedVal = (data * (1 - filterVal)) + (smoothedVal  *  filterVal);

  return (int)smoothedVal;
}

