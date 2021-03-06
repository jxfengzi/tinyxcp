/**
 * Copyright (C) 2013-2015
 *
 * @author jxfengzi@gmail.com
 * @date   2013-11-19
 *
 * @file   XcpwsClientRuntime.c
 *
 * @remark
 *
 */

#include "XcpwsClientRuntime.h"
#include "client/XcpwsClient.h"
#include <tiny_log.h>
#include <tiny_malloc.h>
#include <codec-message/CustomDataType.h>

#define TAG     "XcpwsClientRuntime"

TINY_LOR
static TinyRet XcpwsClientRuntime_Initialize(IotRuntime *thiz)
{
    LOG_D(TAG, "XcpwsClientRuntime_Initialize");

    return TINY_RET_OK;
}

TINY_LOR
static TinyRet XcpwsClientRuntime_Destroy(IotRuntime *thiz)
{
    LOG_D(TAG, "XcpwsClientRuntime_Destroy");

    return TINY_RET_OK;
}

TINY_LOR
static TinyRet XcpwsClientRuntime_Run(IotRuntime *thiz, Bootstrap *bootstrap, Product *product)
{
    TinyRet ret = TINY_RET_OK;
    Channel *channel = NULL;

    LOG_D(TAG, "XcpwsClientRuntime_Run");

    do
    {
        product->context = thiz->context;

        channel = XcpwsClient_New(product, "39.106.31.22", 80);
        if (channel == NULL)
        {
            LOG_D(TAG, "XcpwsClient_New failed");
            ret = TINY_RET_E_INTERNAL;
            break;
        }

        ret = Bootstrap_AddChannel(bootstrap, channel);
        if (RET_FAILED(ret))
        {
            LOG_D(TAG, "Bootstrap_AddChannel failed");
            channel->_onRemove(channel);
            break;
        }

        thiz->channel = channel;
    } while (0);

    return ret;
}

TINY_LOR
static TinyRet XcpwsClientRuntime_Stop(IotRuntime *thiz)
{
    LOG_D(TAG, "XcpwsClientRuntime_Stop");
    return TINY_RET_OK;
}

TINY_LOR
IotRuntime * XcpwsClientRuntime_New(void)
{
    IotRuntime * thiz = IotRuntime_New();
    if (thiz != NULL)
    {
        thiz->Name           = "XcpwsClientRuntime",
        thiz->Initialize     = XcpwsClientRuntime_Initialize,
        thiz->Destroy        = XcpwsClientRuntime_Destroy,
        thiz->Run            = XcpwsClientRuntime_Run,
        thiz->Stop           = XcpwsClientRuntime_Stop,
        thiz->channel        = NULL;
        thiz->context        = NULL;
    }

    return thiz;
}

TINY_LOR
TinyRet XcpwsClientRuntime_SendQuery(IotRuntime *thiz, XcpMessage *query, XcpMessageHandler handler, void *ctx)
{
    return XcpwsClient_SendQuery(thiz->channel, query, handler, ctx);
}