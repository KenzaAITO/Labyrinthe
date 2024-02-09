/******************************************************************//**
* @file max7219.cpp
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


#include "max7219.h"


//*********************************************************************
Max7219::Max7219(SPI *spi_bus, PinName cs): _p_spi(spi_bus)
{
    _num_devices = 1;
    
    _p_cs = new DigitalOut(cs, 1);
    _spi_owner = false;
}


//*********************************************************************
Max7219::Max7219(PinName mosi, PinName miso, PinName sclk, PinName cs)
{
    _num_devices = 1;
    
    _p_spi = new SPI(mosi, miso, sclk);
    _p_cs = new DigitalOut(cs, 1);
    
    _spi_owner = true;
}


//*********************************************************************
Max7219::~Max7219()
{
    delete _p_cs;
    
    if(_spi_owner) 
    {
        delete _p_spi;
    }
}


//*********************************************************************
int32_t Max7219::set_num_devices(uint8_t num_devices)
{
    int32_t rtn_val = -1;
    
    if(num_devices > 0)
    {
        _num_devices = num_devices;
        rtn_val = _num_devices;
    }
    
    return(rtn_val);
}


//*********************************************************************
void Max7219::set_display_test(void)
{
    uint8_t idx = 0;
    
    _p_cs->write(0); 
    for(idx = 0; idx < _num_devices; idx++)
    {
        _p_spi->write(MAX7219_DISPLAY_TEST);
        _p_spi->write(1);
    }
    _p_cs->write(1); 
}


//*********************************************************************
void Max7219::clear_display_test(void)
{
    uint8_t idx = 0;
    
    _p_cs->write(0); 
    for(idx = 0; idx < _num_devices; idx++)
    {
        _p_spi->write(MAX7219_DISPLAY_TEST);
        _p_spi->write(0);
    }
    _p_cs->write(1); 
}


//*********************************************************************
int32_t Max7219::init_device(max7219_configuration_t config)
{
    int32_t rtn_val = -1;
    uint8_t idx = 0;
    
    if(config.device_number > _num_devices)
    {
        rtn_val = -1;
    }
    else if(config.device_number == 0)
    {
        //device numbering starts with index 1
        rtn_val = -2;
    }
    else
    {
        //write DECODE_MODE register of device
        _p_cs->write(0); 
        for(idx = _num_devices; idx > 0; idx--)
        {
            if(config.device_number == idx)
            {
                _p_spi->write(MAX7219_DECODE_MODE);
                _p_spi->write(config.decode_mode);
            }
            else
            {
                _p_spi->write(MAX7219_NO_OP);
                _p_spi->write(0);
            }
        }
        _p_cs->write(1); 
        
        wait_us(1);
        
        //write INTENSITY register of device
        _p_cs->write(0); 
        for(idx = _num_devices; idx > 0; idx--)
        {
            if(config.device_number == idx)
            {
                _p_spi->write(MAX7219_INTENSITY);
                _p_spi->write(config.intensity);
            }
            else
            {
                _p_spi->write(MAX7219_NO_OP);
                _p_spi->write(0);
            }
        }
        _p_cs->write(1); 
        
        wait_us(1);
        
        //write SCAN_LIMT register of device
        _p_cs->write(0); 
        for(idx = _num_devices; idx > 0; idx--)
        {
            if(config.device_number == idx)
            {
                _p_spi->write(MAX7219_SCAN_LIMIT);
                _p_spi->write(config.scan_limit);
            }
            else
            {
                _p_spi->write(MAX7219_NO_OP);
                _p_spi->write(0);
            }
        }
        _p_cs->write(1); 
        
        wait_us(1);
        
        rtn_val = 0;
    }
    
    return(rtn_val);
}


//*********************************************************************
void Max7219::init_display(max7219_configuration_t config)
{
    uint8_t idx = 0;
    
    //write DECODE_MODE register of all devices
    _p_cs->write(0); 
    for(idx = 0; idx < _num_devices; idx++)
    {
        _p_spi->write(MAX7219_DECODE_MODE);
        _p_spi->write(config.decode_mode);
    }
    _p_cs->write(1); 
    
    wait_us(1);
    
    //write INTENSITY register of all devices
    _p_cs->write(0); 
    for(idx = 0; idx < _num_devices; idx++)
    {
        _p_spi->write(MAX7219_INTENSITY);
        _p_spi->write(config.intensity);
    }
    _p_cs->write(1); 
    
    wait_us(1);
    
    //write SCAN_LIMT register of all devices
    _p_cs->write(0); 
    for(idx = 0; idx < _num_devices; idx++)
    {
        _p_spi->write(MAX7219_SCAN_LIMIT);
        _p_spi->write(config.scan_limit);
    }
    _p_cs->write(1); 
    
    wait_us(1);
}


//*********************************************************************
int32_t Max7219::enable_device(uint8_t device_number)
{
    int32_t rtn_val = -1;
    uint8_t idx = 0;
    
    if(device_number > _num_devices)
    {
        rtn_val = -1;
    }
    else if(device_number == 0)
    {
        //device numbering starts with index 1
        rtn_val = -2;
    }
    else
    {
        _p_cs->write(0); 
        for(idx = _num_devices; idx > 0; idx--)
        {
            if(device_number == idx)
            {
                _p_spi->write(MAX7219_SHUTDOWN);
                _p_spi->write(1);
            }
            else
            {
                _p_spi->write(MAX7219_NO_OP);
                _p_spi->write(0);
            }
        }
        _p_cs->write(1); 
        
        rtn_val = 0;
    }
    
    return(rtn_val);
}


//*********************************************************************
void Max7219::enable_display(void)
{
    uint8_t idx = 0;
    
    _p_cs->write(0); 
    for(idx = 0; idx < _num_devices; idx++)
    {
        _p_spi->write(MAX7219_SHUTDOWN);
        _p_spi->write(1);
    }
    _p_cs->write(1); 
}
    

//*********************************************************************    
int32_t Max7219::disable_device(uint8_t device_number)
{
    int32_t rtn_val = -1;
    uint8_t idx = 0;
    
    if(device_number > _num_devices)
    {
        rtn_val = -1;
    }
    else if(device_number == 0)
    {
        //device numbering starts with index 1
        rtn_val = -2;
    }
    else
    {
        _p_cs->write(0); 
        for(idx = _num_devices; idx > 0; idx--)
        {
            if(device_number == idx)
            {
                _p_spi->write(MAX7219_SHUTDOWN);
                _p_spi->write(0);
            }
            else
            {
                _p_spi->write(MAX7219_NO_OP);
                _p_spi->write(0);
            }
        }
        _p_cs->write(1); 
        
        rtn_val = 0;
    }
    
    return(rtn_val);
}


//*********************************************************************    
void Max7219::disable_display(void)
{
    uint8_t idx = 0;
    
    _p_cs->write(0); 
    for(idx = 0; idx < _num_devices; idx++)
    {
        _p_spi->write(MAX7219_SHUTDOWN);
        _p_spi->write(0);
    }
    _p_cs->write(1); 
}



//********************************************************************* 
int32_t Max7219::write_digit(uint8_t device_number, uint8_t digit, uint8_t data)
{
    int32_t rtn_val = -1;
    uint8_t idx = 0;
    
    if(digit > MAX7219_DIGIT_7)
    {
        rtn_val = -3;
    }
    else if(digit < MAX7219_DIGIT_0)
    {
        rtn_val = -4;
    }
    else
    {
        if(device_number > _num_devices)
        {
            rtn_val = -1;
        }
        else if(device_number == 0)
        {
            rtn_val = -2;
        }
        else
        {
            _p_cs->write(0); 
            for(idx = _num_devices; idx > 0; idx--)
            {
                if(idx == device_number)
                {
                    _p_spi->write(digit);
                    _p_spi->write(data);
                }
                else
                {
                    _p_spi->write(MAX7219_NO_OP);
                    _p_spi->write(0);
                }
            }
            _p_cs->write(1); 
            
            rtn_val = 0;
        }
    }
    
    return(rtn_val);
}
    

//*********************************************************************     
int32_t Max7219::clear_digit(uint8_t device_number, uint8_t digit)
{
    int32_t rtn_val = -1;
    uint8_t idx = 0;
    
    if(digit > MAX7219_DIGIT_7)
    {
        rtn_val = -3;
    }
    else if(digit < MAX7219_DIGIT_0)
    {
        rtn_val = -4;
    }
    else
    {
        if(device_number > _num_devices)
        {
            rtn_val = -1;
        }
        else if(device_number == 0)
        {
            rtn_val = -2;
        }
        else
        {
            _p_cs->write(0); 
            for(idx = _num_devices; idx > 0; idx--)
            {
                if(idx == device_number)
                {
                    _p_spi->write(digit);
                    _p_spi->write(0);
                }
                else
                {
                    _p_spi->write(MAX7219_NO_OP);
                    _p_spi->write(0);
                }
            }
            _p_cs->write(1); 
            
            rtn_val = 0;
        }
    }
    
    return(rtn_val);
}


//********************************************************************* 
int32_t Max7219::device_all_on(uint8_t device_number)
{
    int32_t rtn_val = -1;
    uint8_t idx = 0;
    
    if(device_number > _num_devices)
    {
        rtn_val = -1;
    }
    else if(device_number == 0)
    {
        rtn_val = -2;
    }
    else
    {
        rtn_val = 0;
        
        //writes every digit of given device to 0xFF
        for(idx = 0; idx < 8; idx++)
        {
            if(rtn_val == 0)
            {
                rtn_val = write_digit(device_number, (idx + 1), 0xFF);
            }
        }
    }
    
    return(rtn_val);
}


//********************************************************************* 
int32_t Max7219::device_all_off(uint8_t device_number)
{
    int32_t rtn_val = -1;
    uint8_t idx = 0;
    
    if(device_number > _num_devices)
    {
        rtn_val = -1;
    }
    else if(device_number == 0)
    {
        rtn_val = -2;
    }
    else
    {
        rtn_val = 0;
        
        //writes every digit of given device to 0
        for(idx = 0; idx < 8; idx++)
        {
            if(rtn_val == 0)
            {
                rtn_val = write_digit(device_number, (idx + 1), 0);
            }
        }
    }
    
    return(rtn_val);
}


void Max7219::display_all_on(void)
{
    uint8_t idx, idy;
    
    //writes every digit of every device to 0xFF
    for(idx = 0; idx < _num_devices; idx++)
    {
        for(idy = 0; idy < MAX7219_DIGIT_7; idy++)
        {
            write_digit((idx + 1), (idy + 1), 0xFF);
        }
    }
}
    
    
void Max7219::display_all_off(void)
{
    uint8_t idx, idy;
    
    //writes every digit of every device to 0
    for(idx = 0; idx < _num_devices; idx++)
    {
        for(idy = 0; idy < MAX7219_DIGIT_7; idy++)
        {
            write_digit((idx + 1), (idy + 1), 0);
        }
    }
}

