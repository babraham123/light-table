#include <CapacitiveSensor.h>

/*
 * CapitiveSense Library Demo Sketch
 * Paul Badger 2008
 * Uses a high value resistor e.g. 10 megohm between send pin and receive pin
 * Resistor effects sensitivity, experiment with values, 50 kilohm - 50 megohm. Larger resistor values yield larger sensor values.
 * Receive pin is the sensor pin - try different amounts of foil/metal on this pin
 * Best results are obtained if sensor foil and wire is covered with an insulator such as paper or plastic sheet
 */


CapacitiveSensor   cs_4_2 = CapacitiveSensor(4,2);
// 10 megohm resistor between pins 4 & 2 (sensor)
CapacitiveSensor   cs_4_5 = CapacitiveSensor(4,5);
// 10 megohm resistor between pins 4 & 6 (sensor)
CapacitiveSensor   cs_4_8 = CapacitiveSensor(4,8);
// 10 megohm resistor between pins 4 & 8 (sensor)

void setup()                    
{

   cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);
   // turn off autocalibrate on channel 1 - just as an example
   Serial.begin(9600);

}

void loop()                    
{
    long start = millis();
    long total1 =  cs_4_2.capacitiveSensor(30);
    long total2 =  cs_4_5.capacitiveSensor(30);
    long total3 =  cs_4_8.capacitiveSensor(30);

    Serial.print(total1);
    Serial.print("\t");
    Serial.print(total2);
    Serial.print("\t");
    Serial.println(total3);

    delay(10);
    // arbitrary delay to limit data to serial port 
}

