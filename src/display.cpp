#include "appGlobals.h"
#include <SPI.h>
#include "TFT_eSPI.h"

// Initialize the TFT screen

static const uint16_t screenWidth  = 240;
static const uint16_t screenHeight = 240;

TFT_eSPI display = TFT_eSPI(screenWidth, screenHeight);

// Your camera configuration here
camera_config_t config;

static void displayImageTask(void * pvParameters) {
  while (true) {
    
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      continue;
    }

    // Decode JPEG images
    uint8_t* buf = fb->buf;
    uint32_t len = fb->len;
    Serial.println("Image size: " + String(len) + " bytes");

    display.startWrite();
    display.setAddrWindow(0, 0, screenWidth, screenHeight);
    display.pushColors(buf, len);
    display.endWrite();
    Serial.println("Displaying image");

    esp_camera_fb_return(fb);    
    Serial.println("Release frame buffer");

    // Delay for a bit before capturing the next frame
    vTaskDelay(2000 / portTICK_PERIOD_MS); // Delay for 100ms
  }
  vTaskDelete(NULL);
}

void prepDisplay() {
    // Initialize the TFT screen
    display.init();
    display.setRotation(1);
    display.fillScreen(TFT_ORANGE);
    delay(1000);
    display.fillScreen(TFT_BLACK);
    delay(1000);
    display.fillScreen(TFT_BLUE);
}

void startDisplay() {
// Create the display image task
    prepDisplay();

  xTaskCreate(
    &displayImageTask,   // Task function
    "DisplayImageTask", // Task name
    DISPLAY_STACK_SIZE,              // Stack size (this might need to be adjusted)
    NULL,               // Task input parameter
    DISPLAY_PRI,                  // Task priority
    NULL                // Task handle
  );
}