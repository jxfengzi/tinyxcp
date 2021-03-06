/**
 * Copyright (C) 2013-2015
 *
 * @author jxfengzi@gmail.com
 * @date   2013-11-19
 *
 * @file   CommandReader.c
 *
 * @remark
 *
 */

#include "CommandReader.h"
#include "device/IotService.h"

#ifdef _WIN32
#else
#include <tiny_socket.h>
#include <XcpMessage.h>
#include <product/operator/ProductOperator.h>

#endif

static int g_loop = 0;
static Product *g_product = NULL;

static void cmd_help(void)
{
    fprintf(stdout, "\n------------ help --------------\n");
    fprintf(stdout, "h       -- show help information\n");
    fprintf(stdout, "x       -- exit\n");
    fprintf(stdout, "reset   -- reset access key\n");
    fprintf(stdout, "getkey  -- get access key\n");
    fprintf(stdout, "s       -- switch on/off\n");
}

static void onResetAccessKeyResult(XcpMessage *result, void *ctx)
{
    if (result->iq.type == IQ_TYPE_ERROR)
    {
        printf("onResetAccessKeyResult: ERROR!\n");
        return;
    }

    printf("onResetAccessKeyResult: OK!\n");
}

static void cmd_reset_access_key(void)
{
    IotService_ResetAccessKey("helloworld", onResetAccessKeyResult, NULL);
}

static void onGetAccessKeyResult(XcpMessage *result, void *ctx)
{
    if (result->iq.type == IQ_TYPE_ERROR)
    {
        printf("onGetAccessKeyResult: ERROR!\n");
        return;
    }

    printf("key: %s\n", result->iq.content.result.content.getAccessKey.key);
}

static void cmd_get_access_key(void)
{
    IotService_GetAccessKey(onGetAccessKeyResult, NULL);
}

static void cmd_switch_onoff(void)
{
    do
    {
        JsonValue *value = JsonValue_NewBoolean(true);
        if (value == NULL)
        {
            break;
        }

        PropertyOperation *o = PropertyOperation_NewValue(NULL, 8, 10, value);
        if (o == NULL)
        {
            JsonValue_Delete(value);
            break;
        }

        TinyRet ret = Product_DoChangePropertyValue(g_product, o);
        if (RET_FAILED(ret))
        {
            printf("error: %s\n", tiny_ret_to_str(ret));
            PropertyOperation_Delete(o);
            break;
        }
    } while (false);
}

static void cmd_exit(void)
{
    g_loop = 0;
}

typedef void (*cb_command)(void);

struct _cmd_exec
{
    const char * name;
    cb_command exec;
};

struct _cmd_exec cmd_exec[] =
        {
                {"h",       cmd_help},
                {"x",       cmd_exit},
                {"reset",   cmd_reset_access_key},
                {"getkey",  cmd_get_access_key},
                {"s",       cmd_switch_onoff},
        };

static
void command(const char *buf)
{
    int i   = 0;
    int len = DIM(cmd_exec);

    for (i=0; i < len; i++)
    {
        if (strcmp(cmd_exec[i].name, buf) == 0)
        {
            cmd_exec[i].exec();
            break;
        }
    }
}

#ifdef _WIN32
void WaitingForUserCommand(void)
{
    int ret = 0;
    char buf[1024];

    g_loop = 1;

    while (g_loop)
    {
        memset(buf, 0, 1024);

        printf("> ");
        ret = scanf("%s", buf);
		command(buf);
    }
}
#else /* Linux */
void cmd_post_select(fd_set *p_read_set, fd_set *p_write_set, fd_set *p_error_set)
{
    char buf[2048];
    size_t nbytes;
    ssize_t bytes_read;
    int fd = 0;

    nbytes = sizeof(buf);
    memset(buf, 0, nbytes);

    if (FD_ISSET(fd, p_read_set))
    {
        //bytes_read = read(fd, (void*)buf, nbytes);
        bytes_read = read(fd, (void*)buf, nbytes);
        /* remove '\n' */
        if (strlen(buf) > 0)
        {
            buf[strlen(buf) - 1] = 0;
        }

        command(buf);
    }
}

void cmd_pre_select(int *p_max_soc, fd_set *p_read_set, fd_set *p_write_set, fd_set *p_error_set)
{
    int soc = 0;

    FD_SET(soc, p_error_set);
    FD_SET(soc, p_read_set);

    if (*p_max_soc < soc)
    {
        *p_max_soc = soc;
    }
}

void WaitingForUserCommand(Product *product)
{
    fd_set  read_set;
    fd_set  write_set;
    fd_set  error_set;
    int     max_soc = 0;

    g_loop = 1;
    g_product = product;

    fprintf(stdout, "pid: %d\n", getpid());

    while (g_loop)
    {
        int ret = write(1, "> ", 2);

        FD_ZERO(&read_set);
        FD_ZERO(&write_set);
        FD_ZERO(&error_set);

        cmd_pre_select(&max_soc, &read_set, &write_set, &error_set);

        select(max_soc + 1, &read_set, &write_set, &error_set, NULL);

        cmd_post_select(&read_set, &write_set, &error_set);
    }

    fprintf(stdout, "\n");

    printf("-------------- cmd_run -----------------\n");
}
#endif /* Linux */
