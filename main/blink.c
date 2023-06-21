#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

const int LED_GPIO[] = {15, 2, 5, 19, 21};
const int NUM_LEDS = 5;
#define SWITCH_PIN 23 

void setup() {
  gpio_pad_select_gpio(SWITCH_PIN);
  gpio_set_direction(SWITCH_PIN, GPIO_MODE_INPUT);
  gpio_set_pull_mode(SWITCH_PIN, GPIO_PULLUP_ONLY); //set switch

  for (int i = 0; i < NUM_LEDS; i++) {
    gpio_pad_select_gpio(LED_GPIO[i]);
    gpio_reset_pin(LED_GPIO[i]);
    gpio_set_direction(LED_GPIO[i], GPIO_MODE_OUTPUT); //set leds
  } 
}
void blinkLED(int pin, int onDelay, int offDelay) { 
  gpio_set_level(pin, 1);
  vTaskDelay(onDelay / portTICK_PERIOD_MS);

  gpio_set_level(pin, 0);
  vTaskDelay(offDelay / portTICK_PERIOD_MS);
}
void ledOn(int pin){ //seting led on state
  gpio_set_level(pin, 1);
}
void ledOff(int pin){ //seting led off state
  gpio_set_level(pin, 0);
}
void chasing(int onDelay, int offDelay){
  for (int i = 0; i < NUM_LEDS; i++) {
    for(int j =i; j<NUM_LEDS; j++){
      blinkLED(LED_GPIO[j], onDelay, offDelay);
    }
    ledOn(LED_GPIO[i]);

  }
  for(int k = 0; k < NUM_LEDS; k++){
    ledOff(LED_GPIO[k]);
    vTaskDelay(onDelay / portTICK_PERIOD_MS);
  }
}
void running(int onDelay, int offDelay){
  for(int j=0; j<NUM_LEDS; j++){
    blinkLED(LED_GPIO[j], onDelay, offDelay);
  }
}
void snake(int onDelay, int offDelay){
  for(int j=0; j<NUM_LEDS; j++){
    ledOn(LED_GPIO[j]);
    vTaskDelay(onDelay / portTICK_PERIOD_MS);
  }
  for(int i=0; i<NUM_LEDS; i++){
    ledOff(LED_GPIO[i]);
    vTaskDelay(onDelay / portTICK_PERIOD_MS);
  }
}
void strobe(int onDelay, int offDelay){
  for(int i=1; i<NUM_LEDS-1; i++){
    blinkLED(LED_GPIO[i-1], (onDelay/4), (offDelay/2));
    blinkLED(LED_GPIO[i+1], (onDelay/4), (offDelay/2));
  }
}

static int counter = 0;
void modeSelector(){
  int onDelay = 400;
  int offDelay = 100;
  
  switch (counter) {
    case 1:
      printf("strobe light \n");
    for(int i=0;i<10;i++){
      strobe(onDelay, offDelay);
    }
      break;
    case 2:
      printf("running light \n");
    for(int i=0;i<3;i++){
      running(onDelay, offDelay);
    }
      break;
    case 3:
      printf("running snake \n");
    for(int i=0;i<3;i++){
      snake(onDelay, offDelay);
    }
      break;
    case 4:
      printf("filling light \n");
      chasing(onDelay, offDelay);
      break;
    default:
      counter = 0; // Reset the counter when all functions have been executed
      break;
  }
}
void clicklistner(){
  int switchState = gpio_get_level(SWITCH_PIN); // onclick 0 eotherwise 1
  static int lastState = 1;
  if (switchState != lastState) { // only state change affects
    if (switchState == 0) {
      counter++;
      modeSelector();
    }
    lastState = switchState;  
  }
}

void app_main() {
  setup();

  while (1) {
    clicklistner();
    vTaskDelay(pdMS_TO_TICKS(100));
  }
} 



