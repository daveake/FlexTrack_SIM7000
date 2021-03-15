
void SetupGPS()
{
  // Set LED OFF
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  pinMode(PWR_PIN, OUTPUT);
  digitalWrite(PWR_PIN, HIGH);
  delay(300);
  digitalWrite(PWR_PIN, LOW);

  Serial.println("\nWaiting for modem ...");

  delay(10000);

  Serial.println("\nSetting up modem ...");
  
  Serial1.begin(UART_BAUD, SERIAL_8N1, PIN_RX, PIN_TX);

  Serial1.print("AT+SGPIO=0,4,1,1\r");
  delay(1000);
  Serial1.print("AT+CGNSPWR=1\r");
}

int ProcessPosition(char *Buffer, int Count)
{
  int RunStatus=0, GotNewPosition=0;
  
  // +CGNSINF: 1,1,20201028103917.000,51.950289,-2.544395,142.400,0.00,290.9,1,,0.8,1.0,0.7,,27,9,4,,46,,
  // Run status 1 = OK
  // Fix status 1 = Got position
  // UTC date and time
  // latitude
  // Longitude
  // MSL Atitude
  // Speed Over Ground
  // Course Over Ground
  // Fix Mode
  // Reserved1
  // HDOP
  // PDOP
  // VDOP
  // Reserved2
  // GNSS Satellites in View
  // GNSS Satellites Used
  // GLONASS Satellites Used
  // Reserved3
  // C/N0 max
  // HPA
  // VPA

  Serial.print(Buffer);

  if (Buffer[0] == '+')
  {
    char DateAndTime[20];
  // UTC date and time
  // latitude
  // Longitude
  // MSL Altitude
  // Speed Over Ground
  // Course Over Ground
  // Fix Mode
  // Reserved1
  // HDOP
  // PDOP
  // VDOP
  // Reserved2
  // GNSS Satellites in View
  // GNSS Satellites Used
  // GLONASS Satellites Used
  // Reserved3
  // C/N0 max
  // HPA
  // VPA    
    
    if (sscanf (Buffer, "+CGNSINF: %d,%d,%20[^,],%f,%f,%f",
                &RunStatus, &GPS.FixStatus, DateAndTime,
                &GPS.Latitude, &GPS.Longitude, &GPS.Altitude) >= 5)
    {
      // %20[^,],%16[^,],%c,%[^,],%c,%d,%d,%[^,],%[^,]",
      // 20201028103917.000,51.950289,-2.544395,142.400,0.00,290.9,1,,0.8,1.0,0.7,,27,9,4,,46,,

      Serial.printf("Status = %d/%d\n", RunStatus, GPS.FixStatus);
      Serial.printf("DateAndTime = '%s'\n", DateAndTime);
      
      if (GPS.FixStatus)
      {
        GotNewPosition = 1;

        sscanf(DateAndTime+8, "%2d%2d%2d", &GPS.Hours, &GPS.Minutes, &GPS.Seconds);
        
        Serial.printf("Time=%d:%d:%d, Lat=%f, Lon=%f, Alt=%f\n", GPS.Hours, GPS.Minutes, GPS.Seconds, GPS.Latitude, GPS.Longitude, GPS.Altitude);
      }
    }
  }

  return GotNewPosition;
}

int CheckGPS()
{
  static unsigned long NextGPS=0;
  int GotNewPosition=0;
  char Line[128];
  int Length=0;
  
  if (millis() >= NextGPS)
  {
    // Poll GPS
    Serial1.print("AT+CGNSINF\r");
    delay(1000);
  
    while (Serial1.available())
    {
      unsigned char Character;
       
      Character = Serial1.read();
    
      if (Length >= (sizeof(Line)-2))
      {
        Length = 0;
      }
      else if (Character != '\r')
      {
        Line[Length++] = Character;
        if (Character == '\n')
        {
          Line[Length] = '\0';
          if (ProcessPosition(Line, Length))
          {
            GotNewPosition = 1;
          }
          Length = 0;
        }
      }
    }
  }

  return GotNewPosition;
}
