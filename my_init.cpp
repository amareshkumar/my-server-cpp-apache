/*
 * my_init.cpp
 *
 *  Created on: 20-Dec-2014
 *      Author: amaresh
 */

#include "my_init.h"

//part 1
static void my_hooks(apr_pool_t *pool) {
/* create request processing hooks as required */
}
ap_hook_handler(my_handler, NULL, NULL, APR_HOOK_MIDDLE) ;
module AP_MODULE_DECLARE_DATA my_module = {
	STANDARD20_MODULE_STUFF, /* macro to ensure version consistency */
	my_dir_conf, /* create per-directory configuration record */
	my_dir_merge, /* merge per-directory configuration records */
	my_server_conf, /* create per-server configuration record */
	my_server_merge, /* merge per-server configuration records */
	my_cmds, /* configuration directives */
	my_hooks /* register modules functions with the core */
};
static int my_handler(request_rec *r) {
/* do something with the request */
}

int my_func(request_rec* r) {
/* implement the request processing hook here */
}
//Configuration Handlers
static const char* my_cfg(cmd_parms* cmd, void* cfg, /* args */ )

//for allocating memory, isnterad of malloc, we use followings. it automatically free memory and faster
//than malloc
//mytype* myvar = apr_palloc(pool, sizeof(mytype)) ;

static int my_pre_config(apr_pool_t* pool, apr_pool_t* plog, apr_pool_t* ptemp)
//For most purposes, just use the first pool argument. ptemp is suitable for resources
//used during configuration, but will be destroyed before Apache goes into operational
//66 Chapter 3 • The Apache Portable Runtime
//mode. plog remains active for the lifetime of the server, but is cleaned up each time
//the configuration is read.


//Child init
static void my_child_init(apr_pool_t* pool, server_rec* s)
//The child pool is the first argument.
//Monitor
static int my_monitor(apr_pool_t* pool)
//The monitor is a special case: It runs in the parent process and is not tied to any
//time-limited structure.


/*
 * Apache provides four data struct modules:
• apr_table provides tables and arrays.
• apr_hash provides hash tables.
• apr_queue provides first in, first out (FIFO) queues.
• apr_ring provides a ring struct, which is also the basis for APR bucket
  brigades.


  apr_hash_t also stores key/value pairs, but is a lower-level data type than
apr_table_t. It has two advantages:
1. Keys and values can be of any data type (and, unlike with tables, are case
sensitive).
2. Hash tables scale more efficiently as the number of elements grows.
Unlike the array and table, the hash table has no initial size. The most commonly
used operations are insertion and lookup. Other operations supported include iteration,
copy, overlay, and merge.
apr_hash_t* hash = apr_hash_make(pool) ;
/* key and value are pointers to arbitrary data types */


/*apr_hash_set(hash, key, sizeof(*key), value) ;
value = apr_hash_get(hash, key, sizeof(*key)) ;
72 Chapter 3 • The Apache Portable Runtime
There is one special case we commonly encounter: where the key is a character
string. To ensure the proper string comparison semantics are used, we should use
the macro APR_HASH_KEY_STRING in place of the size argument.
 *
 * */

svr_cfg* my_svr_cfg =
ap_get_module_config(server->module_config, &my_module);
dir_cfg* my_dir_cfg =
ap_get_module_config(request->per_dir_config, &my_module);

static int my_early_hook(request_rec* r) {
req_cfg* my_req ;
...
my_req = apr_palloc(r->pool, sizeof(req_cfg)) ;
ap_set_module_config(r->request_config, &my_module, my_req);
/*
 *
 * }
 */
}

static int my_later_hook(request_rec* r) {
	req_cfg* my_req = ap_get_module_config(r->request_config, &my_module);
	/* Now we have all the data and we can do what we want with it */
}

static int my_other_hook(request_rec* r) {
	req_cfg* my_req;
	...
	my_req = ap_get_module_config(r->request_config, &my_module);
	if (my_req == NULL) {
		/* It hasn't been set yet */
		my_req = apr_palloc(r->pool, sizeof(req_cfg)) ;
		ap_set_module_config(r->request_config, &my_module, my_req);
		/* Set the data fields of my_req as required */
	}
/* Now we have my_req, whether or not it was already set */
}

static void register_hooks(apr_pool_t *pool)
{
	my_lib_init() ;
	apr_pool_cleanup_register(pool, NULL, my_lib_terminate,
	apr_pool_cleanup_null);
	/* and of course register whatever this module exports */
}

//End of part 1

//Part 2

static int helloworld_handler(request_rec *r)
{
	if (!r->handler || (strcmp(r->handler, "helloworld") != 0)) {
		return DECLINED;
	}
	if (r->method_number != M_GET) {
		return HTTP_METHOD_NOT_ALLOWED;
	}
		ap_set_content_type(r, "text/html;charset=ascii");
		ap_rputs("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\">\n", r);
		ap_rputs("<html><head><title>Apache HelloWorld " "Module</title></head>", r);
		ap_rputs("<body><h1>Hello World!</h1>", r);
		ap_rputs("<p>This is the Apache HelloWorld module!</p>", r);
		ap_rputs("</body></html>", r);
	return OK;
}

static void helloworld_hooks(apr_pool_t *pool)
{
	ap_hook_handler(helloworld_handler, NULL, NULL, APR_HOOK_MIDDLE);
}

//module AP_MODULE_DECLARE_DATA my_module1 = {
//	STANDARD20_MODULE_STUFF,
//	some_dir_cfg, /* create per-directory config struct */
//	some_dir_merge, /* merge per-directory config struct */
//	some_svr_cfg, /* create per-host config struct */
//	some_svr_merge, /* merge per-host config struct */
//	some_cmds, /* configuration directives for this module */
//	some_hooks /* register module's hooks/etc. with the core */
//}
module AP_MODULE_DECLARE_DATA helloworld_module = {
	STANDARD20_MODULE_STUFF,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	helloworld_hooks
};
