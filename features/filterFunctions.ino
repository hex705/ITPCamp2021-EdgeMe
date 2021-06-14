
    // ***************  filter types  *****************************

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
      float EMA_weight = 0.1;
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
        // technically tolerance is 2x value passed as its a window above and below newValue
        if ( abs(oldValue - newValue) < tolerance) {
          return oldValue;
        } else {
          return newValue;
        }
    }
