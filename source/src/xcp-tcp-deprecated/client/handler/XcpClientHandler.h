/**
 * Copyright (C) 2013-2015
 *
 * @author jxfengzi@gmail.com
 * @date   2013-11-19
 *
 * @file   XcpClientHandler.h
 *
 * @remark
 *
 */


#ifndef __XCP_CLIENT_HANDLER_H__
#define __XCP_CLIENT_HANDLER_H__

#include <tiny_base.h>
#include <channel/ChannelHandler.h>
#include <device/Device.h>

TINY_BEGIN_DECLS


#define XcpClientHandler_Name  "XcpClientHandler"

TINY_LOR
ChannelHandler * XcpClientHandler(Device *device);


TINY_END_DECLS

#endif /* __XCP_CLIENT_HANDLER_H__ */