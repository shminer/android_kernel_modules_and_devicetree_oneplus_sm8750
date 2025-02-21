/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2017-2021, The Linux Foundation. All rights reserved.
 * Copyright (c) 2022-2024 Qualcomm Innovation Center, Inc. All rights reserved.
 */

#ifndef CAM_ICP_HW_INTF_H
#define CAM_ICP_HW_INTF_H

#include "cam_vmrm_interface.h"

#define CAM_ICP_CMD_BUF_MAX_SIZE     128
#define CAM_ICP_MSG_BUF_MAX_SIZE     CAM_ICP_CMD_BUF_MAX_SIZE

#define CAM_ICP_BW_CONFIG_UNKNOWN 0
#define CAM_ICP_BW_CONFIG_V1      1
#define CAM_ICP_BW_CONFIG_V2      2

#define CAM_ICP_UBWC_COMP_EN      BIT(1)

#define HFI_MAX_POLL_TRY 5
#define PC_POLL_DELAY_US 100
#define PC_POLL_TIMEOUT_US 10000

#define CAM_ICP_DUMP_STATUS_REGISTERS BIT(0)
#define CAM_ICP_DUMP_CSR_REGISTERS    BIT(1)

#define CAM_ICP_MAX_ICP_HW_TYPE 2
#define CAM_ICP_DEV_START_IDX CAM_ICP_MAX_ICP_HW_TYPE

#define ICP_CAPS_MASK_IDX 0
#define IPE_CAPS_MASK_IDX 0
#define BPS_CAPS_MASK_IDX 0
#define OFE_CAPS_MASK_IDX 1

/* max caps mask is max value of all device caps mask index added by 1 */
#define MAX_HW_CAPS_MASK 2

/*
 * icp inter vm commnication timeout must be higher,
 * as it has to wait for completion of icp power related operations
 */
#define CAM_ICP_INTER_VM_COMM_TIMEOUT_US 10000

#define CAM_BPS_HW_NUM_MAX 1
#define CAM_OFE_HW_NUM_MAX 1
#define CAM_IPE_HW_NUM_MAX 1
#define CAM_ICP_HW_NUM_MAX 2

#define CAM_ICP_PID_NUM_MAX 10

enum cam_icp_hw_type {
	CAM_ICP_HW_ICP_V1,
	CAM_ICP_HW_ICP_V2,
	CAM_ICP_DEV_IPE,
	CAM_ICP_DEV_BPS,
	CAM_ICP_DEV_OFE,
	CAM_ICP_HW_MAX,
};

#define CAM_ICP_DEV_NUM          (CAM_ICP_HW_MAX - CAM_ICP_DEV_START_IDX)

enum cam_icp_dev_cmd_type {
	CAM_ICP_DEV_CMD_POWER_COLLAPSE,
	CAM_ICP_DEV_CMD_POWER_RESUME,
	CAM_ICP_DEV_CMD_SET_FW_BUF,
	CAM_ICP_DEV_CMD_VOTE_CPAS,
	CAM_ICP_DEV_CMD_CPAS_START,
	CAM_ICP_DEV_CMD_CPAS_STOP,
	CAM_ICP_DEV_CMD_UPDATE_CLK,
	CAM_ICP_DEV_CMD_DISABLE_CLK,
	CAM_ICP_DEV_CMD_RESET,
	CAM_ICP_DEV_CMD_MAX
};

enum cam_icp_cmd_type {
	CAM_ICP_CMD_PROC_SHUTDOWN,
	CAM_ICP_CMD_PROC_BOOT,
	CAM_ICP_CMD_POWER_COLLAPSE,
	CAM_ICP_CMD_POWER_RESUME,
	CAM_ICP_CMD_ACQUIRE,
	CAM_ICP_TEST_IRQ,
	CAM_ICP_SEND_INIT,
	CAM_ICP_CMD_VOTE_CPAS,
	CAM_ICP_CMD_CPAS_START,
	CAM_ICP_CMD_CPAS_STOP,
	CAM_ICP_CMD_UBWC_CFG,
	CAM_ICP_CMD_PC_PREP,
	CAM_ICP_CMD_CLK_UPDATE,
	CAM_ICP_CMD_HW_DUMP,
	CAM_ICP_CMD_HW_MINI_DUMP,
	CAM_ICP_CMD_HW_REG_DUMP,
	CAM_ICP_CMD_SET_HFI_HANDLE,
	CAM_ICP_CMD_PREP_BOOT,
	CAM_ICP_CMD_PREP_SHUTDOWN,
	CAM_ICP_CMD_MAX,
};

struct cam_icp_irq_cb {
	int32_t (*cb)(void *data, bool recover);
	void *data;
};

/**
 * struct cam_icp_boot_args - Boot arguments for ICP processor
 *
 * @firmware.iova: device vaddr to firmware region
 * @firmware.kva: kernel vaddr to firmware region
 * @firmware.len: length of firmware region
 * @irq_cb: irq callback
 * @debug_enabled: processor will be booted with debug enabled
 * @use_sec_pil: If set to true, use secure PIL for load
 */
struct cam_icp_boot_args {
	struct {
		uint32_t iova;
		uint64_t kva;
		uint64_t len;
	} firmware;

	struct cam_icp_irq_cb irq_cb;
	bool debug_enabled;
	bool use_sec_pil;
};

/**
 * struct cam_icp_dev_clk_update_cmd - Payload for hw manager command
 *
 * @curr_clk_rate:        clk rate to HW
 * @clk_level:            clk level corresponding to the clk rate
 *                        populated as output while the clk is being
 *                        updated to the given rate
 * @dev_pc_enable:        power collpase enable flag
 */
struct cam_icp_dev_clk_update_cmd {
	uint32_t  curr_clk_rate;
	int32_t clk_level;
	bool  dev_pc_enable;
};

/**
 * struct cam_icp_ubwc_cfg_cmd - ubwc cmd to send
 *
 * @ubwc_cfg_dev_mask: mask to indicate which device ubwc cfg to send to fw
 * @disable_ubwc_comp: flag to force disable ubwc
 */
struct cam_icp_ubwc_cfg_cmd {
	uint32_t ubwc_cfg_dev_mask;
	bool disable_ubwc_comp;
};

/**
 * struct cam_icp_hw_intf_data - ICP hw intf data
 *
 * @Brief:       ICP hw intf pointer and pid list data
 *
 * @icp_hw_intf: ICP hw intf pointer
 * @num_pid:     Number of pids for given hw
 * @pid:         ICP hw pid values
 */
struct cam_icp_hw_intf_data {
	struct cam_hw_intf *hw_intf;
	uint32_t            num_pid;
	uint32_t           *pid;
};

#endif
