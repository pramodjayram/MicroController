#define trigPin 6
#define echoPin 5
int flag=0;
float distance=0,distance_prev=0,time1=0,time1_prev=0,speed1=0;
void setup() {
  
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
 Serial1.begin(9600);
 Serial1.write("AT");
  delay(1200);
   while (Serial1.available()) {  
    Serial.write(Serial1.read());
  }
  //If Serial receive data, print out to Serial1
  while (Serial.available()) {
    Serial1.write(Serial.read());
  }
  Serial1.write("\n");
  Serial1.write("AT+CMGF=1");
  delay(1200);
   while (Serial1.available()) {  
    Serial.write(Serial1.read());
  }
  //If Serial receive data, print out to Serial1
  while (Serial.available()) {
    Serial1.write(Serial.read());
  }
  Serial1.write("\n");
  Serial1.write("AT+CGNSPWR=1");
 delay(1200);
 Serial1.write("\n");
 while (Serial1.available()) {  
    Serial.write(Serial1.read());
  }
  
}

void loop() {
  String coord="";
  String latlon="";
 
  long duration, distance;
  digitalWrite(trigPin, LOW);  
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  distance /=100;
  time1=millis();
  speed1=(distance-distance_prev)/(time1-time1_prev);
  time1_prev=time1;
  distance_prev=distance;
   Serial.print(distance);
   Serial.print("\t");
   Serial.println(speed1);
  
 
  if(distance<10 && flag==0){
 flag=1;
 int j=0;
 while(j<10){
  coord="";
 latlon="";
 j+=1;
 
 Serial1.write("AT+CGNSINF");
 delay(1200);
 Serial1.write("\n");
 
 while (Serial1.available()>0)            //checking serial data from Serial1
   {
     char inChar = (char)Serial1.read();
     coord+=inChar;
   }
   
 latlon=coord.substring(45,62);
   
 }
 
 Serial.println(latlon);

  Serial1.print("AT+CMGS=\"+Numbertobecontacted\"");
  delay(1200);
   Serial1.print("\n");
   while (Serial1.available()) {  
    Serial.write(Serial1.read());
  }

  Serial1.print("Tailgating detected. Fine issued at ");
  

Serial1.print(latlon);
delay(1200);
Serial1.print("\n");

 while (Serial1.available()) {
    Serial.write(Serial1.read());
  }
 Serial1.write(0x1A);
  Serial1.write("\n");
  Serial1.write(0x1A);
  delay(1200);
  while (Serial1.available()) {  
    Serial.write(Serial1.read());
  }
 
  }
  else{
    flag=0;
  }
 
}
