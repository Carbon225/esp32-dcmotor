#include "dcmotor.h"
#include "error_check_return.h"

esp_err_t DCMotor::Install(const dcmotor_config_t &config)
{
	_ledcSpeedMode = config.highSpeedMode ? LEDC_HIGH_SPEED_MODE : LEDC_LOW_SPEED_MODE;
	_ledcChannel = config.ledcChannel;
	_gpioB = config.gpioB;

	// configure PWM pin A

	ledc_timer_config_t timer_conf = {
			.speed_mode = _ledcSpeedMode,
			.duty_resolution = LEDC_TIMER_11_BIT,
			.timer_num = config.ledcTimer,
			.freq_hz = config.pwmFreq,
			.clk_cfg = LEDC_AUTO_CLK
	};

	ESP_ERROR_CHECK_RETURN(ledc_timer_config(&timer_conf));

	ledc_channel_config_t ledc_conf = {
			.gpio_num = config.gpioA,
			.speed_mode = _ledcSpeedMode,
			.channel = config.ledcChannel,
			.intr_type = LEDC_INTR_DISABLE,
			.timer_sel = config.ledcTimer,
			.duty = 0,
			.hpoint = 0
	};

	ESP_ERROR_CHECK_RETURN(ledc_channel_config(&ledc_conf));


	// configure pin B

	gpio_config_t gpio_conf = {
			.pin_bit_mask = BIT(config.gpioB),
			.mode = GPIO_MODE_OUTPUT,
			.pull_up_en = GPIO_PULLUP_DISABLE,
			.pull_down_en = GPIO_PULLDOWN_DISABLE,
			.intr_type = GPIO_INTR_DISABLE
	};

	ESP_ERROR_CHECK_RETURN(gpio_config(&gpio_conf));
	ESP_ERROR_CHECK_RETURN(gpio_set_level(config.gpioB, 0));

	return ESP_OK;
}

esp_err_t DCMotor::SetSpeed(int16_t speed)
{
	if (speed == 0)
	{
		ESP_ERROR_CHECK_RETURN(ledc_set_duty(_ledcSpeedMode, _ledcChannel, 0));
		ESP_ERROR_CHECK_RETURN(ledc_update_duty(_ledcSpeedMode, _ledcChannel));

		ESP_ERROR_CHECK_RETURN(gpio_set_level(_gpioB, 0));
	}
	else if (speed > 0)
	{
		ESP_ERROR_CHECK_RETURN(ledc_set_duty(_ledcSpeedMode, _ledcChannel, speed));
		ESP_ERROR_CHECK_RETURN(ledc_update_duty(_ledcSpeedMode, _ledcChannel));

		ESP_ERROR_CHECK_RETURN(gpio_set_level(_gpioB, 0));
	}
	else
	{
		ESP_ERROR_CHECK_RETURN(ledc_set_duty(_ledcSpeedMode, _ledcChannel, 2048 + speed));
		ESP_ERROR_CHECK_RETURN(ledc_update_duty(_ledcSpeedMode, _ledcChannel));

		ESP_ERROR_CHECK_RETURN(gpio_set_level(_gpioB, 1));
	}

	return ESP_OK;
}
