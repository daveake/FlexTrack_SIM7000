
int NeedToSendPosition()
{
  static long PreviousAltitude = 0;
  static double PreviousLatitude = 0.0;
  static double PreviousLongitude = 0.0;
  static int Sentence=0;
  int Send = 0;

  // Need a known position to send
  if (GPS.Altitude <= SMS_MAXALT)
  {
    // Low enough.  Now test the various reasons for sending

    if (millis() > GPS.SendSMSAfter)
    {
      Send = 1;
      Serial.println("Send because of timeout");
    }
    else
    {    
      double Distance;
      
      Distance = abs(CalculateDistance(GPS.Latitude, GPS.Longitude, PreviousLatitude, PreviousLongitude));
      if (Distance >= SMS_DISTANCE)
      {
        Send = 1;
        Serial.println("Send because of horizontal movement");
      }
      else
      {  
        if (abs(GPS.Altitude - PreviousAltitude) >= SMS_ALTDELTA)
        {
          Send = 1;
          Serial.println("Send because of vertical movement");
        }
      }
    }
                
    if (Send)
    {
      PreviousAltitude = GPS.Altitude;
      PreviousLatitude = GPS.Latitude;
      PreviousLongitude = GPS.Longitude;
    }
  }

  return Send;
}
