/* ====================================================================
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2001 The Apache Software Foundation.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Apache" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 *
 * Originally developed by Aaron Bannert and Justin Erenkrantz, eBuilt.
 */

#include "flood_easy_reports.h"
#include <apr.h>
#include <apr_portable.h>
#include <apr_strings.h>
#include <unistd.h>

extern apr_file_t *local_stdout;
extern apr_file_t *local_stderr;

typedef struct easy_report_t {
    apr_pool_t *pool;
} easy_report_t;

apr_status_t easy_report_init(report_t **report, config_t *config, 
                              const char *profile_name, apr_pool_t *pool)
{
    easy_report_t *easy;

    easy = apr_palloc(pool, sizeof(easy_report_t));
   
    if (!easy)
        return APR_EGENERAL;
 
    apr_pool_create(&easy->pool, pool);

    *report = easy;
    return APR_SUCCESS;
}

apr_status_t easy_process_stats(report_t *report, int verified, request_t *req, response_t *resp, flood_timer_t *timer)
{
    easy_report_t* easy;
    char *foo;

    easy = (easy_report_t*)report;

    foo = apr_psprintf(easy->pool, "%" APR_INT64_T_FMT " %" APR_INT64_T_FMT 
                       " %" APR_INT64_T_FMT " %" APR_INT64_T_FMT 
                       " %" APR_INT64_T_FMT,
                       timer->begin, timer->connect, timer->write, 
                       timer->read, timer->close);

    switch (verified)
    {
    case FLOOD_VALID:
        foo = apr_pstrcat(easy->pool, foo, " OK", NULL);
        break;
    case FLOOD_INVALID:
        foo = apr_pstrcat(easy->pool, foo, " FAIL", NULL);
        break;
    default:
        foo = apr_psprintf(easy->pool, "%s %d", foo, verified);
    }

#if APR_HAS_THREADS
    foo = apr_psprintf(easy->pool, "%s %d %s", foo, apr_os_thread_current(), 
                       req->uri);
#else
    foo = apr_psprintf(easy->pool, "%s %d %s", foo, getpid(), req->uri);
#endif

    apr_file_printf(local_stdout, "%s\n", foo);

    return APR_SUCCESS;
}

apr_status_t easy_report_stats(report_t *report)
{
    return APR_SUCCESS;
}

apr_status_t easy_destroy_report(report_t *report)
{
    easy_report_t *easy;
    easy = (easy_report_t*)report;
    apr_pool_destroy(easy->pool);
    return APR_SUCCESS;
}
