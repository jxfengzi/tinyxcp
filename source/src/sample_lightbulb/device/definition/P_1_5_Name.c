/**
 * Copyright (C) 2013-2015
 *
 * @author jxfengzi@gmail.com
 * @date   2013-11-19
 *
 * @file   P_1_5_Name.h
 *
 * @remark
 *
 */

#include "P_1_5_Name.h"

Property * P_1_5_Name(void)
{
    return Property_NewInstance(5, "homekit-spec", "model", 0x00000023, NULL, FORMAT_STRING, ACCESS_READ, NONE);
}