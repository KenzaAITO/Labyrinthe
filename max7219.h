/******************************************************************//**
* @file max7219.h
* Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of Maxim Integrated
* Products, Inc. shall not be used except as stated in the Maxim Integrated
* Products, Inc. Branding Policy.
*
* The mere transfer of this software does not imply any licenses
* of trade secrets, proprietary technology, copyrights, patents,
* trademarks, maskwork rights, or any other form of intellectual
* property whatsoever. Maxim Integrated Products, Inc. retains all
* ownership rights.
**********************************************************************/


#ifndef MAX7219_H
#define MAX7219_H

#include "mbed.h"


/**
* @brief Structure for device configuration
* @code
* #include "max7219.h"
*
* max7219_configuration_t cfg = {
*     .device_number = 1,
*     .decode_mode = 0,
*     .intensity = Max7219::MAX7219_INTENSITY_8,
*     .scan_limit = Max7219::MAX7219_SCAN_8
* };
* @endcode
*/
typedef struct
{
    uint8_t device_number;
    uint8_t decode_mode;
    uint8_t intensity;
    uint8_t scan_limit;
}max7219_configuration_t;


/**
* @brief   Serially Interfaced, 8-Digit, LED Display Driver
*
* @details The MAX7219/MAX7221 are compact, serial input/output common-cathode
* display drivers that interface microprocessors (µPs) to 7-segment numeric
* LED displays of up to 8 digits, bar-graph displays, or 64 individual LEDs.
* Included on-chip are a BCD code-B decoder, multiplex scan circuitry, segment
* and digit drivers, and an 8x8 static RAM that stores each digit. Only one
* external resistor is required to set the segment current for all LEDs.
* The MAX7221 is compatible with SPI™, QSPI™, and MICROWIRE™, and has
* slew-rate-limited segment drivers to reduce EMI.
*
* @code
* #include "mbed.h"
* #include "max7219.h"
*
* Max7219 max7219(SPI0_MOSI, SPI0_MISO, SPI0_SCK, SPI0_SS);
*
* int main()
* {
*     max7219_configuration_t cfg = {
*         .device_number = 1,
*         .decode_mode = 0,
*         .intensity = Max7219::MAX7219_INTENSITY_8,
*         .scan_limit = Max7219::MAX7219_SCAN_8
*     };
*
*     max7219.init_device(cfg);
*     max7219.enable_device(1);
*     max7219.set_display_test();
*     wait(1);
*     max7219.clear_display_test();
*
*     while (1) {
*         max7219.write_digit(1, Max7219::MAX7219_DIGIT_0, ...
*     }
* }
* @endcode
*/
class Max7219
{
    public:
    
    /**
     * @brief   Digit and Control Registers
     * @details The 14 addressable digit and control registers.
     */
    typedef enum
    {
        MAX7219_NO_OP = 0,
        MAX7219_DIGIT_0,
        MAX7219_DIGIT_1,
        MAX7219_DIGIT_2,
        MAX7219_DIGIT_3,
        MAX7219_DIGIT_4,
        MAX7219_DIGIT_5,
        MAX7219_DIGIT_6,
        MAX7219_DIGIT_7,
        MAX7219_DECODE_MODE,
        MAX7219_INTENSITY,
        MAX7219_SCAN_LIMIT,
        MAX7219_SHUTDOWN,
        MAX7219_DISPLAY_TEST = 15
    }max7219_register_e;
    
    
    /**
     * @brief   Intensity values
     * @details Digital control of display brightness is provided by an
     * internal pulse-width modulator, which is controlled by
     * the lower nibble of the intensity register.
     */
    typedef enum
    {
        MAX7219_INTENSITY_0 = 0,
        MAX7219_INTENSITY_1,
        MAX7219_INTENSITY_2,
        MAX7219_INTENSITY_3,
        MAX7219_INTENSITY_4,
        MAX7219_INTENSITY_5,
        MAX7219_INTENSITY_6,
        MAX7219_INTENSITY_7,
        MAX7219_INTENSITY_8,
        MAX7219_INTENSITY_9,
        MAX7219_INTENSITY_A,
        MAX7219_INTENSITY_B,
        MAX7219_INTENSITY_C,
        MAX7219_INTENSITY_D,
        MAX7219_INTENSITY_E,
        MAX7219_INTENSITY_F
    }max7219_intensity_e;
    
    
    /**
     * @brief   Scan limit for mutiplexing digits
     * @details The scan-limit register sets how many digits are 
     * displayed, from 1 to 8. They are displayed in a multiplexed
     * manner with a typical display scan rate of 800Hz with 8
     * digits displayed.
     */
    typedef enum
    {
        MAX7219_SCAN_1 = 0,
        MAX7219_SCAN_2,
        MAX7219_SCAN_3,
        MAX7219_SCAN_4,
        MAX7219_SCAN_5,
        MAX7219_SCAN_6,
        MAX7219_SCAN_7,
        MAX7219_SCAN_8
    }max7219_scan_limit_e;
        
    
    /**********************************************************//**
    * @brief Constructor for Max7219 Class.  
    * 
    * @details Allows user to pass pointer to existing SPI bus
    *
    * On Entry:
    *     @param[in] spi_bus - pointer to existing SPI object
    *     @param[in] cs - pin to use for cs
    *
    * On Exit:
    *
    * @return None
    **************************************************************/
    Max7219(SPI *spi_bus, PinName cs);
    
    
    /**********************************************************//**
    * @brief Constructor for Max7219 Class.  
    * 
    * @details Allows user to specify SPI peripheral to use
    *
    * On Entry:
    *     @param[in] mosi - pin to use for mosi
    *     @param[in] miso - pin to use for miso
    *     @param[in] sclk - pin to use for sclk
    *     @param[in] cs - pin to use for cs
    *
    * On Exit:
    *
    * @return None
    **************************************************************/
    Max7219(PinName mosi, PinName miso, PinName sclk, PinName cs);
    
    
    /**********************************************************//**
    * @brief Default destructor for Max7219 Class.  
    *
    * @details Destroys SPI object if owner 
    *
    * On Entry:
    *
    * On Exit:
    *
    * @return None
    **************************************************************/
    ~Max7219();
    
    
    /**********************************************************//**
    * @brief Sets the number of MAX7219 devices being used.
    *        Defaults to one
    *
    * @details 
    *
    * On Entry:
    *     @param[in] num_devices - number of MAX7219 devices being
    *                              used, max of 255 
    *
    * On Exit:
    *
    * @return Returns number of devices
    **************************************************************/
    int32_t set_num_devices(uint8_t num_devices);
    
    
    /**********************************************************//**
    * @brief Tests all devices being used 
    *
    * @details Sets bit0 of DISPLAY_TEST regiser in all devices
    *
    * On Entry:
    *
    * On Exit:
    *
    * @return None
    **************************************************************/
    void set_display_test(void);
    
    
    /**********************************************************//**
    * @brief Stops test
    *
    * @details Clear bit0 of DISPLAY_TEST regiser in all devices
    *
    * On Entry:
    *
    * On Exit:
    *
    * @return None
    **************************************************************/
    void clear_display_test(void);
    
    
    /**********************************************************//**
    * @brief Initializes specific device in display with given 
    *        config data
    *
    * @details 
    *
    * On Entry:
    *     @param[in] config - Structure containing configuration 
    *                         data of device
    *
    * On Exit:
    *
    * @return  Returns  0 on success\n 
    *          Returns -1 if device number is > _num_devices\n
    *          Returns -2 if device number is 0\n
    **************************************************************/
    int32_t init_device(max7219_configuration_t config);
    
    
    /**********************************************************//**
    * @brief Initializes all devices with given config data
    *
    * @details All devices are configured with given data
    *
    * On Entry:
    *     @param[in] config - Structure containing configuration 
    *                         data
    * On Exit:
    *
    * @return None
    **************************************************************/
    void init_display(max7219_configuration_t config);
    
    
    /**********************************************************//**
    * @brief Enables specific device in display
    *
    * @details 
    *
    * On Entry:
    *     @param[in] device_number - device to enable
    *
    * On Exit:
    *
    * @return Returns  0 on success\n 
    *         Returns -1 if device number is > _num_devices\n
    *         Returns -2 if device number is 0\n
    **************************************************************/
    int32_t enable_device(uint8_t device_number);
    
    
    /**********************************************************//**
    * @brief Enables all device in display 
    *
    * @details 
    *
    * On Entry:
    *
    * On Exit:
    *
    * @return None
    **************************************************************/
    void enable_display(void);
    
    
    /**********************************************************//**
    * @brief Disables specific device in display
    *
    * @details 
    *
    * On Entry:
    *     @param[in] device_number - device to disable
    *
    * On Exit:
    *    @return Returns  0 on success\n 
    *            Returns -1 if device number is > _num_devices\n
    *            Returns -2 if device number is 0\n
    **************************************************************/
    int32_t disable_device(uint8_t device_number);
    
    
    /**********************************************************//**
    * @brief Disables all devices in display
    *
    * @details 
    *
    * On Entry:
    *
    * On Exit:
    *
    * @return None
    **************************************************************/
    void disable_display(void);
    
    
    /**********************************************************//**
    * @brief Writes digit of given device with given data, user
    *        must enter correct data for decode_mode chosen
    *
    * @details 
    *
    * On Entry:
    *     @param[in] device_number - device to write too
    *     @param[in] digit - digit to write
    *     @param[in] data - data to write
    *
    * On Exit:
    *
    * @return Returns  0 on success\n 
    *         Returns -1 if device number is > _num_devices\n
    *         Returns -2 if device number is 0\n
    *         Returns -3 if digit > 8\n
    *         Returns -4 if digit < 1\n
    **************************************************************/
    int32_t write_digit(uint8_t device_number, uint8_t digit, uint8_t data);
    
    
    /**********************************************************//**
    * @brief Clears digit of given device
    *
    * @details 
    *
    * On Entry:
    *     @param[in] device_number - device to write too
    *     @param[in] digit - digit to clear
    *
    * On Exit:
    *
    * @return Returns  0 on success\n 
    *         Returns -1 if device number is > _num_devices\n
    *         Returns -2 if device number is 0\n
    *         Returns -3 if digit > 8\n
    *         Returns -4 if digit < 1\n
    **************************************************************/
    int32_t clear_digit(uint8_t device_number, uint8_t digit);
    
    
    /**********************************************************//**
    * @brief Turns on all segments/digits of given device
    *
    * @details 
    *
    * On Entry:
    *     @param[in] device_number - device to write too
    *
    * On Exit:
    *
    * @return Returns  0 on success\n 
    *         Returns -1 if device number is > _num_devices\n
    *         Returns -2 if device number is 0\n
    **************************************************************/
    int32_t device_all_on(uint8_t device_number);
    
    
    /**********************************************************//**
    * @brief Turns off all segments/digits of given device
    *
    * @details 
    *
    * On Entry:
    *     @param[in] device_number - device to write too
    *
    * On Exit:
    *
    * @return Returns 0 on success\n 
    *         Returns -1 if device number is > _num_devices\n
    *         Returns -2 if device number is 0\n
    **************************************************************/
    int32_t device_all_off(uint8_t device_number);
    
    
    /**********************************************************//**
    * @brief Turns on all segments/digits of display
    *
    * @details 
    *
    * On Entry:
    *
    * On Exit:
    *
    * @return None
    **************************************************************/
    void display_all_on(void);
    
    
    /**********************************************************//**
    * @brief Turns off all segments/digits of display
    *
    * @details 
    *
    * On Entry:
    *
    * On Exit:
    *
    * @return None
    **************************************************************/
    void display_all_off(void);
    
      
    private:
    
    SPI *_p_spi;
    DigitalOut *_p_cs;
    bool _spi_owner;
    
    uint8_t _num_devices;
    
};
#endif /* MAX7219_H*/