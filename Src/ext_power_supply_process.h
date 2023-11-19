

#ifndef ROBOTICARM_V4_EXT_POWER_SUPPLY_PROCESS_H
#define ROBOTICARM_V4_EXT_POWER_SUPPLY_PROCESS_H

double ext_power_supply_delay_counter = 0;

int lastState = 0;

void tick_ext_power_supply(double now, double dt)
{
    if (lastState != power_state) {
        lastState = power_state;
        if (power_state) {

            TIM_HandleTypeDef *htimRef = &htim3;

            HAL_TIM_Base_Start(htimRef);
            HAL_TIM_PWM_Start(htimRef, TIM_CHANNEL_3);

            for (int i = 0; i < 80; ++i) {
                double x = i / 80.0;
                double v = x - 1;
                double y = 1 - v * v * v * v;
                int freq = (int) (1000 + y * 600);
                __HAL_TIM_SET_AUTORELOAD(htimRef, 4000000 / freq - 1);
                __HAL_TIM_SET_COMPARE(htimRef, TIM_CHANNEL_3, 4000000 / freq / 2 - 1);

                HAL_Delay(1);
            }

            HAL_TIM_PWM_Stop(htimRef, TIM_CHANNEL_3);
            HAL_TIM_Base_Stop(htimRef);
        } else {
            TIM_HandleTypeDef *htimRef = &htim3;

            HAL_TIM_Base_Start(htimRef);
            HAL_TIM_PWM_Start(htimRef, TIM_CHANNEL_3);

            for (int i = 0; i < 80; ++i) {
                double x = i / 80.0;
                double v = x - 1;
                double y = 1 - v * v * v * v;
                int freq = (int) (1400 - y * 600);
                __HAL_TIM_SET_AUTORELOAD(htimRef, 4000000 / freq - 1);
                __HAL_TIM_SET_COMPARE(htimRef, TIM_CHANNEL_3, 4000000 / freq / 2 - 1);

                HAL_Delay(1);
            }

            HAL_TIM_PWM_Stop(htimRef, TIM_CHANNEL_3);
            HAL_TIM_Base_Stop(htimRef);
        }
    }

    ext_power_supply_delay_counter += dt;

    if (!power_state) {
        ext_power_supply_delay_counter = 0;
    }

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, ext_power_supply_delay_counter < 3);

}

#endif //ROBOTICARM_V4_EXT_POWER_SUPPLY_PROCESS_H
