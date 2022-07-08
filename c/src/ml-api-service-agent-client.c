/* SPDX-License-Identifier: Apache-2.0 */
/**
 * Copyright (c) 2022 Samsung Electronics Co., Ltd. All Rights Reserved.
 *
 * @file ml-api-service-agent-client.c
 * @date 20 Jul 2022
 * @brief agent (dbus) implementation of NNStreamer/Service C-API
 * @see https://github.com/nnstreamer/nnstreamer
 * @author Yongjoo Ahn <yongjoo1.ahn@samsung.com>
 * @bug No known bugs except for NYI items
 */

#include <gio/gio.h>

#include "ml-api-internal.h"
#include "ml-api-service.h"
#include "pipeline-dbus.h"

/**
 * @brief Structure for ml_service_h
 */
typedef struct
{
  gint64 id;
  gchar *service_name;
} ml_service_s;

/**
 * @brief Internal function to get proxy of the pipeline d-bus interface
 */
static MachinelearningServicePipeline *
_get_proxy_new_for_bus_sync (void)
{
  /** @todo deal with GError */
  return
      machinelearning_service_pipeline_proxy_new_for_bus_sync
      (G_BUS_TYPE_SESSION, G_DBUS_PROXY_FLAGS_NONE,
      "org.tizen.machinelearning.service",
      "/Org/Tizen/MachineLearning/Service/Pipeline", NULL, NULL);
}

int
ml_service_launch_pipeline (const char *name, ml_service_h * h)
{
  ml_service_s *server;
  gint out_return_code;
  gint64 out_id;
  MachinelearningServicePipeline *mlsp;

  check_feature_state (ML_FEATURE_SERVICE);

  if (!h)
    _ml_error_report_return (ML_ERROR_INVALID_PARAMETER,
        "The parameter, 'h' is NULL. It should be a valid ml_service_h");

  server = g_new0 (ml_service_s, 1);

  if (server == NULL)
    _ml_error_report_return (ML_ERROR_OUT_OF_MEMORY,
        "Failed to allocate memory for the service_server. Out of memory?");


  mlsp = _get_proxy_new_for_bus_sync ();
  machinelearning_service_pipeline_call_launch_pipeline_sync (mlsp, name,
      &out_return_code, &out_id, NULL, NULL);

  if (out_return_code != 0) {
    g_free (server);
    g_object_unref (mlsp);
    _ml_error_report_return (out_return_code,
        "Failed to launch pipeline, please check its integrity.");
  }

  server->id = out_id;
  server->service_name = g_strdup (name);
  *h = (ml_service_h *) server;

  g_object_unref (mlsp);

  return ML_ERROR_NONE;
}

/**
 * @brief Start the pipeline of given ml_service_h
 */
int
ml_service_start_pipeline (ml_service_h h)
{
  gint out_result;
  ml_service_s *server = (ml_service_s *) h;
  MachinelearningServicePipeline *mlsp;

  check_feature_state (ML_FEATURE_SERVICE);

  if (!h)
    _ml_error_report_return (ML_ERROR_INVALID_PARAMETER,
        "The parameter, 'h' is NULL. It should be a valid ml_service_h");

  mlsp = _get_proxy_new_for_bus_sync ();
  machinelearning_service_pipeline_call_start_pipeline_sync (mlsp, server->id,
      &out_result, NULL, NULL);

  g_object_unref (mlsp);

  return ML_ERROR_NONE;
}

/**
 * @brief Stop the pipeline of given ml_service_h
 */
int
ml_service_stop_pipeline (ml_service_h h)
{
  gint out_result;
  ml_service_s *server = (ml_service_s *) h;
  MachinelearningServicePipeline *mlsp;

  check_feature_state (ML_FEATURE_SERVICE);

  if (!h)
    _ml_error_report_return (ML_ERROR_INVALID_PARAMETER,
        "The parameter, 'h' is NULL. It should be a valid ml_service_h");

  mlsp = _get_proxy_new_for_bus_sync ();
  machinelearning_service_pipeline_call_stop_pipeline_sync (mlsp, server->id,
      &out_result, NULL, NULL);

  g_object_unref (mlsp);

  return ML_ERROR_NONE;
}

/**
 * @brief Destroy the pipeline of given ml_service_h
 */
int
ml_service_destroy_pipeline (ml_service_h h)
{
  gint out_result;
  ml_service_s *server = (ml_service_s *) h;
  MachinelearningServicePipeline *mlsp;

  check_feature_state (ML_FEATURE_SERVICE);

  if (!h)
    _ml_error_report_return (ML_ERROR_INVALID_PARAMETER,
        "The parameter, 'h' is NULL. It should be a valid ml_service_h");

  mlsp = _get_proxy_new_for_bus_sync ();
  machinelearning_service_pipeline_call_destroy_pipeline_sync (mlsp, server->id,
      &out_result, NULL, NULL);

  g_object_unref (mlsp);

  g_free (server->service_name);
  g_free (server);
  return ML_ERROR_NONE;
}

/**
 * @brief Return state of given ml_service_h
 */
int
ml_service_getstate_pipeline (ml_service_h h, ml_pipeline_state_e * state)
{
  gint out_result;
  gint _state;
  ml_service_s *server = (ml_service_s *) h;
  MachinelearningServicePipeline *mlsp;

  check_feature_state (ML_FEATURE_SERVICE);

  if (!h)
    _ml_error_report_return (ML_ERROR_INVALID_PARAMETER,
        "The parameter, 'h' is NULL. It should be a valid ml_service_h");

  mlsp = _get_proxy_new_for_bus_sync ();
  machinelearning_service_pipeline_call_get_state_sync (mlsp, server->id,
      &out_result, &_state, NULL, NULL);

  *state = (ml_pipeline_state_e) _state;

  g_object_unref (mlsp);

  return ML_ERROR_NONE;
}

/**
 * @brief Return the pipeline description of given ml_service_h
 */
int
ml_service_getdesc_pipeline (ml_service_h h, char **desc)
{
  gint out_result;
  ml_service_s *server = (ml_service_s *) h;
  MachinelearningServicePipeline *mlsp;

  check_feature_state (ML_FEATURE_SERVICE);

  if (!h)
    _ml_error_report_return (ML_ERROR_INVALID_PARAMETER,
        "The parameter, 'h' is NULL. It should be a valid ml_service_h");

  mlsp = _get_proxy_new_for_bus_sync ();
  machinelearning_service_pipeline_call_get_description_sync (mlsp, server->id,
      &out_result, desc, NULL, NULL);

  g_object_unref (mlsp);

  return ML_ERROR_NONE;
}