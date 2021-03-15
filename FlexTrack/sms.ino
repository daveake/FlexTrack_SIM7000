#define TINY_GSM_MODEM_SIM7000
#define TINY_GSM_RX_BUFFER 1024 // Set RX buffer to 1Kb
#define SerialAT Serial1

// See all AT commands, if wanted
// #define DUMP_AT_COMMANDS

/*
   Tests enabled
*/
#define TINY_GSM_TEST_GPRS    true
#define TINY_GSM_TEST_GPS     true
#define TINY_GSM_POWERDOWN    true

// set GSM PIN, if any
#define GSM_PIN ""

// Your GPRS credentials, if any
const char apn[]  = "";     //SET TO YOUR APN
const char gprsUser[] = "";
const char gprsPass[] = "";

#include <TinyGsmClient.h>
#include <SPI.h>
#include <SD.h>
#include <Ticker.h>

#ifdef DUMP_AT_COMMANDS  // if enabled it requires the streamDebugger lib
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, Serial);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

#define uS_TO_S_FACTOR 1000000ULL  // Conversion factor for micro seconds to seconds
#define TIME_TO_SLEEP  60          // Time ESP32 will go to sleep (in seconds)

void SetupSMS()
{
  Serial.println("Initializing modem...");
  if (!modem.restart()) {
    Serial.println("Failed to restart modem, attempting to continue without restarting");
  }

  String name = modem.getModemName();
  delay(500);
  Serial.println("Modem Name: " + name);

  String modemInfo = modem.getModemInfo();
  delay(500);
  Serial.println("Modem Info: " + modemInfo);

  String ccid = modem.getSimCCID();
  Serial.println("CCID: " + ccid);

  String imei = modem.getIMEI();
  Serial.println("IMEI: " + imei);

  String cop = modem.getOperator();
  Serial.println("Operator: " + cop);
}

void CheckSMS()
{
  GPS.CSQ = modem.getSignalQuality();
  Serial.println("Signal quality: " + String(GPS.CSQ));
}

#ifdef MOBILE_NUMBER
int SendPositionToPhone()
{
  static int Sentence=0;
  int SentOK=0;
  char Message[512];

  // Text to mobile
  sprintf(Message, "%s position: %02d:%02d:%02d, %f, %f, %.0f http://maps.google.com/?q=%f,%f",
                    PAYLOAD_ID,
                    GPS.Hours, GPS.Minutes, GPS.Seconds,
                    GPS.Latitude, GPS.Longitude, GPS.Altitude,
                    GPS.Latitude, GPS.Longitude);
  Serial.println(Message);                      
  Serial.printf("Sending to mobile %s: %s\n", MOBILE_NUMBER, Message);

  if (modem.sendSMS(MOBILE_NUMBER, Message))
  {
    Serial.println("Sent OK");
    SentOK = 1;
  }
  else
  {
    Serial.println("Failed to send");
  }
  
  return SentOK;
}
#endif

#ifdef GATEWAY_NUMBER
int SendPositionToGateway()
{
  static int Sentence=0;
  int SentOK=0;
  char Message[512];

  sprintf(Message, "HAB:%s,%d,%02d:%02d:%02d,%f,%f,%.0f",
                    PAYLOAD_ID,
                    ++Sentence,
                    GPS.Hours, GPS.Minutes, GPS.Seconds,
                    GPS.Latitude, GPS.Longitude, GPS.Altitude);
  Serial.printf("Sending to gateway %s: %s\n", GATEWAY_NUMBER, Message);
  if (modem.sendSMS(GATEWAY_NUMBER, Message))
  {
    Serial.println("Sent OK");
    SentOK = 1;
  }
  else
  {
    Serial.println("Failed to send");
  }

  return SentOK;
}
#endif
