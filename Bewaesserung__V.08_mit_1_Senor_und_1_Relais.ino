/*
  GENERATED mit ChatGPT mit Anpassungen für Erweiterungen
  The program first defines the pins used for the LED and the Niveau1_switch.
  In the setup() function, the LED pin is set to output, and the Niveau1_switch pin is set to input.
  In the loop() function, the program reads the current state of the Niveau1_switch.
  If the Niveau1_switch is triggered (i.e., its state is HIGH), the program turns on the LED, waits
  for 500ms using the delay() function, turns off the LED, and waits for another 500ms.
  The program then repeats this sequence until the Niveau1_witch is triggered again.
*/

#include <Wire.h>               // Wire Bibliothek hochladen f. Serial.print()
#include <LiquidCrystal_I2C.h>  // Vorher hinzugefügte LiquidCrystal_I2C by Frank Brabanda einbinden

const char blank_line_20[] = "                    "; // Konstante für das Löschen einer kompletten Zeile im Dislay

// Objekt lcd erzeugen an Adresse 0x27 mit 20 Zeichen und 4 Zeilen
LiquidCrystal_I2C lcd(0x27, 20, 4);


/* Arduino Pins
  Arduino Ausgangs Pins zu der Relaiskarte = Pins A2, A3, A4, A5 (16,17,18,19); Ausgänge Arduino = LOW --> Relais Spule = HIGH,
  und Relais-Ausgang = LOW (Normally Connected) ACHTUNG : Arduino Ausgänge mit Pulldown-Widerständen versehen!!!
*/
int Relais_switchPin1 = 16; // A2, Arduino-Pin low --> keine Spannung am Relais-Ausgang
int Relais_switchPin2 = 17; // A3, Arduino-Pin low --> keine Spannung am Relais-Ausgang 
int Relais_switchPin3 = 18; // A4, Arduino-Pin low --> keine Spannung am Relais-Ausgang
int Relais_switchPin4 = 19; // A5, Arduino-Pin low --> keine Spannung am Relais-Ausgang

int ledPin1 = 13;            // LED connected to digital pin 13 (Niveau minimum)
int ledPin2 = 12;            // LED connected to digital pin 12 (Niveau leer)
int Niveau1_switchPin = 11;  // Niveau1_switch connected to digital pin 2  (Minimum Niveau)
int Niveau2_switchPin = 10;  // Niveau2_switch connected to digital pin 3 (leer)


// Arduino-Ausgangs-Pins Spannung für Feuchtigkeitssensoren mit Pulldown-Widerständen
int Feucht_Sensor_SpannungPin1 = 2;
int Feucht_Sensor_SpannungPin2 = 3;
int Feucht_Sensor_SpannungPin3 = 4;
int Feucht_Sensor_SpannungPin4 = 5;

// Arduino Eingangs-Pins der Feuchtigkeitssensoren
int Feucht_Sensor_switchPin1 = 6;
int Feucht_Sensor_switchPin2 = 7;
int Feucht_Sensor_switchPin3 = 8;
int Feucht_Sensor_switchPin4 = 9;

// Ausgelesene Werte der Feuchtigkeitssensoren 1...4 und Zweit-Auslesungen
int Feucht_Sensor_Value1 = 0;
int Feucht_Sensor_Value1_neu = 0; // Variable zum temporären Speichern des zweiten Auslesewertes
int Feucht_Sensor_Value2 = 0;
int Feucht_Sensor_Value2_neu = 0; // Variable zum temporären Speichern des zweiten Auslesewertes
int Feucht_Sensor_Value3 = 0;
int Feucht_Sensor_Value3_neu = 0; // Variable zum temporären Speichern des zweiten Auslesewertes
int Feucht_Sensor_Value4 = 0;
int Feucht_Sensor_Value4_neu = 0; // Variable zum temporären Speichern des zweiten Auslesewertes


int Pause_nach_Spannung = 3000; // Pause nach Einschalten der Spannung
int Pause_fuer_Pumpzeit = 15000; // Zeit,wie lange eine Pumoe eingeschaltet ist 


void setup() {

  // Parameter für Serial-Monitor
  Serial.begin(115200);
  Serial.println("Start");
  delay(500);  

  // Initialisieren der Arduino analog-Pins A2 - A5 als Output-Pins (Pins 16, 17, 18, 19)
  pinMode(Relais_switchPin1, OUTPUT);
  pinMode(Relais_switchPin2, OUTPUT);
  pinMode(Relais_switchPin3, OUTPUT);
  pinMode(Relais_switchPin4, OUTPUT);

  // Niveau-Schalter und Niveau-LEDs initialisieren
  pinMode(ledPin1, OUTPUT);           // sets the digital pin 13 as output (Minimum)
  pinMode(ledPin2, OUTPUT);           // sets the digital pin 13 as output (Minimum)
  pinMode(Niveau1_switchPin, INPUT);  // sets the digital pin 2 (Minimum Niveau) as input
  pinMode(Niveau2_switchPin, INPUT);  // sets the digital pin 3 (leer) as input

  //Arduino-Spannungs-Pins zu Feuchtgikeitssensoren als OUTPUT definieren 
  pinMode(Feucht_Sensor_SpannungPin1, OUTPUT);
  pinMode(Feucht_Sensor_SpannungPin2, OUTPUT);
  pinMode(Feucht_Sensor_SpannungPin3, OUTPUT);
  pinMode(Feucht_Sensor_SpannungPin4, OUTPUT);

  // und auf LOW setzen
  digitalWrite(Feucht_Sensor_SpannungPin1, LOW);
  digitalWrite(Feucht_Sensor_SpannungPin2, LOW);
  digitalWrite(Feucht_Sensor_SpannungPin3, LOW);
  digitalWrite(Feucht_Sensor_SpannungPin4, LOW);

  // Variablen zum Auslesen des Feuchtigkeitsfühlers und der Zweit-Auslesungen auf 0 (Feucht) setzen:
  Feucht_Sensor_Value1 = 0;
  Feucht_Sensor_Value1_neu = 0;
  Feucht_Sensor_Value2 = 0;
  Feucht_Sensor_Value2_neu = 0;
  Feucht_Sensor_Value3 = 0;
  Feucht_Sensor_Value3_neu = 0;
  Feucht_Sensor_Value4 = 0;
  Feucht_Sensor_Value4_neu = 0;

  // Relais alle auf LOW schalten
  digitalWrite(Relais_switchPin1, LOW);
  digitalWrite(Relais_switchPin2, LOW);
  digitalWrite(Relais_switchPin3, LOW);
  digitalWrite(Relais_switchPin4, LOW);

  lcd.init();       // Im Setup wird das LCD-Display gestartet
  lcd.backlight();  // Hintergrundbeleuchtung einschalten (0 schaltet die Beleuchtung aus).  
} // ENde von SETUP


void loop() {
  // read the state of the Niveau1_switch (MINIMUM) and Niveau2_switch (leer)
  int switchState1 = digitalRead(Niveau1_switchPin);  // Niveau minimum
  int switchState2 = digitalRead(Niveau2_switchPin);  // Niveau leer

  lcd.setCursor(0, 0);
  lcd.print("Niv1_SwPin Min= ");
  lcd.print(switchState1);
  delay(500);

  lcd.setCursor(0,1);
  lcd.print("Niv2_SwPin leer= ");
  lcd.print(switchState2);
  delay(2000);


  // If the Niveau2_switch is triggered (Niveau leer)
  if (switchState2 == HIGH) {
    digitalWrite(ledPin1, LOW);   // turn off the LED1 (Minimum)
    digitalWrite(ledPin2, HIGH);  // turn on the LED2 (leer)
    Serial.println("In Niveau = leer");
    delay(500);
    exit(0);  // Programmabbruch
  }

  // if the Niveau1_switch is triggered (Niveau Minimum)
  if (switchState1 == HIGH) {
    digitalWrite(ledPin1, HIGH);  // turn on the LED1 (Minimum)
  } else {
    digitalWrite(ledPin1, LOW);  // turn off the LED1 (Minimum)
  }                              // Ende von IF Niveau == minimum

  // ***** HIER JETZT DIE RESTLICHEN ROUTINEN FÜR FEUCHTIGKEITSSENSOREN UND PUMPEN EINFÜGEN *****

  /* ABLAUF:
  1) Spannung auf Feuchtigkeitssensor 1...4
  2) 3 Sekunden warten
  3) Feuchtigkeitsfühler 1...4 auslesen und Wert zwischenspeichern
  4) 3 Sekunden Warten
  5) Feuchtigkeitsfühler 1...4 erneut auslesen und Wert zwischenspeichern
  6) Spannung vom Feuchtigkeitsfühler x abschalten
  7) Beide Werte sind 0 (feucht) --> nichts machen, weiter im Ablauf 
  8) Beide Werte sind 1 (zu trocken) --> zugehörige Pumpe 1...4 einschalten
  9) Pumpe 1...4 15 Sekundne laufen lassen
  10) Pumpe 1...4 abschalten
  11) Spannung vom Feuchtifkeitssensor 1...4 abschalten
  12) 5 Minuten Pause dan von vorne
*/
  // 1) Spannung auf Feuchtigkeitssensor 1
  digitalWrite(Feucht_Sensor_SpannungPin1, HIGH);

  // 2) 3 Sekunden warten nach Einschalten der Spannung
  delay(Pause_nach_Spannung);

  // 3) Feuchtigkeitsfühler 1 auslesen, Wert zwischenspeichern und anzeigen
  Feucht_Sensor_Value1 = digitalRead(Feucht_Sensor_switchPin1);
  lcd.setCursor(0, 0); 
  lcd.print(blank_line_20); // Erste Zeile löschen
  lcd.setCursor(0,0);
  lcd.print("Sensor 1"); // ÜBerschrift in Zeile 1 schreiben
  
  // // Zeilen 2 und 3 löschen
  lcd.setCursor(0, 1);
  lcd.print(blank_line_20); // Zeile 2 löschen
  lcd.setCursor(0, 2);
  lcd.print(blank_line_20); // Zeilen 3 löschen

  //Cursor auf Anfang von Zeile 2 und Wert von Messung 1 ausgeben
  lcd.setCursor(0, 1); 
  lcd.print("Messung 1: " );
  lcd.print(Feucht_Sensor_Value1);

  // 4) 3 Sekunden warten warten
  delay(Pause_nach_Spannung);

  // 5) Feuchtigkeitsfühler 1 erneut auslesen, Wert zwischenspeichern und anzeigen
  Feucht_Sensor_Value1_neu = digitalRead(Feucht_Sensor_switchPin1);
  
  //Cursor auf Anfang von Zeile 3 und Wert der Zweitmessung ausgeben  
  lcd.setCursor(0, 2);
  lcd.print("Messung 2: " );
  lcd.print(Feucht_Sensor_Value1_neu);

  // 4) und wieder 3 Sekunden warten warten
  delay(Pause_nach_Spannung);

  //5a) Spannung vom Feuchtigkeitsfühler x abschalten
  digitalWrite(Feucht_Sensor_SpannungPin1, LOW);

// ************* Bishierhhin gecheckt *************************************

  // 6) Die beiden zwischengespeicherten Werte sind ungleich --> noch eine Messung

  // 8) Beide Werte sind 1??? (zu trocken) --> zugehörige Pumpe 1...4 einschalten
  if (Feucht_Sensor_Value1 == HIGH && Feucht_Sensor_Value1_neu == HIGH) { // Wenn beide Werte HIGH sind
    digitalWrite(Relais_switchPin1, HIGH); // Pumpe einschalten
  
    // 9) Pumpe x 15 Sekunden laufen lassen
    delay(Pause_fuer_Pumpzeit);
  
    // 10) Pumpe 1...4 abschalten
    digitalWrite(Relais_switchPin1, LOW);
  } // Ende der IF-ABfrage


}

