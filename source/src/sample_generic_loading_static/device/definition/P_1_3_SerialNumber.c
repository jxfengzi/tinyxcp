/**
 * Copyright (C) 2013-2015
 *
 * @author jxfengzi@gmail.com
 * @date   2013-11-19
 *
 * @file   P_1_3_SerialNumber.h
 *
 * @remark
 *
 */

#include "P_1_3_SerialNumber.h"

Property * P_1_3_SerialNumber(void)
{
    return Property_NewInstance(6, "xiot-spec", "serial-number", 0x00000003, NULL, FORMAT_STRING, ACCESS_READ, NONE);
}