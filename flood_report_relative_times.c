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

#include "flood_report_relative_times.h"
#include <apr.h>
#include <apr_portable.h>
#include <apr_strings.h>

extern apr_file_t *local_stdout;
extern apr_file_t *local_stderr;

typedef void relative_times_report_t;

apr_status_t relative_times_report_init(report_t **report, config_t *config, 
                              const char *profile_name, apr_pool_t *pool)
{
    *report = apr_palloc(pool, sizeof(relative_times_report_t));

    return APR_SUCCESS;
}

apr_status_t relative_times_process_stats(report_t *report, int verified, request_t *req, response_t *resp, flood_timer_t *timer)
{
#define FLOOD_PRINT_BUF 256
    apr_size_t buflen;
    char buf[FLOOD_PRINT_BUF];

    buflen = apr_snprintf(buf, FLOOD_PRINT_BUF,
                          "%" APR_INT64_T_FMT " %" APR_INT64_T_FMT
                          " %" APR_INT64_T_FMT " %" APR_INT64_T_FMT " %" APR_INT64_T_FMT,
                          timer->begin,
                          timer->connect - timer->begin,
                          timer->write - timer->begin,
                          timer->read - timer->begin,
                          timer->close - timer->begin);

    switch (verified)
    {
    case FLOOD_VALID:
        apr_snprintf(buf+buflen, FLOOD_PRINT_BUF-buflen, " OK ");
        break;
    case FLOOD_INVALID:
        apr_snprintf(buf+buflen, FLOOD_PRINT_BUF-buflen, " FAIL ");
        break;
    default:
        apr_snprintf(buf+buflen, FLOOD_PRINT_BUF-buflen, " %d ", verified);
    }

    /* FIXME: this call may need to be in a critical section */
    apr_file_printf(local_stdout, "%s %ld %s\n", buf, apr_os_thread_current(), req->uri);

    return APR_SUCCESS;
}

apr_status_t relative_times_report_stats(report_t *report)
{
    return APR_SUCCESS;
}

apr_status_t relative_times_destroy_report(report_t *report)
{
    return APR_SUCCESS;
}