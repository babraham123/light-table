#include <CapacitiveSensor.h>

/*
 * CapitiveSense Library Demo Sketch
 * Paul Badger 2008
 * Uses a high value resistor e.g. 10 megohm between send pin and receive pin
 * Resistor effects sensitivity, experiment with values, 50 kilohm - 50 megohm. Larger resistor values yield larger sensor values.
 * Receive pin is the sensor pin - try different amounts of foil/metal on this pin
 * Best results are obtained if sensor foil and wire is covered with an insulator such as paper or plastic sheet
 */

CapacitiveSensor cs_5_2 = CapacitiveSensor(5, 2);
// 10 megohm resistor between pins 5 & 2 (sensor)
CapacitiveSensor cs_5_6 = CapacitiveSensor(5, 6);
// 10 megohm resistor between pins 5 & 6 (sensor)
CapacitiveSensor cs_5_8 = CapacitiveSensor(5, 8);
// 10 megohm resistor between pins 5 & 8 (sensor)
CapacitiveSensor cs_5_9 = CapacitiveSensor(5, 9);
// 10 megohm resistor between pins 5 & 9 (sensor)

void setup() {
   // cs_5_2.set_CS_AutocaL_Millis(0xFFFFFFFF);
   // cs_5_6.set_CS_AutocaL_Millis(0xFFFFFFFF);
   // cs_5_8.set_CS_AutocaL_Millis(0xFFFFFFFF);
   // cs_5_9.set_CS_AutocaL_Millis(0xFFFFFFFF);
   // turn off autocalibrate of baseline capacitance
   Serial.begin(9600);
}

void loop() {
    long start = millis();
    long total1 =  cs_5_2.capacitiveSensorRaw(50);
    long total2 =  cs_5_6.capacitiveSensorRaw(50);
    long total3 =  cs_5_8.capacitiveSensorRaw(50);
    long total4 =  cs_5_9.capacitiveSensorRaw(50);

    long average = (total1+total2+total3+total4)/4;
    total1 = total1 - average;
    total2 = total2 - average;
    total3 = total3 - average;
    total4 = total4 - average;
    
    Serial.print(total1);
    Serial.print("\t");
    Serial.print(total2);
    Serial.print("\t");
    Serial.print(total3);
    Serial.print("\t");
    Serial.print(total4);
    Serial.println(" end");

    delay(50);
    // arbitrary delay to limit data to serial port 
}

