#include <Arduino.h>
#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp>

// --- MOTOR HIZ AYARLARI ---
// Sol taraf zayıf olduğu için ona maksimum güç veriyoruz, sağ tarafı kısıyoruz.
const int solMotorHizi = 255; // Maksimum güç (0-255 arası)
const int sagMotorHizi = 170; // Sağ tarafı yavaşlattık ki sol taraf yetişebilsin

// Motor Pinleri
const int leftSpeed = 6; 
const int left1 = 7; 
const int left2 = 8;
const int rightSpeed = 9;
const int right1 = 10;
const int right2 = 11;

// Dönüş Süresi
const int turnTime = 100; 

void setup() {
    Serial.begin(115200);
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

    Serial.print(F("Ready to receive IR signals of protocols: "));
    printActiveIRProtocols(&Serial);
    Serial.print(F("at pin "));
    Serial.println(IR_RECEIVE_PIN);

    pinMode(right1, OUTPUT);
    pinMode(right2, OUTPUT);
    pinMode(rightSpeed, OUTPUT);
    pinMode(left1, OUTPUT);
    pinMode(left2, OUTPUT);
    pinMode(leftSpeed, OUTPUT);
    
    // --- HIZLARI BURADA AYARLIYORUZ ---
    analogWrite(leftSpeed, solMotorHizi);   // Sol motor MAX güçte (255)
    analogWrite(rightSpeed, sagMotorHizi);  // Sağ motor KISIK güçte (170)
}

void loop() {
    if (IrReceiver.decode()) {
        uint16_t command = IrReceiver.decodedIRData.command;

        Serial.print("Gelen Komut: 0x");
        Serial.println(command, HEX);

        // --- KONTROL ---
        if (command == 0x18) { // İleri
            Serial.println("Action: Ileri");
            digitalWrite(right1, HIGH);
            digitalWrite(right2, LOW);
            digitalWrite(left1, HIGH);
            digitalWrite(left2, LOW);
            
            // İleri giderken 9 birim gidip duracak
            delay(turnTime * 9); 
            stopCar();
        } 
        else if (command == 0x52) { // Geri (Kodunu 0x52 olarak güncellemişsin, doğru mu kontrol et)
            Serial.println("Action: Geri");
            digitalWrite(right1, LOW);
            digitalWrite(right2, HIGH);
            digitalWrite(left1, LOW);
            digitalWrite(left2, HIGH);
            
            delay(turnTime * 9);
            stopCar();
        } 
        else if (command == 0x5A) { // Sağ
            Serial.println("Action: Sag");
            digitalWrite(right1, LOW);
            digitalWrite(right2, HIGH);
            digitalWrite(left1, HIGH);
            digitalWrite(left2, LOW);
            delay(turnTime);
            stopCar(); 
        } 
        else if (command == 0x8) { // Sol
            Serial.println("Action: Sol");
            digitalWrite(right1, HIGH);
            digitalWrite(right2, LOW);
            digitalWrite(left1, LOW);
            digitalWrite(left2, HIGH);
            delay(turnTime*9);
            stopCar();
        } 
        else if (command == 0x56) { // Dur
            Serial.println("Action: DUR");
            stopCar();
        }

        IrReceiver.resume(); 
    }
}

void stopCar(){
    digitalWrite(right1, LOW);
    digitalWrite(right2, LOW);
    digitalWrite(left1, LOW);
    digitalWrite(left2, LOW);  
}