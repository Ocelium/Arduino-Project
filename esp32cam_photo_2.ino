#include "esp_camera.h"
#include "SD_MMC.h"
#include "FS.h"

#define PIR_PIN 13
#define LED_PIN 4

bool recording = false;
bool lastMotion = false;

unsigned long recordStart = 0;
int frameCount = 0;

// ---------------- CAMERA PINS ----------------
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
// --------------------------------------------

void flashBlink() {
  ledcAttach(LED_PIN, 5000, 8);
  ledcWrite(LED_PIN, 128);
  delay(200);
  ledcWrite(LED_PIN, 0);
}

void captureFrame(int i) {
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) return;

  String path = "/rec_" + String(recordStart) + "_" + String(i) + ".jpg";

  File file = SD_MMC.open(path, FILE_WRITE);
  if (file) {
    file.write(fb->buf, fb->len);
    file.close();
  }

  esp_camera_fb_return(fb);
}

void setup() {
  Serial.begin(115200);

  // IMPORTANT: prevents floating input when unplugged
  pinMode(PIR_PIN, INPUT_PULLDOWN);

  // ---------- CAMERA INIT ----------
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer   = LEDC_TIMER_0;
  config.pin_d0       = Y2_GPIO_NUM;
  config.pin_d1       = Y3_GPIO_NUM;
  config.pin_d2       = Y4_GPIO_NUM;
  config.pin_d3       = Y5_GPIO_NUM;
  config.pin_d4       = Y6_GPIO_NUM;
  config.pin_d5       = Y7_GPIO_NUM;
  config.pin_d6       = Y8_GPIO_NUM;
  config.pin_d7       = Y9_GPIO_NUM;
  config.pin_xclk     = XCLK_GPIO_NUM;
  config.pin_pclk     = PCLK_GPIO_NUM;
  config.pin_vsync    = VSYNC_GPIO_NUM;
  config.pin_href     = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn     = PWDN_GPIO_NUM;
  config.pin_reset    = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format  = PIXFORMAT_JPEG;

  if (psramFound()) {
    config.frame_size   = FRAMESIZE_VGA;
    config.jpeg_quality = 12;
    config.fb_count     = 2;
  } else {
    config.frame_size   = FRAMESIZE_VGA;
    config.jpeg_quality = 15;
    config.fb_count     = 1;
  }

  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("Camera init failed");
    return;
  }

  if (!SD_MMC.begin("/sdcard", true)) {
    Serial.println("SD mount failed");
    return;
  }

  Serial.println("Motion system active");
}

void loop() {

  if digitalRead(PIR_PIN) == high;
    bool motion = true;

  //  detect motion START (edge trigger)
  if (motion == HIGH && !lastMotion && !recording) {

    delay(50); // stability check

    if (digitalRead(PIR_PIN) == HIGH) {

      Serial.println("Motion detected → recording 10s");

      recording = true;
      recordStart = millis();
      frameCount = 0;

      flashBlink();
    }
  }

  //  record for 10 seconds
  if (recording) {

    captureFrame(frameCount++);
    delay(200);

    if (millis() - recordStart >= 10000) {
      recording = false;
      frameCount = 0;
      Serial.println("Recording finished");
      flashBlink();
    }
  }

  lastMotion = motion;
}
