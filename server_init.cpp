/*
 * server_init.cpp
 *
 *  Created on: 20-Dec-2014
 *      Author: amaresh
 */


#include "server_init.h"
//#include "log_dbg.hpp"
#include <string.h>

struct MODULE_HANDLER
{
	int someFlag;
};



static int printitem(void* rec, const char* key, const char* value) {
  /* rec is a userdata pointer.  We'll pass the request_rec in it */
  request_rec* r = static_cast<request_rec*>(rec) ;
  ap_rprintf(r, "<tr><th scope=\"row\">%s</th><td>%s</td></tr>\n",
	ap_escape_html(r->pool, key), ap_escape_html(r->pool, value)) ;
  /* Zero would stop iterating; any other return value continues */
  return 1 ;
}

static void printtable(request_rec* r, apr_table_t* t,
	const char* caption, const char* keyhead, const char* valhead) {

  /* print a table header */
  ap_rprintf(r, "<table><caption>%s</caption><thead>"
	"<tr><th scope=\"col\">%s</th><th scope=\"col\">%s"
	"</th></tr></thead><tbody>", caption, keyhead, valhead) ;

  /* Print the data: apr_table_do iterates over entries with our callback */
  apr_table_do(printitem, r, t, NULL) ;

  /* and finish the table */
  ap_rputs("</tbody></table>\n", r) ;
}

static int redirect(request_rec *r,string url){
    //log_dbg("Into the redirect");
    apr_table_set(r->headers_out,"location","https://www.wslogin.visteon.com/login.cgi?WslIP=136.17.228.139&back=http://localhost/wsl_agent" );
    //return HTTP_MOVED_TEMPORARILY;

}

/* Look through 'Cookie' header for indicated cookie; extract it
 */
static const char * extract_cookie(request_rec *r, const char *szCookie_name){

    const char *szRaw_cookie = NULL, *szRaw_cookie_start = NULL, *szRaw_cookie_end;
    char *szCookie;
    int i;

    /* get cookie string */

    szRaw_cookie = apr_table_get(r->headers_in, "Cookie");

    //
    ap_rprintf(r, "%s The value of szRaw_cookie: ", szRaw_cookie);
    //ap_rputs (szRaw_cookie, r);
    //

    //printf("%s", szRaw_cookie);


    if (szRaw_cookie){
        //log_dbg("--Cookie Found--");
        //ap_rputs ("--Cookie Found--", r);
        ap_rprintf (r, "--Cookie Found--");
        exit (1);

        do {
	/* search cookie name in cookie string */
	if ((szRaw_cookie = strstr(szRaw_cookie, szCookie_name)) == 0)
	    return 0;
	szRaw_cookie_start = szRaw_cookie;
	/* search '=' */
	if ((szRaw_cookie = strchr(szRaw_cookie, '=')) == 0)
	    return 0;
    } while (strncmp(szCookie_name, szRaw_cookie_start, szRaw_cookie - szRaw_cookie_start) != 0);

    /* skip '=' */
    szRaw_cookie++;

    }else{
        // log_dbg("No WSLCookie Found");
        //ap_rputs ("%sNo WSLCookie Found", r);
        ap_rprintf (r, "No WSLCookie Found");
        redirect(r,"https://www.wslogin.visteon.com/login.cgi?WslIP=136.17.228.139&back=http://localhost/wsl_agent");
	return 0;
    }

    /* loop to search cookie name in cookie header */
    szRaw_cookie;
}


EXTERN_C_FUNC
int authenticate_user( request_rec* inpRequest)
{
    int nReturnVal = DECLINED;
    const char *redirectFlag = NULL;

    apr_array_header_t *pairs = NULL;
    apr_off_t len;
    apr_size_t size;
    int res;
    int i = 0;
    char *buffer;
    keyValuePair* kvp;
	char UNAME[50];
	char PWD[50];

//Key=cdsid
//Key=pw

    res = ap_parse_form_data(inpRequest, NULL, &pairs, -1, HUGE_STRING_LEN);
    //if (res != OK || !pairs) return NULL; /* Return NULL if we failed or if there are is no POST data */
    kvp = apr_pcalloc(inpRequest->pool, sizeof(keyValuePair) * (pairs->nelts + 1));
    while (pairs && !apr_is_empty_array(pairs)) {
        ap_form_pair_t *pair = (ap_form_pair_t *) apr_array_pop(pairs);
        apr_brigade_length(pair->value, 1, &len);
        size = (apr_size_t) len;
        buffer = apr_palloc(inpRequest->pool, size + 1);
        apr_brigade_flatten(pair->value, buffer, &size);
        buffer[len] = 0;
        kvp[i].key = apr_pstrdup(inpRequest->pool, pair->name);
        kvp[i].value = buffer;
		if(!strcmp(kvp[i].key,"cdsid"))
			strcpy(UNAME,kvp[i].value);
		if(!strcmp(kvp[i].key,"pw"))
			strcpy(PWD,kvp[i].value);
        i++;
    }

    /*if ( inpRequest->handler != NULL && strcmp( inpRequest->handler, "wsl_agent" ) == 0 )*/
    {
	    ap_set_content_type(inpRequest, "text/plain");
        //ap_rprintf("WSL Initiated\n\n", inpRequest);

        //find cookie
        ap_rprintf(inpRequest,"\nUser Name = %s\n\n", UNAME);
        //redirectFlag = extract_cookie(inpRequest,"Cookie");
        ap_rprintf(inpRequest,"\nPwd = %s\n\n", PWD);

        /*if(!redirectFlag){

            nReturnVal = HTTP_MOVED_TEMPORARILY;
        }*/

        nReturnVal = OK;
    }

//    printtable(inpRequest, inpRequest->headers_in, "Request Headers", "Header", "Value") ;
//    printtable(inpRequest, inpRequest->headers_out, "Response Headers", "Header", "Value") ;
//    printtable(inpRequest, inpRequest->subprocess_env, "Environment", "Variable", "Value") ;

    return nReturnVal;
}

static int wsl_post_config(apr_pool_t *pool, apr_pool_t *plog,apr_pool_t *ptemp, server_rec *s)
{
	MODULE_HANDLER *mh = ap_get_module_config(r->per_dir_config, &server_auth_module);
	mh->someFlag = 1;
	init_func(s);
	return OK ;
}

EXTERN_C_FUNC
int authenticate_user( request_rec* inpRequest)
{
	int nReturnVal = DECLINED;
	MODULE_HANDLER *mh = ap_get_module_config(r->per_dir_config, &server_auth_module);

	if ( inpRequest->handler != NULL && mh->someFlag)
	{
		login_func(inpRequest);
		nReturnVal = OK;
		mh->someFlag = 0;
	}
	else if(inpRequest->handler != NULL)
	{
		auth_func(inpRequest);
		nReturnVal = OK;
	}
	else
		nReturnVal = HTTP_MOVED_TEMPORARILY;

	return nReturnVal;
}

EXTERN_C_FUNC
static void register_hooks(apr_pool_t *pool)
{
	//ap_hook_child_init(my_child_init, NULL, NULL, APR_HOOK_MIDDLE);
	ap_hook_post_config(wsl_post_config, NULL, NULL, APR_HOOK_MIDDLE) ;
	ap_hook_handler(authenticate_user, NULL, NULL, APR_HOOK_MIDDLE) ;
}



static const command_rec Modulehandler[] ={
  AP_INIT_FLAG("ModuleHandler",
               ap_set_flag_slot,
               (void *)APR_OFFSETOF(MODULE_HANDLER, someFlag),,
               RSRC_CONF,
               "Decision maker for handler"
               ),
	{ NULL }
};


EXTERN_C_BLOCK_BEGIN
module AP_MODULE_DECLARE_DATA server_auth_module =
{
    STANDARD20_MODULE_STUFF,
    NULL,
    NULL,
    GET_MODULE_HANDLER,
    NULL,
    Modulehandler,
    register_hooks
};
EXTERN_C_BLOCK_END
