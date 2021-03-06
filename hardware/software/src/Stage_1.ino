/*
 * take a 1 second clock input, service it in an interrupt routune. Count 30secs and generate a 1/2 minute 
 * pluse
 * # Robertson
Control code for Robertson Clock at Bristol University



Place holder information and questions.

From the circuit diagram of the Arduino shield <TODO - Attach document >  the following Arduino Due pins are defined:

ClkInPin       = 2; // input clock pin. rising endge pulse for 250mS, 1Hz
ClkInRefPin    = 3; // input clock reference for comparison with the input clock and subsequent adjustment
HalfMinOutPin  = 4; // out pulse every 30s aligned with the input clock. Pulse of 250mS duration
Unknown1       = 5; // <TODO  - check what the function of this pin is >
Unknown2       = 6  // <TODO  - check what the function of this pin is >
PllStep0OutPin = 7; // Drive a stepper motor to apply timing correction to the clock <TODO check stepper motor drive type
PllStep1OutPin = 8; // Drive a stepper motor to apply timing correction to the clock <TODO check stepper motor drive type
Step0OutPin    = 9; // <TODO - check function >
Step1OutPin    = 10;// <TODO - check function >

LED_BUILTIN    = 13;// Standard Arduino LED

NOTES:
the example code given here will be built up in simple stages to demonstrate some arduino and programming concepts.

Stage 1: Receive an input clock (for simplicity, this clock will be generated by the Due itself and will require a wired connection from pin 13 to pin 2). Generate an interrupt on the rising edge. Assume no debounce is required for this stage. In this example we will also generate the same interrupt on the falling edge and we will use this to time onther pulses.

The ISR will generate the 1/2 minute pulse by counting 30 seconds
*/
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
Adafruit_BME280 bme; // I2C

const int ClkInPin       = 2;
const int ClkInRefPin    = 3;
const int HalfMinOutPin  = 4;
const int Unknown1       = 5;
const int Unknown2       = 6
const int PllStep0OutPin = 7;
const int PllStep1OutPin = 8;
const int Step0OutPin    = 9;
const int Step1OutPin    = 10;

int inClk = 0;
int secondCount = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output. Use this to simultate the input clock
  pinMode(LED_BUILTIN, OUTPUT);

  // the input clock is fed into ClkPin so set it as an input
  pinMode(ClkPinIn, INPUT_PULLUP); 
  // Set this pin up to generate an interrput in the clk rising edge. Run the 'TickingISR' on interrupt
  attachInterrupt(digitalPinToInterrupt(ClkPinIn), tickingISR, RISING);

  pinMode(ClkInRefPin, INPUT_PULLUP);
  pinMode(SecOutPIn, OUTPUT);
  pinMode(PllStep0OutPin, OUTPUT);
  pinMode(PllStep1OutPin, OUTPUT);  
  pinMode(Step0OutPin, OUTPUT);  
  pinMode(Step1OutPin, OUTPUT);
    
  // setup serial port for debug an later functionality
  Serial.begin(9600);

  Serial.println(F("BME280 test"));

  bool status;
    
  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  status = bme.begin();  
  if (!status) {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
      while (1);
  }
}

void tickingISR() {
  // got her because of the clock interrupt

  // read the pin (migh need it latter
  inClk = digitalRead(ClkPinIn);
  
  // Generate a 30s output
  secondCount = (secondCount + 1) % 30;
  Serial.println(secondCount);
}

// the loop function runs over and over again forever
void loop() {
  //this is for conveniance. Normaly the clock would come from outside. Generate a 1 second pulse
  // and pyshically connect this pin to the ClkPin input 
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(500);                       // wait for a second
  
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");
 }
