#include "power.h"
#include "arm.h"
#include "i2c.h"

void mcuReboot() {
    I2C_writeReg(I2C_DEV_CTR_MCU, 0x22, 1 << 0); // poweroff LCD to prevent MCU hangs
    ARM_WbDC();
    ARM_DSB();
    I2C_writeReg(I2C_DEV_CTR_MCU, 0x20, 1 << 2);
    while(true);
}

void mcuPoweroff()
{
    I2C_writeReg(I2C_DEV_CTR_MCU, 0x22, 1 << 0); // poweroff LCD to prevent MCU hangs
    ARM_WbDC();
    ARM_DSB();
    I2C_writeReg(I2C_DEV_CTR_MCU, 0x20, 1 << 0);
    while(true);
}
