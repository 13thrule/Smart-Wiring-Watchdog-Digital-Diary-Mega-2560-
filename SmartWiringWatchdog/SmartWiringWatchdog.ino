#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);

// --- HARDWARE CONFIGURATION ---
const int RED = 49, YEL = 47, GRN = 43, BZ = 41;
const int BTN_ARM = 50, BTN_DISARM = 48; 

// --- SYSTEM STATES ---
int protectedPin = -1;      
bool isAlarmActive = false; 
unsigned long pageTimer = 0;
int currentPage = 0; 
bool libraryLocked = false;

// --- TRIPLE PRESS LOGIC ---
int clickCount = 0;
unsigned long lastClickTime = 0;
const int clickInterval = 1000; 

// --- WIRING DIARY (RAM) ---
int diary[10]; 
int diaryIndex = 0;

void setup() {
  delay(500); 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  
  pinMode(RED, OUTPUT); pinMode(YEL, OUTPUT); pinMode(GRN, OUTPUT); pinMode(BZ, OUTPUT);
  noTone(BZ); 

  pinMode(BTN_ARM, INPUT_PULLUP);
  pinMode(BTN_DISARM, INPUT_PULLUP);

  for(int i = 2; i <= 53; i++) {
    if(i != 20 && i != 21 && i != 49 && i != 47 && i != 43 && i != 41 && i != 50 && i != 48) {
      pinMode(i, INPUT_PULLUP);
    }
  }
  // Initialize diary with 0s once at startup
  for(int i=0; i<10; i++) diary[i] = 0; 
}

void loop() {
  // 1. PIN SCANNER
  int currentScan = -1;
  int activeCount = 0;
  for(int i = 2; i <= 53; i++) {
    if(i == 20 || i == 21 || i == 49 || i == 47 || i == 43 || i == 41 || i == 50 || i == 48) continue;
    if(digitalRead(i) == LOW) {
      activeCount++;
      if(currentScan == -1) currentScan = i;
    }
  }

  // 2. BUTTON LOGIC
  bool armPressed = (digitalRead(BTN_ARM) == LOW);
  bool disarmPressed = (digitalRead(BTN_DISARM) == LOW);

  // --- BTN 48: DISARM / EXIT (Now preserves Diary) ---
  if(disarmPressed) {
    protectedPin = -1;      // Stop guarding the current pin
    isAlarmActive = false;  // Kill the alarm
    libraryLocked = false;  // Unlock the screen
    currentPage = 0;        // Go back to Monitor
    noTone(BZ);
    // Note: diary[] is NOT reset here
    delay(300); 
  }

  // --- BTN 50: ARM / TRIPLE CLICK ---
  if(armPressed) {
    unsigned long currentTime = millis();
    if (currentTime - lastClickTime < clickInterval) { clickCount++; } 
    else { clickCount = 1; }
    lastClickTime = currentTime;

    if (clickCount >= 3) {
      libraryLocked = true;
      currentPage = 1; 
      tone(BZ, 2000, 100); delay(100); tone(BZ, 2500, 100);
      clickCount = 0;
    } 
    else if (currentScan != -1 && !libraryLocked) {
      protectedPin = currentScan;
      isAlarmActive = false;
      
      // Update Diary only if it's a new pin
      bool exists = false;
      for(int i=0; i<10; i++) { if(diary[i] == currentScan) exists = true; }
      if(!exists) { 
        diary[diaryIndex] = currentScan; 
        diaryIndex = (diaryIndex + 1) % 10; 
      }
    }
    delay(250); 
  }

  // 3. WATCHDOG (Auto-Reset)
  if(protectedPin != -1) {
    isAlarmActive = (digitalRead(protectedPin) == HIGH);
  }

  // 4. LIGHTS & SOUND
  if (isAlarmActive) {
    digitalWrite(RED, HIGH); digitalWrite(YEL, LOW); digitalWrite(GRN, LOW);
    if((millis() / 250) % 2 == 0) tone(BZ, 2000); else noTone(BZ);
  } else {
    noTone(BZ);
    if (protectedPin != -1) { digitalWrite(RED, LOW); digitalWrite(YEL, LOW); digitalWrite(GRN, HIGH); }
    else if (activeCount > 0) { digitalWrite(RED, LOW); digitalWrite(YEL, HIGH); digitalWrite(GRN, LOW); }
    else { digitalWrite(RED, LOW); digitalWrite(YEL, LOW); digitalWrite(GRN, HIGH); }
  }

  // 5. PAGE DISPLAY LOGIC
  if (!libraryLocked && (millis() - pageTimer >= 5000)) { 
    pageTimer = millis(); 
    if (currentPage == 0) currentPage = 2; 
    else currentPage = 0; 
  }
  
  display.clearDisplay();
  display.setTextColor(WHITE);
  
  if (currentPage == 0) {
    // MONITOR
    display.setTextSize(1); display.setCursor(0,0);
    display.print("WATCHDOG MONITOR");
    display.drawLine(0, 9, 128, 9, 1);
    if(isAlarmActive) {
      display.setCursor(15, 20); display.setTextSize(2); display.print("! ALARM !");
      display.setTextSize(1); display.setCursor(15, 45);
      display.print("PORT "); display.print(protectedPin); display.print(" REMOVED");
    } else {
      display.setCursor(0, 15);
      if(protectedPin != -1) {
        display.print("STATUS: SECURED");
        display.setTextSize(2); display.setCursor(0, 30); display.print("PORT #"); display.print(protectedPin);
      } else {
        display.print(currentScan != -1 ? "STATUS: FOUND" : "STATUS: READY");
        display.setTextSize(2); display.setCursor(0, 30);
        if(currentScan != -1) { display.print("PORT #"); display.print(currentScan); }
        else { display.setTextSize(1); display.setCursor(20,35); display.print("SCANNING..."); }
      }
    }
  } 
  else if (currentPage == 1) {
    // LIBRARY (LOCKED)
    display.setTextSize(1); display.setCursor(0,0);
    display.print("WIRING LIBRARY");
    display.drawLine(0, 9, 128, 9, 1);
    for(int i = 0; i < 10; i++) {
      int row = (i < 5) ? 20 : 40;
      int col = (i % 5) * 25;
      display.setCursor(col, row);
      if(diary[i] != 0) { display.print("#"); display.print(diary[i]); } else { display.print("--"); }
    }
    display.setCursor(0, 55); display.print("RESET TO EXIT (KEEPS DATA)");
  } 
  else {
    // REGISTRY
    display.setTextSize(1); display.setCursor(0,0);
    display.print("HARDWARE REGISTRY");
    display.drawLine(0, 9, 128, 9, 1);
    display.setCursor(0, 15); display.print("ARM: 50 | CLR: 48");
    display.setCursor(0, 25); display.print("BZ:  41 | RED: 49");
    display.setCursor(0, 35); display.print("YEL: 47 | GRN: 43");
    display.setCursor(0, 50); display.print("ACTIVE: "); 
    if(protectedPin == -1) display.print("NONE"); else display.print(protectedPin);
  }
  display.display();
}