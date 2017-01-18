#ifndef OTP_M_H
#define OTP_M_H

#include "cipher.h"
#include "data_struct.h"
#include "right_bit_shift_secret_derivator.h"
#include "config.h"

int can_go_one_level_down(struct data *client_data);
int is_current_otp_allowed(struct data *client_data);
int get_next_otp(struct data *client_data, char *result);
#endif
