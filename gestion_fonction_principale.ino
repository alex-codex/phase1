#include <Servo.h>
#include <Bonezegei_DHT11.h>

// Définition des pins
#define BUTTON_PIN 2
#define LED_PIN 3
#define PIN_SON A0
#define DHT_PIN 5
#define TRIG_PIN 6
#define ECHO_PIN 7
#define SERVO_PIN 8

 //Variables
Servo myServo;
Bonezegei_DHT11 dht(DHT_PIN);

bool ledState = false;
unsigned long lastClap = 0;
unsigned long delai_clap = 1000;
unsigned long last_humidite = 0;
unsigned long last_distance = 0;
unsigned long humidite_Intervalle = 450; // Intervalle de lecture de l'humidité
unsigned long ultrasonic_Intervalle = 450; // Intervalle de lecture du capteur ultrason
unsigned long startTime = 0;  // Temps de début de l'action
const long duree = 2000;   // Durée de l'action en millisecondes 
bool actionActive = false;    // État de l'action

void Porte_o() {
    if (!actionActive) {  // je considere la porte ferme des le debut
        actionActive = true;
        startTime = millis();  // Enregistre le temps de début
        myServo.write(180);  // tourne moteur pour ouvrir
    }
}

void Porte_f() {
    if (actionActive && millis() - startTime >= duree) {  // la porte etait precedemment ouverte et on a attendu 'duree' secondes
        actionActive = false;
        myServo.write(0);  // tourne moteur pour fermer
    }
}
// Setup
void setup() {
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(PIN_SON, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  myServo.attach(SERVO_PIN);
  myServo.write(0); 

  dht.begin();

  Serial.begin(9600);
}

// Loop
void loop() {
  unsigned long currentMillis = millis();
  
  // Bouton poussoir
  if (digitalRead(BUTTON_PIN) == HIGH) {
    tone(9, 1750, 300); 
  }

  // Clap de mains
  if (analogRead(PIN_SON) >120 && (currentMillis - lastClap) > delai_clap) {
    ledState = !ledState;delai_clap
    digitalWrite(LED_PIN, ledState);
    lastClap = currentMillis;
  } 

  // Capteur d'humidité
  if (currentMillis - last_humidite >= humidite_Intervalle) {
    if (dht.getData()) {                        
    float tempDeg = dht.getTemperature();      
     int hum = dht.getHumidity();              
    String str  = "Temperature: ";
           str += tempDeg;
           str += "°C  ";
           str +=" ; Humidite:";
           str += hum;
    Serial.println(str.c_str()); 
    Serial.println("");
    }
    last_humidite = currentMillis;
  }

  // Capteur ultrason
  if (currentMillis - last_distance >= ultrasonic_Intervalle) {
    last_distance = currentMillis;
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duree = pulseIn(ECHO_PIN, HIGH);
    float distance = duree * 0.034 / 2;

    Serial.print("Distance: ");
    Serial.println(distance);
    Serial.println(" cm");
    
    if (distance <= 10) {
     Porte_o();
    } 
    Porte_f();
  }   
}   

