/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2017, 2019 The Linux Foundation. All rights reserved.
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 */

#ifndef _CNSS_UTILS_H_
#define _CNSS_UTILS_H_

#include <linux/types.h>

struct device;

enum cnss_utils_cc_src {
	CNSS_UTILS_SOURCE_CORE,
	CNSS_UTILS_SOURCE_11D,
	CNSS_UTILS_SOURCE_USER
};

enum cnss_utils_device_type {
	CNSS_UNSUPPORETD_DEVICE_TYPE = -1,
	CNSS_HMT_DEVICE_TYPE,
	CNSS_HSP_DEVICE_TYPE
};

enum cnss_status_type {
	CNSS_UTILS_FMD_STATUS,
	CNSS_UTILS_MAX_STATUS_TYPE
};

typedef void (*cnss_utils_status_update)(void *cb_ctx, bool status);

extern int cnss_utils_set_wlan_unsafe_channel(struct device *dev,
					      u16 *unsafe_ch_list,
					      u16 ch_count);
extern int cnss_utils_get_wlan_unsafe_channel(struct device *dev,
					      u16 *unsafe_ch_list,
					      u16 *ch_count, u16 buf_len);
extern enum cnss_utils_device_type cnss_utils_update_device_type(
				enum cnss_utils_device_type device_type);
extern int cnss_utils_fmd_status(int is_enabled);
extern int
cnss_utils_register_status_notifier(enum cnss_status_type status_type,
				    cnss_utils_status_update status_update_cb,
				    void *cb_ctx);
extern int cnss_utils_wlan_set_dfs_nol(struct device *dev,
				       const void *info, u16 info_len);
extern int cnss_utils_wlan_get_dfs_nol(struct device *dev,
				       void *info, u16 info_len);
extern int cnss_utils_get_driver_load_cnt(struct device *dev);
extern void cnss_utils_increment_driver_load_cnt(struct device *dev);
extern int cnss_utils_set_wlan_mac_address(const u8 *in, uint32_t len);
extern u8 *cnss_utils_get_wlan_mac_address(struct device *dev, uint32_t *num);
extern int cnss_utils_set_wlan_derived_mac_address(const u8 *in, uint32_t len);
extern u8 *cnss_utils_get_wlan_derived_mac_address(struct device *dev,
							uint32_t *num);
extern void cnss_utils_set_cc_source(struct device *dev,
				     enum cnss_utils_cc_src cc_source);
extern enum cnss_utils_cc_src cnss_utils_get_cc_source(struct device *dev);

#ifdef CONFIG_FEATURE_SMEM_MAILBOX
extern int cnss_utils_smem_mailbox_write(struct device *dev, int flags,
					 const __u8 *data, uint32_t len);
#endif

#endif
