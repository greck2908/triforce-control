/* Copyright (c) 2017 Cameron A. Craig, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdlib.h>
#include <vector>
#include "mbed.h"
#include "esc.h"
#include "comms_pwm.h"
#include "config.h"

/**
* @file comms_pwm.cpp
* @author Cameron A. Craig
* @date 23 Sep 2017
* @copyright 2017 Cameron A. Craig
* @brief Implements PWM communication for ESC control.
*/

static std::vector<ESC> lookup_table;

/* The various drive configurations are available in docs/drive_modes. */
volatile comms_impl_t comms_impl_pwm = {
  .impl_id = COMMS_IMPL_PWM,
  .str = "PWM",
  .init_comms = comms_impl_pwm_init_comms,
  .init_esc = comms_impl_pwm_init_esc,
  .set_speed = comms_impl_pwm_set_speed,
  .get_speed = NULL,
  .get_status = NULL,
  .stop = comms_impl_pwm_stop
};


void comms_impl_pwm_init_comms() {
  lookup_table.push_back(ESC(DRIVE_ESC_OUT_1_PIN, 20, 1500));
  lookup_table.push_back(ESC(DRIVE_ESC_OUT_2_PIN, 20, 1500));
  lookup_table.push_back(ESC(DRIVE_ESC_OUT_3_PIN, 20, 1500));
  lookup_table.push_back(ESC(WEAPON_ESC_OUT_1_PIN));
  lookup_table.push_back(ESC(WEAPON_ESC_OUT_2_PIN));
  lookup_table.push_back(ESC(WEAPON_ESC_OUT_3_PIN));
}

void comms_impl_pwm_init_esc(comms_esc_t *esc, comms_esc_id_t id){
  esc->id = id;
}

void comms_impl_pwm_set_speed(comms_esc_t *esc, uint32_t speed) {
    lookup_table[COMMS_OUTPUT_DRIVE_1].setThrottle(speed);
}

void comms_impl_pwm_stop(comms_esc_t *esc) {
  // The failsafe function just sets the throttle to 0
  // TODO: Is the failsafe function really necessary?
  // Could just do setThrottle(0), which is arguably more readable?
  lookup_table[COMMS_OUTPUT_DRIVE_1].failsafe();
}
