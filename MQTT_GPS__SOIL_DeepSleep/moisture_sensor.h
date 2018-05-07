void getSoilTest(gps_info gps_data) {
  int touch_value = 100;
  touch_value = touchRead(TOUCH_PIN);
  Serial.print("Soil Test: ");
  Serial.println(touch_value);  // get value using T0
  long lastMsg = 0;

  if (touch_value < 50)
  {
    digitalWrite (BUILTIN_LED, HIGH);
  }
  else
  {
    digitalWrite (BUILTIN_LED, LOW);
  }
  int n = 1; //Number of data types to send
  String sType[n], sUOM[n];
  float sValue[n];
  for (int i = 0; i < n; i++) {
    if (i == 0) {
      sType[i] = 'M';
      sValue[i] = touch_value;
      sUOM[i] = 'R';
    }
  }
  sendJS(gps_data, n, sType, sValue, sUOM);
}

