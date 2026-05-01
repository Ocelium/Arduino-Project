#include "esp_camera.h"
#include "SD_MMC.h"
#include "FS.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

// Camera pins (AI Thinker)
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

#define FLASH_PIN 4

bool cameraOK = false;
int photoCount = 0;

// Flash LED function
void flashLED(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(FLASH_PIN, HIGH);
    delay(150);
    digitalWrite(FLASH_PIN, LOW);
    delay(150);
  }
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  pinMode(FLASH_PIN, OUTPUT);
  digitalWrite(FLASH_PIN, LOW);

  Serial.begin(115200);
  delay(1000);

  Serial.println("\n=== BOOT ===");

  // Camera config
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer   = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;

  config.xclk_freq_hz = 10000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_VGA;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  Serial.println("Initializing camera...");
  if (esp_camera_init(&config) == ESP_OK) {
    Serial.println("Camera OK");
    cameraOK = true;
  } else {
    Serial.println("Camera FAILED");
  }

  // SD card init
  if (SD_MMC.begin("/sdcard", true)) {
    Serial.println("SD OK");
  } else {
    Serial.println("SD FAILED");
  }

  // Status flash
  if (cameraOK) {
    flashLED(1); // success
  } else {
    flashLED(2); // failure
  }
}

void loop() {

  if (!cameraOK) return;

  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Capture failed");
    return;
  }

  String path = "/photo" + String(photoCount++) + ".jpg";
  Serial.println("Saving: " + path);

  File file = SD_MMC.open(path.c_str(), FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file");
  } else {
    file.write(fb->buf, fb->len);
    file.close();
    Serial.println("Saved");
  }

  esp_camera_fb_return(fb);

  delay(2000); // take a photo every 2 seconds
}
