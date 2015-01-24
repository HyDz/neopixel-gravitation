#include <Adafruit_NeoPixel.h>

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

  for(int j = 0; j < NUMBER_OF_LEDS_ON_RING; j++)
    neoring.setPixelColor(j, 0, 0, 0);

  neoring.setPixelColor(circularize(led - 1), 30, 60, 30); 
  neoring.setPixelColor(circularize(led + 1), 30, 60, 30); 
  neoring.setPixelColor(led, 100, 255, 100); 
  neoring.show();

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





