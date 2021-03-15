//------------------------------------------------------------------------------------------------------

// TRACKER CONFIGURATION

#define PAYLOAD_ID      "GSM"

// Your phone number to send SMS: + (plus sign) and country code, followed by phone number
// UK country code is 44; USA is 1; for others check online
// So sample number might be "+4477001234567"

// #define GATEWAY_NUMBER      "+44....."   // <<--- To send to an SMS-HABHUB gateway, uncomment and fill in the gateway's phone number
// #define MOBILE_NUMBER       "+44....."   // <<--- To send to a smartphone, uncomment and fill in the smartphone's number

// Parameters to control when texts are sent

#define SMS_MAXALT    2000      // Don't try to send above this altitude
#define SMS_TIMEOUT   10        // Send position every x minutes regardless of movement
#define SMS_DISTANCE  50        // Send position if it moves horizontally by at least this many metres
#define SMS_ALTDELTA  50        // Send position if it moves vertically by at least this many metres


// HARDWARE CONFIGURATION

#define UART_BAUD   9600
#define PIN_DTR     25
#define PIN_TX      27
#define PIN_RX      26
#define PWR_PIN     4
#define LED_PIN     12


//------------------------------------------------------------------------------------------------------
//
//  Globals

struct TGPS
{
  int FixStatus;
  int Hours, Minutes, Seconds;
  float Longitude, Latitude, Altitude;
  unsigned int Satellites, SendSMSAfter;
  int NeedToSendToPhone, NeedToSendToGateway;
  int CSQ;
} GPS;


//------------------------------------------------------------------------------------------------------

void setup()
{
  Serial.begin(9600);
  
  Serial.printf("\n\nFlexTrack GSM Flight Computer, payload ID '%s'\n", PAYLOAD_ID);
   
  SetupSMS();

  SetupGPS();

  Serial.println("Init done");
}


void loop()
{  
  CheckSMS();
  
  if (CheckGPS())
  {
    // Got new position

    if (NeedToSendPosition())
    {
      GPS.NeedToSendToPhone = 1;
      GPS.NeedToSendToGateway = 1;
    }

    // Enough signal ?
    if (GPS.CSQ > 4)
    {
      #ifdef MOBILE_NUMBER
        if (GPS.NeedToSendToPhone)
        {
          if (SendPositionToPhone())
          {
            GPS.NeedToSendToPhone = 0;
            
            GPS.SendSMSAfter = millis() + SMS_TIMEOUT * 60000L;
          }
        }
    
      #endif
    
      #ifdef GATEWAY_NUMBER
        if (GPS.NeedToSendToGateway)
        {
          if (SendPositionToGateway())
          {
            GPS.NeedToSendToGateway = 0;
            
            GPS.SendSMSAfter = millis() + SMS_TIMEOUT * 60000L;
          }
        }
      #endif
    }
  }
}
