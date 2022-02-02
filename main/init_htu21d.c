#include "init_htu21d.h"
#include "htu21d.h"

int init_htu21d(void)
{
    int ret = htu21d_init(I2C_NUM_0, GPIO_NUM_21, GPIO_NUM_22,
                          GPIO_PULLUP_DISABLE, GPIO_PULLUP_DISABLE);
    return ret;
}