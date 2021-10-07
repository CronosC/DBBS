#ifndef __moisture_sensor__ // avoid duplicate definition
#define __moisture_sensor__

void ADC_init(void);
int16_t ADC_read(void);

int16_t MOISTURE_SENSOR_get_averaged_reading(int8_t n);

void MOISTURE_SENSOR_init(void);
void MOISTURE_SENSOR_toggle(void);

#endif
