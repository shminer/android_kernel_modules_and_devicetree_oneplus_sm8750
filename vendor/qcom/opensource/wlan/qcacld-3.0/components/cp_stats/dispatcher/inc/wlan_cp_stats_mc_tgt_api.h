/*
 * Copyright (c) 2018, 2021 The Linux Foundation. All rights reserved.
 * Copyright (c) 2022-2024 Qualcomm Innovation Center, Inc. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all
 * copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

/**
 * DOC: wlan_cp_stats_mc_tgt_api.h
 *
 * This header file provide with API declarations to interface with Southbound
 */
#ifndef __WLAN_CP_STATS_MC_TGT_API_H__
#define __WLAN_CP_STATS_MC_TGT_API_H__

#ifdef QCA_SUPPORT_CP_STATS
#include "wlan_cp_stats_mc_defs.h"

//TODO -  Check if this is true for hamilton
#ifdef QCA_WIFI_QCA6490
#define TGT_MAC_ID_24G 2
#define TGT_MAC_ID_5G 1
#else
#define TGT_MAC_ID_24G 0
#define TGT_MAC_ID_5G 0
#endif

/* FW update to host tx power 63 is FW init value.
 * Host should convert it to zero which will reply to
 * user space when use iw dev get tx power.
 */
#define TARGET_MAX_TX_POWER    63

/**
 * target_if_mc_cp_get_mac_id(): API to get mac id
 * @vdev_mlme: vdev mlme pointer
 *
 * Return: mac id
 */
uint8_t target_if_mc_cp_get_mac_id(struct vdev_mlme_obj *vdev_mlme);

/**
 * tgt_mc_cp_stats_process_stats_event(): API to process stats event
 * @psoc: pointer to psoc object
 * @ev: event parameters
 *
 * Return: QDF_STATUS_SUCCESS on Success, other QDF_STATUS error codes on
 * failure
 */
QDF_STATUS
tgt_mc_cp_stats_process_stats_event(struct wlan_objmgr_psoc *psoc,
				    struct stats_event *ev);

#ifdef WLAN_SUPPORT_INFRA_CTRL_PATH_STATS
/**
 * tgt_mc_cp_stats_process_infra_stats_event(): API to process event from
 * cp stats infrastructure
 * @psoc: pointer to psoc object
 * @infra_event: infra cp stats event parameters
 *
 * Return: status of operation
 */
QDF_STATUS tgt_mc_cp_stats_process_infra_stats_event(
				struct wlan_objmgr_psoc *psoc,
				struct infra_cp_stats_event *infra_event);

#endif

#ifdef WLAN_FEATURE_BIG_DATA_STATS
/**
 * tgt_mc_cp_stats_process_big_data_stats_event(): API to process big data
 * stats event
 * @psoc: pointer to psoc object
 * @event: big data stats event parameters
 *
 * Return: status of operation
 */
QDF_STATUS
tgt_mc_cp_stats_process_big_data_stats_event(
				struct wlan_objmgr_psoc *psoc,
				struct big_data_stats_event *event);
#endif

/**
 * tgt_send_mc_cp_stats_req(): API to send stats request to lmac
 * @psoc: pointer to psoc object
 * @type: specific type of stats requested
 * @req: pointer to stats request
 *
 * Return: status of operation
 */
QDF_STATUS tgt_send_mc_cp_stats_req(struct wlan_objmgr_psoc *psoc,
				    enum stats_req_type type,
				    struct request_info *req);

/**
 * tgt_set_pdev_stats_update_period(): API to set pdev stats update
 * period to FW
 * @psoc: pointer to psoc object
 * @pdev_id: pdev id
 * @val: pdev stats update period, 0: disabled periodical stats report.
 *
 * Return: status of operation
 */
QDF_STATUS tgt_set_pdev_stats_update_period(struct wlan_objmgr_psoc *psoc,
					    uint8_t pdev_id, uint32_t val);

/**
 * tgt_mc_cp_stats_inc_wake_lock_stats() : API to increment wake lock stats
 * given the wake reason code
 * @psoc: pointer to psoc object
 * @reason: wake reason
 * @stats: vdev wow stats to update
 * @unspecified_wake_count: unspecified wake count to update
 *
 * Return : status of operation
 */
QDF_STATUS tgt_mc_cp_stats_inc_wake_lock_stats(struct wlan_objmgr_psoc *psoc,
				uint32_t reason, struct wake_lock_stats *stats,
				uint32_t *unspecified_wake_count);

#endif /* QCA_SUPPORT_CP_STATS */
#endif /* __WLAN_CP_STATS_MC_TGT_API_H__ */
