#include "network_interface.h"
#include "network_module.h"
#include "network_module_namespace.h"
#include "user_common_define.h"
#include "Common/concurrent.h"
#include <stdio.h>

USING_NAMESPACE_NETWORK_MODULE
static Network_module *g_engine = NULL;

INIT_ENGINE_RESULT init_engine()
{
    g_engine = new Network_module;
	if (g_engine->init_log(MODULE_NAME, LOG_PATH, LOG_LEVEL) != Synchronized::success)
	{
		return INIT_ENGINE_LOG_ERROR;
	}

	return INIT_ENGINE_SUCC;
}

START_ENGINE_RESULT start_engine()
{
	if (!g_engine)
	{
		return START_ENGINE_NOT_INIT;
	}

	g_engine->run();
	return START_ENGINE_PROCESS;
}

void stop_engine()
{
	g_engine->stop_engine();
	delete g_engine;
	g_engine = NULL;
}

void regist_callback()
{
    printf("regist\n");
}

char *get_local_guid()
{
	if (!g_engine)
	{
		return NULL;
	}

	return g_engine->GetMyGUID()->ToString();
}
