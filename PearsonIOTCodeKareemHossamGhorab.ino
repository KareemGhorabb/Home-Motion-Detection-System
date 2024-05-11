#define BLYNK_TEMPLATE_ID "TMPL2o-0v5Ak1"
#define BLYNK_TEMPLATE_NAME "IoT Pearson Project"
#define BLYNK_AUTH_TOKEN "IqPRlmQp8M2a_w2Qj-kc_qoOToEfLEIh"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <EMailSender.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Hossam Farag 1"; // Change your Wifi/ Hotspot Name
char pass[] = "H0$$am@1279#06$s"; // Change your Wifi/ Hotspot Password

BlynkTimer timer;
EMailSender emailSend("IotPearson.KareemHossamGhorab@gmail.com", "tezdgemsjtwkerwi");

#define motion D7
#define RED D1
#define buzzer D4
int motion_Val = 0;
unsigned long motionStartTime = 0;
bool motionDetected = false;
bool alarmActivated = false; // Flag to track if alarm has been activated

WidgetLED led(V1);

void setup() {
  Serial.begin(9600);
  pinMode(motion, INPUT);
  pinMode(RED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
  Blynk.begin(auth, ssid, pass);
  delay(2000);
  timer.setInterval(500L, mySensor);
}

void loop() {
  Blynk.run();
  timer.run();
}

void mySensor() {
  motion_Val = digitalRead(motion);
  unsigned long currentMillis = millis();

  if (motion_Val == HIGH && !motionDetected) { // Check if motion detected and alarm not already activated
    motionStartTime = currentMillis;
    motionDetected = true;

    digitalWrite(RED, HIGH);
    digitalWrite(buzzer, HIGH);
    Blynk.virtualWrite(V0, 1);
    Serial.println("Motion detected!");
    led.on();

    // Send email
    EMailSender::EMailMessage message;
    message.subject = "Motion Detected!";
    message.message = "Motion has been detected by the sensor.";
    EMailSender::Response resp = emailSend.send("kareemhossam.weschool@gmail.com", message);
    Serial.println("Sending status: ");
    Serial.println(resp.status);
    Serial.println(resp.code);
    Serial.println(resp.desc);
  }

  if (motionDetected && (currentMillis - motionStartTime >= 1000)) { // If 3 seconds have passed since motion detected
    digitalWrite(RED, LOW);
    digitalWrite(buzzer, LOW);
    Blynk.virtualWrite(V0, 0);
    Serial.println("No motion detected.");
    led.off();
    motionDetected = false; // Reset motion detection flag
  }
}

