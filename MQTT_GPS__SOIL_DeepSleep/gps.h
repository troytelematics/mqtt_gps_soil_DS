gps_info getGpsReading() {
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  // for (unsigned long start = millis(); millis() - start < 1000;)
  // {
  while (Serial1.available())
  {
    char c = Serial1.read();
    //Serial.write(c); // uncomment this line if you want to see the GPS data flowing
    if (gps.encode(c)) // Did a new valid sentence come in?

      newData = true;
  }
  //}

  if (newData)
  {


    float flat, flon;
    unsigned long fix_age, time, date, speed, course;

    smartdelay(4000);
    Serial.println("Getting Position");
    gps.f_get_position(&flat, &flon, &fix_age);
    if (fix_age != TinyGPS::GPS_INVALID_AGE) {
      int year;
      byte month, day, hour, minute, second, hundredths;

      gps.crack_datetime(&year, &month, &day,
                         &hour, &minute, &second, &hundredths, &fix_age);

      unsigned long epoch = unixTimestamp(year, month, day, hour, minute, second);

      //      Serial.println(epoch);
      //      Serial.print(" SAT=");
      //      Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
      //      Serial.print(" PREC=");
      //      Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
      //Send the data
      gps_info gps_data;
      gps_data.utime = epoch;
      gps_data.lat = flat;
      gps_data.lon = flon;
      return gps_data;
      //sendJS(epoch, flat, flon);
    }
  }

}

