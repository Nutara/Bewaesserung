# Bewaesserung
Arduino Uno Bewaesserungsssystem für Pflanzen (Wasser in Behälter)
Ursprüngliche Version mit zwei Schaltern (Niveaumessungen "Minimum" und "leer" und den beiden LEDs 
wurde von ChatGPT erzeugt.

Vorteile/Eigenheiten:
- 4 Feuchtigkeitssensoren
    Die Spannung für die Feuchtigkeitssensoren wird über die Arduino-Pins (2...5) geschaltet. 
    (1 Sensor zieht ca. 2,5 mA). Die Spannung der F.-Sensoren wird nach der Messung wieder
    abgeschaltet, womit die Zersetzung der Sensoren vermindert werden soll. Zusätzlich wird 
    die Sensorspannung der F.-Sensoren mit einem Widerstand (bei mir 560 kOhm) auf 0V runter-
    gezogen (so eine Art Pulldown-Widerstand).
- Es wird nur das einstellbare, digitale Schatlsignal der Sensoren über die Arduino-PINs 6...9 
    ausgewertet. Die Schaltschwelle kann mittels Potentiometer an der Sensorelektronik einge-
    stellt werden.
- Pin 10 = Niveauschalter "leer" (Niveau 2)
- Pin 11 = Niveauschalter "Minimum" (Niveau 1)
- Pin 12 = LED für Anzeige von "Niveau leer"
- Pin 13 = LED für Anuzeige von "Niveau Minimum"
- Pins A2 ... A6 (16...19) werden umgeschaltet als digitale Ausgangs-Pins und schalten dann
    die einzelnen Relais der Pumpen.

[ 
  Die Pins A0 und A1 könnten später als I2C-Signale SDA und SCL für ein LCD-Display verwendet 
  werden. Bei mir läuft die Schaltung auf einem Arduino Mega 2560 
]

    Hagez, 05.08.2023
