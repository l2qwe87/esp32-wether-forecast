#define PIR_PIN GPIO_NUM_34

void setup_sleep(){

}

void gotoDeepSleep(){
  Serial.println("gotoDeepSleep");
    
  delay(1000);
  Serial.println(5);
  delay(1000);
  Serial.println(4);
  delay(1000);
  Serial.println(3);
  delay(1000);
  Serial.println(2);
  delay(1000);
  Serial.println(1);
  delay(1000);

  Serial.println(esp_sleep_is_valid_wakeup_gpio(PIR_PIN));
  esp_sleep_enable_ext0_wakeup(PIR_PIN, HIGH);
  esp_deep_sleep_start();
}
