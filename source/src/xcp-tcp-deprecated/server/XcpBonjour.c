/**
 * Copyright (C) 2013-2015
 *
 * @author jxfengzi@gmail.com
 * @date   2013-11-19
 *
 * @file   XcpBonjour.c
 *
 * @remark
 *
 */

#include <tiny_malloc.h>
#include <tiny_log.h>
#include <channel/ChannelHandler.h>
#include <channel/SocketChannel.h>
#include <channel/multicast/MulticastChannel.h>
#include <codec/DnsMessageCodec.h>
#include <MdnsHandler.h>
#include <MdnsConstant.h>
#include "XcpBonjour.h"

#define TAG     "XcpBonjour"


TINY_LOR
static void XcpBonjourInitializer(Channel *channel, void *ctx)
{
    ChannelHandler *handler = MdnsHandler();
    MdnsHandler_Register(handler, (ServiceInfo *) ctx);
    SocketChannel_AddLast(channel, DnsMessageCodec());
    SocketChannel_AddLast(channel, handler);
}

TINY_LOR
Channel * XcpBonjour_New(Device *device)
{
    Channel *thiz = NULL;

    do
    {
        #if (defined(ESP32) || defined(ESP8266))
        bool reuse = false;
        #else
        bool reuse = true;
        #endif

        ServiceInfo info;

        thiz = MulticastChannel_New();
        if (thiz == NULL)
        {
            LOG_D(TAG, "MulticastChannel_New failed");
            break;
        }

        if (RET_FAILED(ServiceInfo_Construct(&info)))
        {
            LOG_D(TAG, "ServiceInfo_Construct failed");
            thiz = NULL;
            break;
        }

        ServiceInfo_Initialize(&info, "test", "._xcp._tcp.local", device->ip, device->port);
        ServiceInfo_SetTXTByString(&info, "did", device->did);
        ServiceInfo_SetTXTByInteger(&info, "pid", device->productId);
        ServiceInfo_SetTXTByInteger(&info, "ver", device->productVersion);

        MulticastChannel_Initialize(thiz, XcpBonjourInitializer, &info);
        ServiceInfo_Dispose(&info);

        if (RET_FAILED(MulticastChannel_Join(thiz, device->ip, MDNS_GROUP, MDNS_PORT, reuse)))
        {
            LOG_D(TAG, "MulticastChannel_Join failed");
            thiz->_onRemove(thiz);
            thiz = NULL;
            break;
        }
    } while (0);

    return thiz;
}
