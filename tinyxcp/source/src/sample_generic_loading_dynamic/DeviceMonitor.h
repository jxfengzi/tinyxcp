/**
* Copyright (C) 2013-2015
*
* @author jxfengzi@gmail.com
* @date   2013-11-19
*
* @file   DeviceMonitor.h
*
* @remark
*
*/

#ifndef __DeviceMonitor_H__
#define __DeviceMonitor_H__

#include <device/Device.h>

TINY_BEGIN_DECLS


TinyRet StartDeviceMonitor(Device *device);

TinyRet StopDeviceMonitor(void);


TINY_END_DECLS

#endif /* __DeviceMonitor_H__ */