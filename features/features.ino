// feature detection examples

// feature examples
// developed by steve.daniels@ryerson.ca
// developed for ITP Camp 2021
// created June 2021

/*
  demo of :
      threshold detection
      change detection
      rasing / falling edge detection 
*/

// possibly helpful links

// format serial plotter -- it doesn't work for me -- YMMV -- ? windows only ?
// https://diyrobocars.com/2020/05/04/arduino-serial-plotter-the-missing-manual/

// median filter library source -- if required
// https://github.com/daPhoosa/MedianFilter


#include "filterVariables.h" // all the filter variables from before in a seperate file


// get started -- set up your sensor

/* 
   i am assuming you have worked your way through the filter.ino example
   if your sensor is not analogRead compatible -- replace following line and contents of
   getSingleSensorReading() -- now found in the filterFunctions tab
*/

int sensorPin = A0; // analog input pin for your sensor

// timer variables
unsigned long timeNow = 0;
unsigned long lastSensorReadTime = 0;
unsigned int  sensorReadInterval = 20;


// sensor reading storage -- for determing features
int newFilteredValue = -1;
int oldFilteredValue = -1;

// threshold variables
  int threshold;
  int newThresholdState = -1;
  int oldThresholdState = -1;

// change variables
  int newChange = -1;

// edge variables
  int newEdgeState = -1;


void setup(){
  pinMode (sensorPin, INPUT); // not actually needed for analog in -- but habit
  Serial.begin(9600);         // default (slow)
  while(!Serial);
}

void loop(){

  timeNow = millis();

  // timer -- because delay blocks -- https://www.youtube.com/watch?v=BYKQ9rk0FEQ
  if (( timeNow - lastSensorReadTime ) > sensorReadInterval ){
    newFilteredValue = getNewReadingAndFilterIt();
    lastSensorReadTime = timeNow; // reset timer
  }

  // threshold  *******************
        // define the threshold -- look at your trace -- every context is different
        threshold = 500;
            
        // are we above (1) or below (0) the  threshold ?
        newThresholdState = checkThreshold (newFilteredValue, threshold);
        
  // change detection  *******************
        // did we change sides of the threshold? 
        newChange = checkForChange( newThresholdState, oldThresholdState);
      
  // edge detection  *******************
        // are we raising (1) or falling(0)   
        //newEdgeState = checkEdges (newThresholdState, oldThresholdState);
        //newEdgeState = checkEdges (newFilteredValue, oldFilteredValue);

  showFeatures(); // shows readings 

  // store new in old for comparison in the next loop
  oldThresholdState = newThresholdState;
  oldFilteredValue  = newFilteredValue;

} // end loop


void showFeatures(){

 
    Serial.print(sensorValue); // a string
    Serial.print(',');
   
    Serial.print(newFilteredValue); // a string
    Serial.print(',');

    Serial.print(threshold); // a number
    Serial.print(',');

    // threshold *****************
        if ( newThresholdState == 0) {
            Serial.print("low(dark) ");
        } else {
            Serial.print("high(bright) ");
        } // end threshold
        Serial.print(',');


    // change *****************
        if ( newChange == 1) {
           Serial.print("changed "); 
        } // end change
        Serial.print(','); 

 
    // edge *****************
        if ( newEdgeState == 0 ) {
            Serial.print("falling");
        } else if (newEdgeState == 1) {
            Serial.print("raising");
        } // end edge
        Serial.print(',');

    Serial.println();

}//  end showFeatures;


// void featuresToJS(){
//
//   // for Serial Monitor -- does not impact logic
//   // note - meaning of crossing threshold is sensor dependant (light, distance, sound)
//   if ( THRESHOLD_STATE == 0){
//     currentBrightness = "dark";
//   } else {
//     currentBrightness = "light";
//   }
//
//   // for Serial Monitor -- does not impact logic
//   // note - meaning of crossing threshold is sensor dependant (light, distance, sound)
//   switch (NEW_EDGE_STATE){
//     case EDGE_LOW:    // 0
//       currentEdge = "LOW";
//       // do stuff
//     break;
//
//     case EDGE_HIGH:   // 1
//       currentEdge = "HIGH";
//         // do stuff
//     break;
//
//     case EDGE_RAISING: //2
//       currentEdge = "RAISING";
//         // do stuff
//     break ;
//
//     case EDGE_FALLING: //3
//       currentEdge = "FALLING";
//         // do stuff
//     break;
//   } // end switch
//
//
//   // THRESHOLD
//       Serial.print(threshold); // a number
//       Serial.print(',');
//
//       Serial.print(currentBrightness); // a string
//       Serial.print(',');
//
//       // scaleUp for plotter
//       THRESHOLD_STATE = ( THRESHOLD_STATE * 100 ) + 100; // map 0,1 to 100,200
//       Serial.print(THRESHOLD_STATE);
//       Serial.print(',');
//
//   // EDGES
//       Serial.print(currentEdge); // string
//       Serial.print(',');
//
//       Serial.print(EDGE_STATE); // number
//       Serial.print(',');
//
//
//   // READINGS -- show to help keep track of things
//   Serial.print(sensorValue);
//   Serial.print(',');
//
//   Serial.print(filteredValue);
//   Serial.print(',');
//
//   Serial.println();
// }

//
//void showFeaturesMonitor(){
//  
//  // THRESHOLD
//      Serial.print(currentBrightness); // a string
//      Serial.print(',');
//
//  // EDGES
//      Serial.print(currentEdge); // string
//      Serial.print(',');
//
//  Serial.println();
//}


// check if reading is above or below thresold
int checkThreshold(int valueToCheck, int threshold){
  // simple compare of sensor value to fix value
  int compare;

  if (valueToCheck > threshold) compare = 1; // above
  if (valueToCheck < threshold) compare = 0; // below

  return compare;
}

// check for change in variable of interest
int checkForChange (int newValue, int oldValue){
   int changed = 0; // no change
    if (newValue != oldValue) {
      changed = 1;  // changed
    }
    return changed;
}

// check for edge type 
int checkEdges (int newValue, int oldValue){
    int tempEdge = -1;
    if (newValue > oldValue) tempEdge = 1; // raising
    if (newValue < oldValue) tempEdge = 0; // falling
    
    return tempEdge;
}




int getNewReadingAndFilterIt (){
  // take reading and filter it
  sensorValue = getSingleSensorReading(); // simple read
  //sensorMovingAverage = getMovingAverage(sensorValue);  
  sensorMedian = getMedian(sensorValue);                    
  sensorFlatten = getFlattener(sensorMedian, oldFilteredValue, 5 ); // last number is tolerance

  // store the new filtered value
  return sensorFlatten;
}
