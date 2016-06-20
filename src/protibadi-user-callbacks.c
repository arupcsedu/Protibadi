/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "protibadi-user-callbacks.h"
#include "main.h"

telephony_handle_list_s handle_list;

int voice_call_noti_tbl[] = {
    TELEPHONY_NOTI_VOICE_CALL_STATUS_IDLE,
    TELEPHONY_NOTI_VOICE_CALL_STATUS_ACTIVE,
    TELEPHONY_NOTI_VOICE_CALL_STATUS_HELD,
    TELEPHONY_NOTI_VOICE_CALL_STATUS_DIALING,
    TELEPHONY_NOTI_VOICE_CALL_STATUS_ALERTING,
    TELEPHONY_NOTI_VOICE_CALL_STATUS_INCOMING,
};

void voice_call_status_noti_cb(telephony_h handle, telephony_noti_e noti_id, void *data,
                               void *user_data)
{
    int ret_value;
    telephony_call_h *call_list;
    unsigned int count = 0;

    switch (noti_id) {
    case TELEPHONY_NOTI_VOICE_CALL_STATUS_IDLE:
        dlog_print(DLOG_DEBUG, LOG_TAG, "Voice Call Status Idle [%d]", *(int *)data);
        break;

    case TELEPHONY_NOTI_VOICE_CALL_STATUS_ACTIVE:
        dlog_print(DLOG_DEBUG, LOG_TAG, "Voice Call Status Active [%d]", *(int *)data);
        break;

    case TELEPHONY_NOTI_VOICE_CALL_STATUS_HELD:
        dlog_print(DLOG_DEBUG, LOG_TAG, "Voice Call Status Held [%d]", *(int *)data);
        break;

    case TELEPHONY_NOTI_VOICE_CALL_STATUS_DIALING:
        dlog_print(DLOG_DEBUG, LOG_TAG, "Voice Call Status Dialing [%d]", *(int *)data);
        break;

    case TELEPHONY_NOTI_VOICE_CALL_STATUS_ALERTING:
        dlog_print(DLOG_DEBUG, LOG_TAG, "Voice Call Status Alerting [%d]", *(int *)data);
        break;

    case TELEPHONY_NOTI_VOICE_CALL_STATUS_INCOMING:
        dlog_print(DLOG_DEBUG, LOG_TAG, "Voice Call Status Incoming [%d]", *(int *)data);
        break;

    default:
        dlog_print(DLOG_DEBUG, LOG_TAG, "Unknown noti");
        break;
    }

    ret_value = telephony_call_get_call_list(handle_list.handle[0], &count, &call_list);
    if (ret_value != TELEPHONY_ERROR_NONE)
        dlog_print(DLOG_DEBUG, LOG_TAG, "telephony_call_get_call_list() failed [0x%x]", ret_value);
    else {
        int i;
        unsigned int handle_id = 0;
        char *number = NULL;
        telephony_call_type_e type = 0;
        telephony_call_status_e status = 0;
        telephony_call_direction_e direction = 0;
        bool conf_status = 0;

        for (i = 0; i < count; i++) {
            telephony_call_get_handle_id(call_list[i], &handle_id);
            telephony_call_get_number(call_list[i], &number);
            telephony_call_get_type(call_list[i], &type);
            telephony_call_get_status(call_list[i], &status);
            telephony_call_get_direction(call_list[i], &direction);
            telephony_call_get_conference_status(call_list[i], &conf_status);
            dlog_print(DLOG_DEBUG, LOG_TAG,
                       "id[%d] number[%s] type[%s] status[%s] direction[%s] conference_status[%s]",
                       handle_id, number,
                       type == TELEPHONY_CALL_TYPE_VOICE ? "VOICE" : type ==
                       TELEPHONY_CALL_TYPE_VIDEO ? "VIDEO" : "E911",
                       status == TELEPHONY_CALL_STATUS_IDLE ? "IDLE" : status ==
                       TELEPHONY_CALL_STATUS_ACTIVE ? "ACTIVE" : status ==
                       TELEPHONY_CALL_STATUS_HELD ? "HELD" : status ==
                       TELEPHONY_CALL_STATUS_DIALING ? "DIALING" : status ==
                       TELEPHONY_CALL_STATUS_ALERTING ? "ALERTING" : status ==
                       TELEPHONY_CALL_STATUS_INCOMING ? "INCOMING" : "UNKNOWN",
                       direction == TELEPHONY_CALL_DIRECTION_MO ? "MO" : "MT",
                       conf_status ? "TRUE" : "FALSE");

            free(number);
        }

        telephony_call_release_call_list(count, &call_list);
    }
}

char *_telephony_call_state_to_string(telephony_call_state_e voice_call_state)
{
    switch (voice_call_state) {
    case TELEPHONY_CALL_STATE_IDLE:
        return "TELEPHONY_CALL_STATE_IDLE";

    case TELEPHONY_CALL_STATE_CONNECTING:
        return "TELEPHONY_CALL_STATE_CONNECTING";

    case TELEPHONY_CALL_STATE_CONNECTED:
        return "TELEPHONY_CALL_STATE_CONNECTED";

    default:
        return "Unknown";
    }
}

char *_telephony_sim_state_to_string(telephony_sim_state_e sim_state)
{
    switch (sim_state) {
    case TELEPHONY_SIM_STATE_UNAVAILABLE:
        return "TELEPHONY_SIM_STATE_UNAVAILABLE";

    case TELEPHONY_SIM_STATE_LOCKED:
        return "TELEPHONY_SIM_STATE_LOCKED";

    case TELEPHONY_SIM_STATE_AVAILABLE:
        return "TELEPHONY_SIM_STATE_AVAILABLE";

    case TELEPHONY_SIM_STATE_UNKNOWN:
        return "TELEPHONY_SIM_STATE_UNKNOWN";

    default:
        return "Unknown";
    }
}

char *_telephony_network_state_to_string(telephony_network_service_state_e network_state)
{
    switch (network_state) {
    case TELEPHONY_NETWORK_SERVICE_STATE_IN_SERVICE:
        return "TELEPHONY_NETWORK_SERVICE_STATE_IN_SERVICE";

    case TELEPHONY_NETWORK_SERVICE_STATE_OUT_OF_SERVICE:
        return "TELEPHONY_NETWORK_SERVICE_STATE_OUT_OF_SERVICE";

    case TELEPHONY_NETWORK_SERVICE_STATE_EMERGENCY_ONLY:
        return "TELEPHONY_NETWORK_SERVICE_STATE_EMERGENCY_ONLY";

    default:
        return "Unknown";
    }
}

void video_call_state_noti_cb(telephony_h handle, telephony_noti_e noti_id, void *data,
                              void *user_data)
{
    telephony_call_state_e video_call_state = *(int *)data;
    char *video_call_state_string = _telephony_call_state_to_string(video_call_state);
    PRINT_MSG("Video call state: [%s]", video_call_state_string);
    dlog_print(DLOG_DEBUG, LOG_TAG, "Video call state: [%s]", video_call_state_string);
}

void sim_state_noti_cb(telephony_h handle, telephony_noti_e noti_id, void *data, void *user_data)
{
    telephony_sim_state_e sim_state = *(int *)data;
    char *sim_state_string = _telephony_sim_state_to_string(sim_state);
    PRINT_MSG("SIM state: [%s]", sim_state_string);
    dlog_print(DLOG_DEBUG, LOG_TAG, "SIM state: [%s]", sim_state_string);
}

void network_service_state_noti_cb(telephony_h handle, telephony_noti_e noti_id, void *data,
                                   void *user_data)
{
    telephony_network_service_state_e network_state = *(int *)data;
    char *network_state_string = _telephony_network_state_to_string(network_state);
    PRINT_MSG("SIM state: [%s]", network_state_string);
    dlog_print(DLOG_DEBUG, LOG_TAG, "SIM state: [%s]", network_state_string);
}

/* Application scenarios */

void _telephony_scenario_telephony_information(appdata_s *ad, Evas_Object *obj, void *event_info)
{
    telephony_handle_list_s handle_list;
    int i;
    char *state = NULL;
    telephony_error_e ret;

    /*  Create a telephony handle */
    ret = telephony_init(&handle_list); // In case of single SIM, we get only one handle
    if (ret != TELEPHONY_ERROR_NONE)
        PRINT_MSG("[telephony_init] failed");
    else {
        for (i = 0; i < handle_list.count; i++) {
            dlog_print(DLOG_DEBUG, LOG_TAG, "telephony handle[%p] for subscription[%d]",
                       handle_list.handle[i], i);
            PRINT_MSG("telephony handle[%p] for subscription[%d]", handle_list.handle[i], i);
        }

        /* Get the telephony handles for all subscriptions  */
        telephony_call_h *call_list;
        unsigned int count = 0;
        int ret_value = telephony_call_get_call_list(handle_list.handle[0], &count, &call_list);
        if (ret_value != TELEPHONY_ERROR_NONE) {
            dlog_print(DLOG_DEBUG, LOG_TAG, "telephony_call_get_call_list() failed [0x%x]",
                       ret_value);
        } else {
            int i;
            unsigned int handle_id = 0;
            char *number = NULL;
            telephony_call_type_e type = 0;
            telephony_call_status_e status = 0;
            telephony_call_direction_e direction = 0;
            bool conf_status = 0;

            for (i = 0; i < count; i++) {
                telephony_call_get_handle_id(call_list[i], &handle_id);
                telephony_call_get_number(call_list[i], &number);
                telephony_call_get_type(call_list[i], &type);
                telephony_call_get_status(call_list[i], &status);
                telephony_call_get_direction(call_list[i], &direction);
                telephony_call_get_conference_status(call_list[i], &conf_status);
                dlog_print(DLOG_DEBUG, LOG_TAG,
                           "id[%d] number[%s] type[%s] status[%s] direction[%s] conference_status[%s]",
                           handle_id, number,
                           type == TELEPHONY_CALL_TYPE_VOICE ? "VOICE" : type ==
                           TELEPHONY_CALL_TYPE_VIDEO ? "VIDEO" : "E911",
                           status == TELEPHONY_CALL_STATUS_IDLE ? "IDLE" : status ==
                           TELEPHONY_CALL_STATUS_ACTIVE ? "ACTIVE" : status ==
                           TELEPHONY_CALL_STATUS_HELD ? "HELD" : status ==
                           TELEPHONY_CALL_STATUS_DIALING ? "DIALING" : status ==
                           TELEPHONY_CALL_STATUS_ALERTING ? "ALERTING" : status ==
                           TELEPHONY_CALL_STATUS_INCOMING ? "INCOMING" : "UNKNOWN",
                           direction == TELEPHONY_CALL_DIRECTION_MO ? "MO" : "MT",
                           conf_status ? "TRUE" : "FALSE");

                free(number);
            }

            telephony_call_release_call_list(count, &call_list);
        }

        /*
         * Register the voice_call_status_noti_cb() callback function
         * to be notified if the voice call state changes
         */

        for (i = 0; i < (sizeof(voice_call_noti_tbl) / sizeof(int)); i++) {
            ret =
                telephony_set_noti_cb(handle_list.handle[0], voice_call_noti_tbl[i],
                                      voice_call_status_noti_cb, NULL);
            if (ret != TELEPHONY_ERROR_NONE)
                PRINT_MSG("[telephony_set_noti_cb] failed");
        }

        /*
         * Register the video_call_state_noti_cb() callback function
         * to be notified if the video call state changes
         */
        ret = telephony_set_noti_cb(handle_list.handle[0], TELEPHONY_NOTI_VIDEO_CALL_STATUS_ACTIVE,
                                    video_call_state_noti_cb, NULL);
        if (ret != TELEPHONY_ERROR_NONE)
            PRINT_MSG("[telephony_set_noti_cb] failed");

        /* Get the state of the SIM card */
        telephony_sim_state_e sim_state;
        ret = telephony_sim_get_state(handle_list.handle[0], &sim_state);
        state = _telephony_sim_state_to_string(sim_state);
        if (ret != TELEPHONY_ERROR_NONE)
            PRINT_MSG("[telephony_sim_get_state] failed");
        else {
            PRINT_MSG("SIM status: [%s]", state);
            dlog_print(DLOG_DEBUG, LOG_TAG, "SIM status: [%s]", state);
        }

        /*
         * Register the sim_state_noti_cb() callback function
         * to be notified if the state of the SIM card changes
         */
        ret = telephony_set_noti_cb(handle_list.handle[0], TELEPHONY_NOTI_SIM_STATUS,
                                    sim_state_noti_cb, NULL);
        if (ret != TELEPHONY_ERROR_NONE)
            PRINT_MSG("[telephony_set_noti_cb] failed");

        if (sim_state == TELEPHONY_SIM_STATE_AVAILABLE) {
            /* Get ICCID information from a SIM card */
            char *iccid = NULL;
            ret = telephony_sim_get_icc_id(handle_list.handle[0], &iccid);
            if (ret != TELEPHONY_ERROR_NONE)
                PRINT_MSG("[telephony_sim_get_icc_id] failed");
            else {
                PRINT_MSG("Integrated Circuit Card IDentification: %s", iccid);
                dlog_print(DLOG_DEBUG, LOG_TAG, "Integrated Circuit Card IDentification: %s", iccid);
                free(iccid);
            }
        }

        /* Get the Network Service state */
        telephony_network_service_state_e network_service_state;
        ret = telephony_network_get_service_state(handle_list.handle[0], &network_service_state);
        state = _telephony_network_state_to_string(network_service_state);
        if (ret != TELEPHONY_ERROR_NONE)
            PRINT_MSG("[telephony_network_get_service_state] failed");
        else {
            PRINT_MSG("Network Service State [%s]", state);
            dlog_print(DLOG_DEBUG, LOG_TAG, "Network Service State [%s]", state);
        }

        /*
         * Register the network_service_state_noti_cb() callback function
         * to be notified if the Network Service state changes
         */
        ret = telephony_set_noti_cb(handle_list.handle[0], TELEPHONY_NOTI_NETWORK_SERVICE_STATE,
                                    network_service_state_noti_cb, NULL);
        if (ret != TELEPHONY_ERROR_NONE)
            PRINT_MSG("[telephony_set_noti_cb] failed");

        /* Get the Cell ID */
        int cell_id;
        ret = telephony_network_get_cell_id(handle_list.handle[0], &cell_id);
        if (ret != TELEPHONY_ERROR_NONE)
            PRINT_MSG("[telephony_network_get_cell_id] failed");
        else {
            PRINT_MSG("Cell Id: %d", cell_id);
            dlog_print(DLOG_DEBUG, LOG_TAG, "Cell Id: %d", cell_id);
        }

        /* Get the MNC */
        char *mnc = NULL;
        ret = telephony_network_get_mnc(handle_list.handle[0], &mnc);
        if (ret != TELEPHONY_ERROR_NONE)
            PRINT_MSG("[telephony_network_get_mnc] failed");
        else {
            PRINT_MSG("mnc: %s", mnc);
            dlog_print(DLOG_DEBUG, LOG_TAG, "mnc: %s", mnc);
            free(mnc);
        }

        /* Get the modem information */
        char *imei = NULL;
        ret = telephony_modem_get_imei(handle_list.handle[0], &imei);
        if (ret != TELEPHONY_ERROR_NONE)
            PRINT_MSG("[telephony_modem_get_imei] failed");
        else {
            PRINT_MSG("imei: %s", imei);
            dlog_print(DLOG_DEBUG, LOG_TAG, "imei: %s", imei);
            free(imei);
        }
    }
}

void create_buttons_in_main_window(appdata_s *ad)
{
    Evas_Object *display = _create_new_cd_display(ad, "Telephony Information", _pop_cb);

    bool telephony_supported;
    int ret = system_info_get_platform_bool("http://tizen.org/feature/network.telephony", &telephony_supported);
    if (ret != SYSTEM_INFO_ERROR_NONE) {
        PRINT_MSG("Couldn't obtain system information");
        return;
    }

    if (!telephony_supported) {
        PRINT_MSG("Telephony is not supported on this device");
        return;
    }

    _new_button(ad, display, "Start", _telephony_scenario_telephony_information);
}
