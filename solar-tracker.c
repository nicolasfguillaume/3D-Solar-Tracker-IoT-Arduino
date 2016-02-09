#include <Servo.h> 
#include <LowPower.h>

Servo myservo;

// Define constants
const int LDR1Pin = 0;      // define a pin for Photo resistor
const int LDR2Pin = 1;      // define a pin for Photo resistor
const int TempPin = 2;      // define a pin for Temperature sensor
const int servoPin = 9;     // define a pin for servo
const int batteryPin = 3;   // +V from battery is connected to analog pin 3
const int pvPin = 4;        // +V from pv is connected to analog pin 4

// Battery and PV monitoring
const int R1 = 10000; // value for a maximum voltage of 10 volts
const int R2 = 10000;

const int epsilon = 10;      // define tolerance (*****) -- 3 works well at noon when sunny (and on USB)
int difference = 0;        // difference between value_right and value_left // to be declared in the header only
int angle = 0;             // define angle for servo
int servo_step = 2;        // define step for servo (*****) 
long prog_speed = 200;       // define delay (ms) between two loop of the arduino program (*****)        was 200
long prog_break = 200;     // define idle time (ms) for power management -- was 8000
int angle_ref = 110;        // angle of horizontal array ; Vilros tag facing the sky

// Define BT variables
long previousMillis = 0;         // will store last time data was sent
long interval = 8000;           // interval at which to send data (ms)

// Define running average variables
const int numReadings = 6;     // number of sample for the running average (*****)     
int readings[numReadings][5];      // the readings from the 6 analog inputs
int index = 0;                  // the index of the current reading
int total[5] = { 0 };           // the running total
int average_diff = 0;           // the average of the difference

long readVcc() {
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = (1100L * 1024L) / result; // Back-calculate AVcc in mV
  return result;
}

void setup() 
{                 
  angle = angle_ref;
  myservo.attach(servoPin);
  myservo.write(angle);         // set servo to mid-point -- (panels are horizontal)
    
  pinMode(LDR1Pin, INPUT);
  pinMode(LDR2Pin, INPUT);
  pinMode(TempPin, INPUT);
  pinMode(batteryPin, INPUT);
  pinMode(pvPin, INPUT);
  Serial.begin(9600);           //Begin serial communcation
 
   // initialize all the readings for the 5 running averages: 
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
      {
       for (int j = 0; j <= 5 ; j++)
         readings[thisReading][j] = 0; 
      }
} 

void loop() 
{
  //Calculate the runnning average
  //...for the light comparator
  total[0] -= readings[index][0];         // subtract the last reading:
  readings[index][0] = analogRead(LDR2Pin) - analogRead(LDR1Pin); // read the difference from the sensor: 
  total[0] += readings[index][0];         // add the reading to the total:
  //...the same for Vcc
  total[1] -= readings[index][1];         
  readings[index][1] = readVcc();                      
  total[1] += readings[index][1];       
  //...the same for Batt                     
  total[2] -= readings[index][2];         
  readings[index][2] = analogRead(batteryPin);          
  total[2] += readings[index][2];         
    //...the same for PV
  total[3] -= readings[index][3];    
  readings[index][3] = analogRead(pvPin);             
  total[3] += readings[index][3];  
  //...the same for Temp
  total[4] -= readings[index][4];         
  readings[index][4] = analogRead(TempPin);           
  total[4] += readings[index][4];  
  //...the same for A5
  //total[5] -= readings[index][5];           //ATTENTION: using the last element of the array
  //readings[index][5] = 0;                   //will mess up the calculation of the other running averages
  //total[5] += readings[index][5];           //especially average_diff 
  //...
  index += 1;                             // advance to the next position in the array:                
  if (index >= numReadings)               // at the end of the array, wrap around to the beginning:   
    index = 0;                           
  average_diff = total[0] / numReadings;     // return the runnning average of the difference
  Serial.print(average_diff ); Serial.println(" average_diff ");
  float ref_voltage = (total[1] / numReadings) / 1000.0;       // return the runnning average of Vcc
  float average_batt = total[2] / numReadings;     // return the runnning average of batt
  float average_pv = total[3] / numReadings;     // return the runnning average of batt
  //float average_temp = total[4] / numReadings;     // return the runnning average of temp
    
  if ((abs(average_diff) >= epsilon) || (abs(-average_diff) >= epsilon))
    {
       if (average_diff > 0)
           {
             angle += servo_step;
             if(angle < 148) { myservo.write(angle); }
             else            { angle = 148; }      // low limit  --   high number is lower -- modify the design of structure to allow lower
           }
        else  
           {
             angle -= servo_step;
             if(angle > 53) { myservo.write(angle); }
             else          { angle = 53; }         // high limit -- small number is higher 
           }
       prog_speed = 200;    // faster response to a light change
    }
    else
    {
       prog_speed = prog_break;    // slower response if no major light change
    }

   //Update temperature
    //float voltage_temp = average_temp * ref_voltage / 1024.0;
    //float temperatureC = (voltage_temp - 0.5) * 100 ;  //converting from 10 mv per degree with 500 mV offset

   //Update battery capacity %
    float voltage_batt = average_batt * ref_voltage * 2.0 / 1024.0; //    /(R2/(R1 + R2)) 
    float battery_capa = (voltage_batt - 3.0) * (100 - 0) / (4.2 - 3.0);

   //Update PV voltage
    float voltage_pv = average_pv * ref_voltage * 2.0 / 1024.0; //    /(R2/(R1 + R2)) 
    float power_pv = voltage_pv * 0.150 * 5;
    
    //Correct angle for display
    float angle_corr = angle * (148 - 110) / 45 - 100 * (148 - 110) / 45;

   //Datalogging over bluetooth
   unsigned long currentMillis = millis();
   unsigned long currentTime = millis() / 1000.0;
   if(currentMillis - previousMillis > interval) 
   {
    previousMillis = currentMillis;   
    Serial.print("Time = "); Serial.print(millis()/1000.0); Serial.print(" s       |  ");
    Serial.print("Array angle = "); Serial.print(angle_corr); Serial.println(" deg");
    //Serial.print("Temperature = "); Serial.print(temperatureC); Serial.println(" deg C");
    Serial.print("Battery SOC = "); Serial.print( (int) battery_capa); Serial.print("%  |  ");
    Serial.print("V batt = "); Serial.print(voltage_batt); Serial.println(" V");
    Serial.print("V pv = "); Serial.print(voltage_pv); Serial.print(" V        |  ");
    Serial.print("P pv (estimate) = "); Serial.print(power_pv); Serial.println(" W");
    Serial.print("Vcc = "); Serial.print(ref_voltage); Serial.println(" V");
    Serial.println();
  }
  
   // Power Management
   if (prog_speed == 200)
   {
      delay(prog_speed); //short delay for faster response to light
      //LowPower.powerDown(SLEEP_60MS, ADC_OFF, BOD_OFF);  // sleep for 15, 30 or 60 msec
   }
   else if (currentTime > 10)                  // slower response if no major light change
   {
      Serial.print("Sleep for 8 sec...");
      delay(50);
      LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);  // sleep for 1, 2, 4 or 8 sec
      //delay(prog_break);
      Serial.println(" Waking up!"); Serial.println("");
      prog_speed == 200 ;  // back to a faster response
   }
