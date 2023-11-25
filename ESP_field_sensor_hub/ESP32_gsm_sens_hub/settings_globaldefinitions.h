int8_t lastRebootReason = -1;  /* indicate the last reboot reason */

bool readyForReboot = false;

/* DS18B20Temperature sensors connected to HUB  */
#define HUBDS18B20_ONE_WIRE_BUS 32
#define HUBDS18B20_TEMPERATURE_PRECISION 12
