int8_t lastRebootReason = -1;  /* indicate the last reboot reason */

bool readyForReboot = false;
uint16_t atNoResponseCounter = 0;
bool powerKeyCycle = false;
uint16_t mqttMsgId = 0;

/* DS18B20Temperature sensors connected to HUB  */
#define HUBDS18B20_ONE_WIRE_BUS 32
#define HUBDS18B20_TEMPERATURE_PRECISION 12

/*aTresp_unknown = -1,
        aTresp_noResponse = -2,
        aTresp_timeout = -3,
        aTresp_noKeyFound = -4,
        aTresp_failKeyFound = -5,
        aTresp_ok = 0*/
