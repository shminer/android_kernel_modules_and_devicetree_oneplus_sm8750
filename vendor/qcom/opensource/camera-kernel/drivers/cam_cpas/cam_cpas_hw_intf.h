/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2017-2021, The Linux Foundation. All rights reserved.
 * Copyright (c) 2022-2024 Qualcomm Innovation Center, Inc. All rights reserved.
 */

#ifndef _CAM_CPAS_HW_INTF_H_
#define _CAM_CPAS_HW_INTF_H_

#include <linux/platform_device.h>

#include "cam_cpas_api.h"
#include "cam_hw.h"
#include "cam_hw_intf.h"
#include "cam_debug_util.h"

/* Number of times to retry while polling */
#define CAM_CPAS_POLL_RETRY_CNT 5
/* Minimum usecs to sleep while polling */
#define CAM_CPAS_POLL_MIN_USECS 200
/* Maximum usecs to sleep while polling */
#define CAM_CPAS_POLL_MAX_USECS 250
/* Number of times to retry while polling */
#define CAM_CPAS_POLL_QH_RETRY_CNT 5

/* Number of CPAS hw caps registers */
#define CAM_CPAS_MAX_CAPS_REGS 2

/**
 * enum cam_cpas_hw_type - Enum for CPAS HW type
 */
enum cam_cpas_hw_type {
	CAM_HW_CPASTOP,
	CAM_HW_CAMSSTOP,
};

/**
 * enum cam_cpas_reg_base - Enum for register base identifier. These
 *                          are the identifiers used in generic register
 *                          write/read APIs provided by cpas driver.
 */
enum cam_cpas_reg_base {
	CAM_CPAS_REG_CPASTOP,
	CAM_CPAS_REG_CAMNOC,
	CAM_CPAS_REG_CAMSS,
	CAM_CPAS_REG_RPMH,
	CAM_CPAS_REG_CESTA,
	CAM_CPAS_REG_CAMNOC_RT,
	CAM_CPAS_REG_CAMNOC_NRT,
	CAM_CPAS_REG_SECURE,
	CAM_CPAS_REG_MAX
};

/**
 * enum cam_cpas_hw_cmd_process - Enum for CPAS HW process command type
 */
enum cam_cpas_hw_cmd_process {
	CAM_CPAS_HW_CMD_REGISTER_CLIENT,
	CAM_CPAS_HW_CMD_UNREGISTER_CLIENT,
	CAM_CPAS_HW_CMD_SET_ADDR_TRANS,
	CAM_CPAS_HW_CMD_REG_WRITE,
	CAM_CPAS_HW_CMD_REG_READ,
	CAM_CPAS_HW_CMD_AHB_VOTE,
	CAM_CPAS_HW_CMD_AXI_VOTE,
	CAM_CPAS_HW_AXI_FLOOR_LVL,
	CAM_CPAS_HW_CMD_LOG_VOTE,
	CAM_CPAS_HW_CMD_SELECT_QOS,
	CAM_CPAS_HW_CMD_LOG_EVENT,
	CAM_CPAS_HW_CMD_GET_SCID,
	CAM_CPAS_HW_CMD_ACTIVATE_LLC,
	CAM_CPAS_HW_CMD_DEACTIVATE_LLC,
	CAM_CPAS_HW_CMD_CONFIGURE_STALING_LLC,
	CAM_CPAS_HW_CMD_NOTIF_STALL_INC_LLC,
	CAM_CPAS_HW_CMD_DUMP_BUFF_FILL_INFO,
	CAM_CPAS_HW_CMD_CSID_INPUT_CORE_INFO_UPDATE,
	CAM_CPAS_HW_CMD_CSID_PROCESS_RESUME,
	CAM_CPAS_HW_CMD_ENABLE_DISABLE_DOMAIN_ID_CLK,
	CAM_CPAS_HW_CMD_TPG_MUX_SEL,
	CAM_CPAS_HW_CMD_DUMP_STATE_MONITOR_INFO,
	CAM_CPAS_HW_CMD_INVALID,
};

/**
 * struct cam_cpas_hw_cmd_csid_input_core_info_update : CPAS cmd struct for updating acquired
 *                                                      csid core info to cpas
 *
 * @csid_idx: CSID core index
 * @sfe_idx:  SFE core index corresponding to CSID core
 * @set_port: Indicates whether to set or reset port for given client
 *
 */
struct cam_cpas_hw_cmd_csid_input_core_info_update {
	int csid_idx;
	int sfe_idx;
	bool set_port;
};

/**
 * struct cam_cpas_hw_cmd_reg_read_write : CPAS cmd struct for reg read, write
 *
 * @client_handle: Client handle
 * @reg_base: Register base type
 * @offset: Register offset
 * @value: Register value
 * @mb: Whether to do operation with memory barrier
 *
 */
struct cam_cpas_hw_cmd_reg_read_write {
	uint32_t client_handle;
	enum cam_cpas_reg_base reg_base;
	uint32_t offset;
	uint32_t value;
	bool mb;
};

/**
 * struct cam_cpas_hw_cmd_ahb_vote : CPAS cmd struct for AHB vote
 *
 * @client_handle: Client handle
 * @ahb_vote: AHB voting info
 *
 */
struct cam_cpas_hw_cmd_ahb_vote {
	uint32_t client_handle;
	struct cam_ahb_vote *ahb_vote;
};

/**
 * struct cam_cpas_hw_cmd_axi_vote : CPAS cmd struct for AXI vote
 *
 * @client_handle: Client handle
 * @axi_vote: axi bandwidth vote
 *
 */
struct cam_cpas_hw_cmd_axi_vote {
	uint32_t client_handle;
	struct cam_axi_vote *axi_vote;
};

/**
 * struct cam_cpas_hw_axi_floor_lvl : CPAS struct for AXI floor level
 *
 * @client_handle: Client handle
 * @floor_lvl: axi floor level
 *
 */
struct cam_cpas_hw_axi_floor_lvl {
	uint32_t client_handle;
	enum cam_vote_level floor_lvl;
};

/**
 * struct cam_cpas_hw_cmd_start : CPAS cmd struct for start
 *
 * @client_handle: Client handle
 *
 */
struct cam_cpas_hw_cmd_start {
	uint32_t client_handle;
	struct cam_ahb_vote *ahb_vote;
	struct cam_axi_vote *axi_vote;
};

/**
 * struct cam_cpas_hw_cmd_stop : CPAS cmd struct for stop
 *
 * @client_handle: Client handle
 *
 */
struct cam_cpas_hw_cmd_stop {
	uint32_t client_handle;
};

/**
 * struct cam_cpas_hw_addr_trans_data : CPAS cmd struct for programming address translator
 *
 * @client_handle: Client handle
 * @addr_trans_data: Register values to be programmed for address translator
 *
 */
struct cam_cpas_hw_addr_trans_data {
	uint32_t client_handle;
	struct cam_cpas_addr_trans_data *addr_trans_data;
};

/**
 * struct cam_cpas_hw_cmd_notify_event : CPAS cmd struct for notify event
 *
 * @identifier_string: Identifier string passed by caller
 * @identifier_value: Identifier value passed by caller
 *
 */
struct cam_cpas_hw_cmd_notify_event {
	const char *identifier_string;
	int32_t identifier_value;
};

/**
 * struct cam_cpas_hw_caps : CPAS HW capabilities
 *
 * @camera_family: Camera family type
 * @camera_version: Camera version
 * @cpas_version: CPAS version
 * @camera_capability: array of camera hw capabilities
 * @num_capability_reg: Number of camera hw capabilities registers
 * @fuse_info: Fuse information
 *
 */
struct cam_cpas_hw_caps {
	uint32_t camera_family;
	struct cam_hw_version camera_version;
	struct cam_hw_version cpas_version;
	uint32_t camera_capability[CAM_CPAS_MAX_CAPS_REGS];
	uint32_t num_capability_reg;
	struct cam_cpas_fuse_info fuse_info;
};

int cam_cpas_hw_probe(struct platform_device *pdev,
	struct cam_hw_intf **hw_intf);
int cam_cpas_hw_remove(struct cam_hw_intf *cpas_hw_intf);

/**
 * @brief : API to register CPAS hw to platform framework.
 * @return struct platform_device pointer on on success, or ERR_PTR() on error.
 */
int cam_cpas_dev_init_module(void);

/**
 * @brief : API to remove CPAS interface from platform framework.
 */
void cam_cpas_dev_exit_module(void);

/**
 * @brief : API to select TPG mux select.
 */
int cam_cpas_enable_tpg_mux_sel(uint32_t tpg_mux_sel);

#endif /* _CAM_CPAS_HW_INTF_H_ */
