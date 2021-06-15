
let canvas;
let serial;
let sensorReadings;

let sensorValue, sensorAverage, sensorMovingAverage, sensorEMA, sensorMedian, sensorFlatten, min,mid,max;

let featureThreshold, currentBrightness, threshold_state, currentEdge,
edge_state, filteredValue;

let displayString = "default";

let ballScale = 0.15;
let ballColor;
let textColor;

let currentChecked, lastChecked;

let FILTER =  0;
let FEATURE = 1;
let DISPLAY_STATE  = FILTER;


let SONAR = 0;
let RESISTOR = 1;
let SENSOR_TYPE = RESISTOR;



function setup(){

  //colorMode(RGB, 255);

  // font settings
  textAlign(CENTER,CENTER);

  // set background colors -- normal and eStopped
  bgColor = color (200,200,200);
  ballColor = color(0,0,0);
  textColor = color(0,0,0);;

  // canvas
  canvas = createCanvas(600,512);
  canvas.position(10,50);
  canvas.background(bgColor);
  canvas.id('canvas_div');

  // initialize the serialport object:
  serial = new p5.SerialPort();

  serial.on('list', printList);   // callback function for serialport list event
  serial.on('data', serialEvent); // callback function for serialport data event
  serial.list(); // list the serial ports

}



function draw(){

  checkSliderState(); // get display state = resitor or sonar
  if ( DISPLAY_STATE == FILTER ) {
    drawFilter();
  }

  if( DISPLAY_STATE == FEATURE) {
    drawFeature();
  }
  lastChecked = currentChecked;
} // end draw


function checkSliderState(){
  let temp = document.getElementById('theScale');
  currentChecked = temp.checked;
    if (currentChecked){
    //  console.log('checked = sonar');
        SENSOR_TYPE = SONAR;
    } else{
    //  console.log('not checked = resistor');
        SENSOR_TYPE = RESISTOR;
    }

    if ( currentChecked != lastChecked) console.log('scale changed');
}

function drawFilter(){

  if ( SENSOR_TYPE == SONAR ) {
      ballScale = .15; // context specific ::  2 for photcell and pot, 0.15 for sonar
    }

  if ( SENSOR_TYPE == RESISTOR ) {
      ballScale = 2;
  }

    graphicScale = 2;
    background(bgColor);

    stroke(0);fill(0);
    strokeWeight(3);

    line ( 0,height-max/graphicScale,width,height-max/graphicScale );
    line ( 0,height-mid/graphicScale,width,height-mid/graphicScale );
    line ( 0,height-min/graphicScale,width,height-min/graphicScale );

    strokeWeight(1);
    fill(255,0,0);
    ellipse(100, height - sensorValue/ballScale,40,40 );

    fill(0,255,0);
    ellipse(200, height - sensorAverage/ballScale,40,40 );

    fill(255,255,0);
    ellipse(400, height - sensorMovingAverage/ballScale,40,40 );

    fill(255,0,255);
    ellipse(300, height - sensorEMA/ballScale,40,40 );

    fill(0,255,255);
    ellipse(500, height - sensorMedian/ballScale,40,40 );

}


function drawFeature(){

    if ( SENSOR_TYPE == SONAR ) {
        ballScale = .15; // context specific ::  2 for photcell and pot, 0.15 for sonar
      }

    if ( SENSOR_TYPE == RESISTOR ) {
        ballScale = 2;
    }

  // if ( threshold_state == 0) { // dark
  //   bgColor = color(20,20,20);
  //   ballColor = color(255,255,255);
  // } else {
  //   bgColor = color(200,200,200);
  //   ballColor = color(0,0,0);
  // }

  switch(threshold_state){
    case "low(dark) ":    // 0
      displayString = "It's dark !";
      textColor = color(255,0,0);
      bgColor = color(20,20,20);
      ballColor = color(255,255,255);
    // do stuff
    break;

    case "high(bright) ":   // 1
      displayString = "Look at the view!";
        textColor = color(0,0,0);
        bgColor = color(200,200,200);
        ballColor = color(0,0,0);
    // do stuff
    break;
  }

  switch(edge_state){
    case "raising": //2
        displayString = "  Going up";
        textColor = color(255,255,0);
    // do stuff
    break ;

    case "falling": //3
        displayString = "  Coming down again";
        textColor = color(200,0,255);

    // do stuff
    break;

    default:

    break;
  }
  background(bgColor);

  textSize(24);
  fill(textColor);
  textAlign(CENTER,CENTER);
  //console.log(displayString);
  text(displayString,300,450);

  stroke(ballColor);
  fill(ballColor);
  strokeWeight(3);

  line ( 0,height-featureThreshold/graphicScale,width,height-featureThreshold/graphicScale );

      strokeWeight(1);
      fill(ballColor);
      ellipse(100, height - sensorValue/ballScale, 40, 40 );

      fill(ballColor);
      ellipse(500, height - filteredValue/ballScale, 40, 40 );

}

// this function fires when data arrives
// i am using a very crude parser to put
// comma seperated values into an array
// then stuff them into variables

function serialEvent() {
  // read a line of incoming data:
  var inData = serial.readLine();
  // if the line is not empty, parse it to JSON:
  if (inData) {
    //.log(inData);
    sensorReadings = inData.split(",");
  //  console.log(sensorReadings.length);
  //  console.log(sensorReadings);
  //  if ( sensorReadings.length <= 7 ){
  if ( (sensorReadings[3] == "low(dark) ") || (sensorReadings[3] == 'high(bright) ' )){
      //console.log("feature");
    //  console.log(sensorReadings);
      DISPLAY_STATE = FEATURE;
      sensorValue = parseInt(sensorReadings[0]);
      filteredValue = parseInt(sensorReadings[1]);
      featureThreshold = parseInt(sensorReadings[2]);
      threshold_state = (sensorReadings[3]);
      change_state = sensorReadings[4];
      edge_state = sensorReadings[5];



    } else {
      DISPLAY_STATE = FILTER;
      //console.log("filter");
      //let sensorValue , sensorAverage, sensorMovingAverage, sensorEMA, sensorMedian, sensorFlatten, min,mid,max;
      sensorValue = parseInt(sensorReadings[0]);
      sensorAverage = parseInt(sensorReadings[1]);
      sensorMovingAverage = parseInt(sensorReadings[2]);
      sensorEMA = parseInt(sensorReadings[3]);
      sensorMedian = parseInt(sensorReadings[4]);
      sensorFlatten = parseInt(sensorReadings[5]);

      min = parseInt(sensorReadings[6]);
      mid = parseInt(sensorReadings[7]);
      max = parseInt(sensorReadings[8]);
    }



  } // end indata
} // serialEvent

// make a serial port selector object:
function printList(portList) {
  // create a select object:
  portSelector = document.getElementById('portSelector'); // html exists
  console.log(portSelector.length);
  console.log(portList.length);
  // portList is an array of serial port names
  if ( portList.length != portSelector.length){
      for (var i = 0; i < portList.length; i++) {
        // add this port name to the select object:
        if (portSelector[i] != portList[i]){
          var option = document.createElement("option");
          option.text = portList[i];
          portSelector.add(option);
          //console.log(portSelector.length);
        }
      }
   } // end if
  //console.log(portSelector.length);
  // set an event listener for when the port is changed:
  portSelector.addEventListener('change', openPort);
}

function openPort() {
  let item = portSelector.value;
  // if there's a port open, close it:
  if (serial.serialport != null) {
    serial.close();
  }
  // open the new port:
  serial.open(item);
}
