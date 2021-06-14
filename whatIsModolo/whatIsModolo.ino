int count = 0;
int idx;
int divide;

int modVal = 6;


void setup(){
   Serial.begin(9600);
   while(!Serial);
 } // end setup

void loop(){

   modVal = 3;
   idx = count % modVal; // must be 2 or bigger

   printSimple();

     // do you want to see how mod and divide relate?
     // then uncomment 2 below and comment out printSimple() above
        // divide = count/modVal;
        // printVerbose();

   count++;
   delay(500);
} //end loop



void printSimple(){
   if (count == 0 ) Serial.println("Count\tIdx");
    Serial.print(count);
    Serial.print("\t");
    Serial.println(idx);
}

void printVerbose(){
  Serial.print(count);
  Serial.print("\tdivided by ");
  Serial.print(modVal);
  Serial.print(" = ");
  Serial.print(divide);
  Serial.print(" with %= ");
  Serial.print(idx);
  Serial.println(" (remainder.)");
}
