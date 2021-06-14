// filter exmaples
// developed by steve.daniels@ryerson.ca
// developed for ITP Camp 2021
// created June 2021

/*
  demo of :
    simple analog readings -- it all starts somewhere
    simple averaging of readings -- removes noise, obtain background data
    moving average of readings -- removes noise
    exponential moving average of readings -- removes noise
    median filtering -- removes spikes
    flattening -- removes small ripples / flicker
*/

// possibly helpful links

// format serial plotter -- it doesn't work for me -- YMMV -- ? windows only ?
// https://diyrobocars.com/2020/05/04/arduino-serial-plotter-the-missing-manual/

// median filter library source
// https://github.com/daPhoosa/MedianFilter


// ************* get started **************************

// I am assuming you have a sensor that uses analog read
// if you have a different kind of sensor -- set it up here
// comment out this line if not using it

// if your sensor uses an alt method (SPI or I2C) scroll way down to
//  int getSingleSensorReading(){...} and change that function too.

int sensorPin = A0; // analog input pin for your sensor


// ************* filter variables **************************
// each filter type needs specific variables
// we will uncomment as we move through them

// single raw - variables
    int sensorValue = -1;
    int oldSensorValue= -1;

// simple average - variables
    int sensorSimpleAverage = -1;
    int numberOfMeasuresToAverage = 16;

// moving average - variables
    int sensorMovingAverage = -1;
    const int  windowSize = 16; // needs to be const for array
    int  ma_readings[windowSize]; // array to hold readings
    int  sum, INDEX;

// EMA - Exponential Moving Average - variables
    int sensorEMA=-1;
    float EMA_weight = 0.5;
    float EMA;

// median variables and set up
    #include <MedianFilter.h>
    int sensorMedian = -1;
    const int medianWindowSize = 16;
    const int seed = 0; // seed could = expected value (how to get that?)
    MedianFilter median_readings(medianWindowSize,seed);

// flatten variables
    int sensorFlatten = -1;
    int oldSensorAverage=-1;

// thresholds -- for display -- features ?
    int low = 250, mid = 500, high = 750;


void setup(){
  pinMode (sensorPin, INPUT); // not actually needed for analog in -- but habit
  Serial.begin(9600); // default (slow)
  while(!Serial);
}


void loop(){

  sensorValue = getSingleSensorReading(); // simple read
  sensorSimpleAverage = getSimpleAverage(numberOfMeasuresToAverage);  // get average of 8 reads
  sensorMovingAverage = getMovingAverage(sensorValue); // using currentReading
  sensorEMA = getEMA(sensorValue); // EMA = exponential moving average
  sensorMedian = getMedian(sensorValue);
  sensorFlatten = getFlattener(sensorValue, oldSensorValue, 5 ); // last number is tolerance

  showFilterResults();

  delay(50); // slows loop to 20x / second

  // store readings from this loop for later
    oldSensorValue = sensorValue;
    //oldSensorAverage = sensorAverage;
}

void showFilterResults(){
  // this code block set us data for the plotter

  // plotter needs data to be comma or tab delimited
  // every println updates plotter with data
  // so, to plot multiple points:
        //  use print, then comma or tab, finally println()

  // other details
  // https://diyrobocars.com/2020/05/04/arduino-serial-plotter-the-missing-manual/

  Serial.print(sensorValue);
  Serial.print(',');

  Serial.print(sensorSimpleAverage);
  Serial.print(',');

  Serial.print(sensorMovingAverage);
  Serial.print(',');

  Serial.print(sensorEMA);
  Serial.print(',');

  Serial.print(sensorMedian);
  Serial.print(',');

  Serial.print(sensorFlatten);
  Serial.print(',');

  // threshold labels
    Serial.print(low);
    Serial.print(',');

    Serial.print(mid);
    Serial.print(',');

    Serial.print(high);
    Serial.print(',');

  Serial.println();
}

// filter types

// basic sensor reading -- you may need to customize this
// to match your sensor
int getSingleSensorReading(){
  // if
  //  you have a sensor that needs other ways of getting data
  //  such as I2C, SPI, library interface,
  // then
  //  replace line below with code for your sensor
  int theReading = analogRead( sensorPin );

  // send the new reading back
  return theReading;
}



/* SMA -- SIMPLE AVERAGE */
// simple average take a burst of readings at one time
// and returns that average as a single value
// average = ( sum of items ) / ( number of items)
int getSimpleAverage (int numberOfReadings){
  int tempSum = 0;
  for (int i = 0; i < numberOfReadings; i ++ ){
    tempSum += getSingleSensorReading(); // note += this is adding
  }
  return tempSum/numberOfReadings; // send this back to call
}



/* MA -- MOVING AVERAGE
  ++ advantage -- works on recent data only ( window size dependant )
      -- in fact last windowSize number of measures
  ++ integer math

  -- if window gets too big this lags
  -- needs windowSize memory set aside
*/

int getMovingAverage ( int newReading ){
  //https://maker.pro/arduino/tutorial/how-to-clean-up-noisy-sensor-data-with-a-moving-average-filter
   sum = sum - ma_readings[INDEX];      // Remove the oldest entry from the sum
   ma_readings[INDEX] = newReading;     // Add the newest reading to the window
   sum = sum + newReading;              // Add the newest reading to the sum
   INDEX = (INDEX+1) % windowSize;      // Increment the Index, and wrap to 0 if it exceeds the window size - ferris wheel style!

  return  sum / windowSize;
}



/* EMA -- EXPONENTIAL MOVING AVERAGE
    Now we apply an exponential moving average
    A what ? -- https://hackaday.com/2019/09/06/sensor-filters-for-coders/

  ++ advantage -- simple ! one line of code
  ++ advantage -- NO ARRAY, NO LOOP -- less memory, less time
  ++ advantage -- you can set sensitivity !

  -- uses floats -- but there are work arounds for this

  NOTE -- extremely small weights (<1.0) will have slow response

    variable EMA_weight is a sliding scale from 0 to 1 (like a teeter-totter)
    EMA  = (some proportion of the newTestValue) + ( some proportion of the MOVING AVERAGE)
    If EMA_weight is close to ZERO (0) -- then OLD readings are more important than NEW ones
    IF EMA_weight is close to ONE  (1) -- then NEW readings are more important than OLD ones
*/

int getEMA ( int newReading ) {
  float EMA_weight = 0.2;
  EMA = (EMA_weight * newReading) + (( 1.0-EMA_weight ) * EMA);
  return int(EMA); // toss the decimal bits, return value

}

/*  median filter -- interface is library dependant */
// get source code here
// https://github.com/daPhoosa/MedianFilter
// install Arduino--> Sketch--> Include Library--> Add .ZIP library

int getMedian ( int newReading ) {
    median_readings.in(newReading); // adds new reading to window
    int temp = median_readings.out();
    return temp; // gets the current median
}

/* flatten tiny ripples */
// probably has a better name

int getFlattener ( int newValue, int oldValue, int tolerance ) {
  // technically tolerance is 2X value passed as its a window above and below newValue
  if ( abs(oldValue - newValue) < tolerance) newValue = oldValue;
  return newValue;

}
