## FlexTrack_SIM7000

Backup tracker for high altitude balloons though could be used for other applications.

It's written for the TTGO SIM7000 board which has a combined GPS/GSM module.

The program monitors the GPS position and, periodically or when it detects significant movement, sends a text message (SMS) to one or two recipients.

These messages are in 2 formats:

1 - UKHAS string to send to a gateway (Pi + GSM dongle) which relays the message to the HABHUB server
2 - Readable string to send to a smartphone, with embedded map co-ordinates which when touched will start the phone's mapping application and show the location

## Disclaimer

The code is provided as is with no guarantees of performance or operation. 

If you decide to use this code under a balloon it’s your responsibility to ensure you comply with the local legislation and laws regarding meteorological balloon launching and radio transmission in the air. 

## Further Reading on High Altitude Ballooning

Please read this http://www.daveakerman.com/?p=1732

