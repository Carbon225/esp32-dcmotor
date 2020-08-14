#ifndef COMPONENTS_ESP32_DCMOTOR_INCLUDE_DCMOTOR_H_
#define COMPONENTS_ESP32_DCMOTOR_INCLUDE_DCMOTOR_H_

#include "driver/ledc.h"

struct dcmotor_config_t
{
	gpio_num_t gpioA;
	gpio_num_t gpioB;
	ledc_timer_t ledcTimer;
	ledc_channel_t ledcChannel;
	bool highSpeedMode = false;
	uint32_t pwmFreq = 5000;
};

class DCMotor
{
public:
	esp_err_t Install(const dcmotor_config_t &config);
	esp_err_t SetSpeed(int16_t speed);

private:
	gpio_num_t _gpioB = GPIO_NUM_MAX;
	ledc_channel_t _ledcChannel = LEDC_CHANNEL_MAX;
	ledc_mode_t _ledcSpeedMode = LEDC_SPEED_MODE_MAX;
};


#endif /* COMPONENTS_ESP32_DCMOTOR_INCLUDE_DCMOTOR_H_ */
