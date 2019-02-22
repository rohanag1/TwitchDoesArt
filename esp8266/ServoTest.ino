#include <Servo.h>
#include <ESP8266WiFi.h>
 
const char* ssid     = "RedRover";
const char* password = "yourpassword";
 
const char* host = "10.148.3.239";

//Music
#define  a3f    208     // 208 Hz
#define  b3f    233     // 233 Hz
#define  b3     247     // 247 Hz
#define  c4     261     // 261 Hz MIDDLE C
#define  c4s    277     // 277 Hz
#define  e4f    311     // 311 Hz    
#define  f4     349     // 349 Hz 
#define  a4f    415     // 415 Hz  
#define  b4f    466     // 466 Hz 
#define  b4     493     // 493 Hz 
#define  c5     523     // 523 Hz 
#define  c5s    554     // 554 Hz
#define  e5f    622     // 622 Hz  
#define  f5     698     // 698 Hz 
#define  f5s    740     // 740 Hz
#define  a5f    831     // 831 Hz 

#define rest    -1

// change these pins according to your setup
int piezo = 3;

volatile int beatlength = 100; // determines tempo
float beatseparationconstant = 0.3;

int a; // part index
int b; // song index
int c; // lyric index

boolean flag; // play/pause

// Parts 1 and 2 (Intro)

int song1_intro_melody[] =
{c5s, e5f, e5f, f5, a5f, f5s, f5, e5f, c5s, e5f, rest, a4f, a4f};

int song1_intro_rhythmn[] =
{6, 10, 6, 6, 1, 1, 1, 1, 6, 10, 4, 2, 10};

// Parts 3 or 5 (Verse 1)

int song1_verse1_melody[] =
{ rest, c4s, c4s, c4s, c4s, e4f, rest, c4, b3f, a3f,
  rest, b3f, b3f, c4, c4s, a3f, a4f, a4f, e4f,
  rest, b3f, b3f, c4, c4s, b3f, c4s, e4f, rest, c4, b3f, b3f, a3f,
  rest, b3f, b3f, c4, c4s, a3f, a3f, e4f, e4f, e4f, f4, e4f,
  c4s, e4f, f4, c4s, e4f, e4f, e4f, f4, e4f, a3f,
  rest, b3f, c4, c4s, a3f, rest, e4f, f4, e4f
};

int song1_verse1_rhythmn[] =
{ 2, 1, 1, 1, 1, 2, 1, 1, 1, 5,
  1, 1, 1, 1, 3, 1, 2, 1, 5,
  1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 3,
  1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 4,
  5, 1, 1, 1, 1, 1, 1, 1, 2, 2,
  2, 1, 1, 1, 3, 1, 1, 1, 3
};



// Parts 4 or 6 (Chorus)

int song1_chorus_melody[] =
{ b4f, b4f, a4f, a4f,
  f5, f5, e5f, b4f, b4f, a4f, a4f, e5f, e5f, c5s, c5, b4f,
  c5s, c5s, c5s, c5s,
  c5s, e5f, c5, b4f, a4f, a4f, a4f, e5f, c5s,
  b4f, b4f, a4f, a4f,
  f5, f5, e5f, b4f, b4f, a4f, a4f, a5f, c5, c5s, c5, b4f,
  c5s, c5s, c5s, c5s,
  c5s, e5f, c5, b4f, a4f, rest, a4f, e5f, c5s, rest
};

int song1_chorus_rhythmn[] =
{ 1, 1, 1, 1,
  3, 3, 6, 1, 1, 1, 1, 3, 3, 3, 1, 2,
  1, 1, 1, 1,
  3, 3, 3, 1, 2, 2, 2, 4, 8,
  1, 1, 1, 1,
  3, 3, 6, 1, 1, 1, 1, 3, 3, 3, 1, 2,
  1, 1, 1, 1,
  3, 3, 3, 1, 2, 2, 2, 4, 8, 4
};

Servo lFront;
Servo lBack;
Servo rFront;
Servo rBack;
Servo lPen;
Servo rPen;

#define left 0
#define right 1
#define forward 1
#define backward -1
#define wait 0
int CurrentPen=left;

//Lift both the pens up (aka disable drawing)

void pensUp(){
  lPen.write(100);
  rPen.write(70);  
}

void pensDown(){
  lPen.write(77);
  rPen.write(93);
}

void backDown(){
  if (CurrentPen==left){
    lPen.write(77);
    rPen.write(70);
  }
  else{
    lPen.write(100);
    rPen.write(93);
  }
}
void penSwitch(){
  if (CurrentPen==left){
    lPen.write(100);
    rPen.write(93);
    CurrentPen=right;
  }
  else{
    lPen.write(77);
    rPen.write(70);
    CurrentPen=left;
  }
}

//Lift both the pens down (aka enable drawing)

//Initiate series of steps that allows us to continue from where we left off with the other pen
void switchColor(){
  //left to right
  pensUp();
  if (CurrentPen==left){
    pivot(left, 640);
    pivot(right,770);
    drive(backward, 700);
  }
  //right to left
  else{
    pivot(right, 640);
    pivot(left, 770);
    drive(backward, 700);
  }
  penSwitch();
  delay(2000);
}

//banana = direction; move both wheels on left side of robot in dir (called in several functions like drive)
void leftSide(int banana){
 if(banana==forward){
    lFront.write(107);
    lBack.write(107);
  }
  else if (banana==backward){
    lFront.write(73);
    lBack.write(73);
  }
  else{
    lFront.write(90);
    lBack.write(90);
  }
}

//banana = direction; move both wheels on right side of robot in dir (called in several functions like drive)
void rightSide(int banana){
 if(banana==forward){
    rFront.write(70);
    rBack.write(70);
  }
  else if (banana==backward){
    rFront.write(110);
    rBack.write(110);
  }
  else{
    rFront.write(90);
    rBack.write(90);
  }
}

//Move straight
void drive(int dir, int duration){
  leftSide(dir);
  rightSide(dir);
  delay(duration); 
  leftSide(wait);
  rightSide(wait);
}

//pivot in a certain direction, with a certain time delay before you stop pivoting (to make variable degree turns)
void pivot(int dir, int duration){
  if (dir==left){
    leftSide(0);
    rightSide(1);
  }
  else {
    leftSide(1);
    rightSide(0);
  }
  delay(duration);
  leftSide(wait);
  rightSide(wait);
}

//make a turn 90 degrees 
void leftTurn(){
    pensUp();
    lFront.write(50);
    lBack.write(50);
    rFront.write(95);
    rBack.write(95);
    delay(1500);
    drive(forward, 1200);
    backDown();
}

void rightTurn(){
    pensUp();
    lFront.write(85);
    lBack.write(85);
    rFront.write(130);
    rBack.write(130);
    delay(1500);
    drive(forward, 1000);
    backDown();
}

void circle() {
  if (CurrentPen == left) {
    pivot(left, 7000);
  }
  else {
    pivot(right, 7000);
  }
}

void square() {
  if (CurrentPen == left) {
    rightTurn();
    drive(forward, 800);
    rightTurn();
    drive(forward, 800);
    rightTurn();
    drive(forward, 800);
    rightTurn();
    drive(forward, 800);
  }
  else {
    leftTurn();
    drive(forward, 800);
    leftTurn();
    drive(forward, 800);
    leftTurn();
    drive(forward, 800);
    leftTurn();
    drive(forward, 800);
  }
}


String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void play() {
  int notelength;
  if (a == 1 || a == 2) { // Intro
    // intro
    notelength = beatlength * song1_intro_rhythmn[b];
    if (song1_intro_melody[b] > 0) { // if not a rest, play note
      tone(piezo, song1_intro_melody[b], notelength);
    }
    b++;
    if (b >= sizeof(song1_intro_melody) / sizeof(int)) {
      a++;
      b = 0;
      c = 0;
    }
  }
  else if (a == 3 || a == 5) { // Verse 1
    // verse
    notelength = beatlength * 2 * song1_verse1_rhythmn[b];
    if (song1_verse1_melody[b] > 0) {
      tone(piezo, song1_verse1_melody[b], notelength);
      c++;
    }
    b++;
    if (b >= sizeof(song1_verse1_melody) / sizeof(int)) {
      a++;
      b = 0;
      c = 0;
    }
  }
  else if (a == 4 || a == 6) { //chorus
    // chorus
    notelength = beatlength * song1_chorus_rhythmn[b];
    if (song1_chorus_melody[b] > 0) {
      tone(piezo, song1_chorus_melody[b], notelength);
      c++;
    }
    b++;
    if (b >= sizeof(song1_chorus_melody) / sizeof(int)) {
      a++;
      b = 0;
      c = 0;
    }
  }
  delay(notelength); // necessary because piezo is on independent timer
  noTone(piezo);
  delay(notelength * beatseparationconstant); // create separation between notes
  if (a == 7) { // loop back around to beginning of song
    a = 1;
  }
} 

void setup() {
  // put your setup code here, to run once:
  lFront.attach(13);
  rFront.attach(12);
  lBack.attach(0);
  rBack.attach(15);
  lPen.attach(2);
  rPen.attach(16);
  Serial.begin(115200);
  delay(100);

  //music setup
  pinMode(piezo, OUTPUT);
  flag = true;
  a = 4;
  b = 0;
  c = 0;
 
  // We start by connecting to a WiFi network
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:
  //drive(forward or backward, duration)
  //pivot(left or right, duration)
  //switchColor() switches the pen
  //
  delay(1000);
  
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 90;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/getData";
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(500);
  
  // Read all the lines of the reply from server and print them to Serial
  String line = "";
  while(client.available()){
    line += client.readStringUntil('\r');
  }
  
//  Serial.println("closing connection");

String twitch = getValue(line, '\n', 7);
String music = getValue(line, '\n', 8);

Serial.println("Line is:");
  Serial.println(line);

Serial.println("Twitch is:");
  Serial.println(twitch);

Serial.println("Music is:");
  Serial.println(music);
  backDown();

   if (music == "On") {
    Serial.println("Playing music");
    while (flag == true) {
      play();
    }
  }
  else {
    Serial.println("Not playing music");
  }
  
  if (twitch == "Pivot Left"){
    Serial.println("Entered pivot left");
    pivot(left, 1000);
  }
  else if (twitch == "Pivot Right") {
    Serial.println("Entered pivot right");
    pivot(right, 1000);
  }
  else if (twitch == "Forward") {
    Serial.println("Entered go forward");
    drive(forward, 1000);
  }
  else if (twitch == "Backward") {
    Serial.println("Entered go backward");
    pensUp();
    drive(backward, 1000);
    backDown();
  }
  else if (twitch == "Left") {
    Serial.println("Entered 90 degree left");
    leftTurn();
  }
  else if (twitch == "Right") {
    Serial.println("Entered 90 degree right");
    rightTurn();
  }
  else if (twitch == "Pen Up") {
    Serial.println("Entered pen up");
    pensUp();
  }
  else if (twitch == "Switch Colors") {
    Serial.println("Entered switch colors");
    switchColor();
  }
  else if (twitch == "Circle") {
    Serial.println("Entered circle");
    circle();
  }
  else if (twitch == "Square") {
    Serial.println("Entered square");
    square();
  }
  else {
    Serial.println("Doing nothing");
  }


}
