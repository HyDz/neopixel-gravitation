#include <Adafruit_NeoPixel.h>
#include <QueueList.h>

#define NUMBER_OF_LEDS_ON_RING 16
#define NEOPIXEL_RING_DATA_PIN 2
#define ACCELEROMETER_X_OUTPUT 0
#define ACCELEROMETER_Y_OUTPUT 1

Adafruit_NeoPixel neoring = Adafruit_NeoPixel(NUMBER_OF_LEDS_ON_RING, NEOPIXEL_RING_DATA_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  neoring.begin();
  neoring.setBrightness(10);
}

int x, y, led, previousLed;
float nx, ny, angle; 

QueueList <int> ledQueue;

void loop(){

  x = analogRead(ACCELEROMETER_X_OUTPUT);
  y = analogRead(ACCELEROMETER_Y_OUTPUT);
  nx = (x - 375) / 75.0;
  ny = (y - 375) / 75.0;
  angle = atan((ny/nx)) * 180 / 3.14;
  if(angle > 0.0){
    if(nx < 0.0)
      angle += 180;
  } 
  else {
    if(ny > 0.0)
      angle += 180;
    else
      angle += 360;
  }

  if(angle == 360.0)
    angle = 0;

  led = circularize(angle / (360 / NUMBER_OF_LEDS_ON_RING));

  // make led movement smooth
  if(previousLed == led){ 
    // nothing to do 
  }
  else if (counterClockwiseDistanceBetweenLeds(previousLed, led) <= 8)
    led = circularize(previousLed + 1);
  else 
    led = circularize(previousLed - 1);

  ledQueue.push(led);
  makeLightShow();
  previousLed = led;
  delay(25);
}

int circularize(int pos){
  if(pos >= NUMBER_OF_LEDS_ON_RING)
    return(pos - NUMBER_OF_LEDS_ON_RING);
  else if(pos < 0)
    return(pos + NUMBER_OF_LEDS_ON_RING);
  else
    return(pos);
}

int distance;
int counterClockwiseDistanceBetweenLeds(int prevPos, int nextPos){
  distance = nextPos - prevPos;
  if(distance < 0)
    distance += NUMBER_OF_LEDS_ON_RING;
    
  return(distance); 
}

int ledPosition, currentQueueSize;
#define NUMBER_OF_LEDS_TO_SHINE 10
int brightnessStep = 255/NUMBER_OF_LEDS_TO_SHINE;

void makeLightShow(){
  for(int j = 0; j < NUMBER_OF_LEDS_ON_RING; j++)
    neoring.setPixelColor(j, 0, 0, 0);
    
  currentQueueSize = ledQueue.count();
  for(int k = 0; k < currentQueueSize; k++){
    ledPosition = ledQueue.pop();
    neoring.setPixelColor(ledPosition, 0, (brightnessStep * k), 0); 
    if((k == 0 && currentQueueSize < NUMBER_OF_LEDS_TO_SHINE) || k > 0)
      ledQueue.push(ledPosition);
      
  }
  neoring.show();
}



