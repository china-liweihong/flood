/* Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Originally developed by Aaron Bannert and Justin Erenkrantz, eBuilt.
 */

#ifndef __flood_easy_reports_h
#define __flood_easy_reports_h

#include <apr_pools.h>

#include "flood_config.h"
#include "flood_profile.h"

apr_status_t easy_report_init(report_t **report, config_t *config, const char *profile_name, apr_pool_t *pool);

apr_status_t easy_process_stats(report_t *report, int verified, request_t *req, response_t *resp, flood_timer_t *timer);

apr_status_t easy_report_stats(report_t *report);

apr_status_t easy_destroy_report(report_t *report);

#endif  /* __flood_easy_reports_h */
