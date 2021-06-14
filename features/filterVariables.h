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
  //  int low = 250, mid = 500, high = 750;
