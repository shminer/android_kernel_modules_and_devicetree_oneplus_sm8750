// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2022-2023 Qualcomm Innovation Center, Inc. All rights reserved.
 * Copyright (c) 2017-2020, The Linux Foundation. All rights reserved.
 */

#define pr_fmt(fmt)	"[drm:%s:%d] " fmt, __func__, __LINE__
#include "sde_reg_dma.h"
#include "sde_hw_reg_dma_v1.h"
#include "sde_dbg.h"
#include "sde_hw_ctl.h"

#define REG_DMA_VER_1_0 0x00010000
#define REG_DMA_VER_1_1 0x00010001
#define REG_DMA_VER_1_2 0x00010002
#define REG_DMA_VER_2_0 0x00020000
#define REG_DMA_VER_3_0 0x00030000

static int default_check_support(enum sde_reg_dma_features feature,
		     enum sde_reg_dma_blk blk,
		     bool *is_supported)
{

	if (!is_supported)
		return -EINVAL;

	*is_supported = false;
	return 0;
}

static int default_setup_payload(struct sde_reg_dma_setup_ops_cfg *cfg)
{
	DRM_ERROR("not implemented\n");
	return -EINVAL;
}

static int default_kick_off(struct sde_reg_dma_kickoff_cfg *cfg, u32 dpu_idx)
{
	DRM_ERROR("not implemented\n");
	return -EINVAL;

}

static int default_reset(struct sde_hw_ctl *ctl)
{
	DRM_ERROR("not implemented\n");
	return -EINVAL;
}

struct sde_reg_dma_buffer *default_alloc_reg_dma_buf(u32 size, u32 dpu_idx)
{
	DRM_ERROR("not implemented\n");
	return ERR_PTR(-EINVAL);
}

int default_dealloc_reg_dma(struct sde_reg_dma_buffer *lut_buf, u32 dpu_idx)
{
	DRM_ERROR("not implemented\n");
	return -EINVAL;
}

static int default_buf_reset_reg_dma(struct sde_reg_dma_buffer *lut_buf)
{
	DRM_ERROR("not implemented\n");
	return -EINVAL;
}

static int default_last_command(struct sde_hw_ctl *ctl,
		enum sde_reg_dma_queue q, enum sde_reg_dma_last_cmd_mode mode)
{
	return 0;
}

static int default_last_command_sb(struct sde_hw_ctl *ctl,
		enum sde_reg_dma_queue q, enum sde_reg_dma_last_cmd_mode mode)
{
	return 0;
}

static void default_dump_reg(u32 dpu_idx)
{
}

static void set_default_dma_ops(struct sde_hw_reg_dma *reg_dma)
{
	const static struct sde_hw_reg_dma_ops ops = {
		default_check_support, default_setup_payload,
		default_kick_off, default_reset, default_alloc_reg_dma_buf,
		default_dealloc_reg_dma, default_buf_reset_reg_dma,
		default_last_command, default_last_command_sb,
		default_dump_reg};
	memcpy(&reg_dma->ops, &ops, sizeof(ops));
}

static struct sde_hw_reg_dma reg_dma[DPU_MAX];

static int sde_reg_dma_reset(void *ctl_data, void *priv_data)
{
	struct sde_hw_ctl *sde_hw_ctl = (struct sde_hw_ctl *)ctl_data;
	struct sde_hw_reg_dma_ops *ops = sde_reg_dma_get_ops(sde_hw_ctl->dpu_idx);

	if (ops && ops->reset) {
		SDE_EVT32(sde_hw_ctl ? sde_hw_ctl->idx : 0xff, SDE_EVTLOG_FUNC_ENTRY);
		return ops->reset(sde_hw_ctl);
	}

	return 0;
}

int sde_reg_dma_init(void __iomem *addr, struct sde_mdss_cfg *m,
		struct drm_device *dev)
{
	int rc = 0;
	void *client_entry_handle;
	u32 dpu_idx = 0;

	struct msm_fence_error_ops sde_reg_dma_event_ops = {
		.fence_error_handle_submodule = sde_reg_dma_reset,
	};

	client_entry_handle = msm_register_fence_error_event(dev, &sde_reg_dma_event_ops, NULL);
	if (IS_ERR_OR_NULL(client_entry_handle))
		DRM_INFO("register fence_error_event failed.\n");


	if (!addr || !m || !dev) {
		DRM_DEBUG("invalid addr %pK catalog %pK dev %pK\n", addr, m,
				dev);
		return 0;
	}

	/**
	 * Register dummy range to ensure register dump is only done on
	 * targeted LUTDMA regions. start = 1, end = 1 so full range isn't used
	 */
	sde_dbg_reg_register_dump_range(LUTDMA_DBG_NAME, "DUMMY_LUTDMA", 1, 1,
			m->dma_cfg.xin_id);

	if (!m->reg_dma_count)
		return 0;

	if (!dev->primary) {
		DRM_DEBUG("invalid primary dev %pK\n", dev->primary);
		return 0;
	}

	if (dev->primary->index >= DPU_MAX) {
		DRM_DEBUG("invalid dpu idx %u\n", dev->primary->index);
		return 0;
	}

	dpu_idx = dev->primary->index;
	set_default_dma_ops(&(reg_dma[dpu_idx]));

	reg_dma[dpu_idx].reg_dma_count = m->reg_dma_count;
	reg_dma[dpu_idx].drm_dev = dev;
	reg_dma[dpu_idx].addr = addr;
	reg_dma[dpu_idx].caps = &m->dma_cfg;

	switch (reg_dma[dpu_idx].caps->version) {
	case REG_DMA_VER_1_0:
		rc = init_v1(&(reg_dma[dpu_idx]), dpu_idx);
		if (rc)
			DRM_DEBUG("init v1 dma ops failed\n");
		break;
	case REG_DMA_VER_1_1:
		rc = init_v11(&(reg_dma[dpu_idx]), dpu_idx);
		if (rc)
			DRM_DEBUG("init v11 dma ops failed\n");
		break;
	case REG_DMA_VER_1_2:
		rc = init_v12(&(reg_dma[dpu_idx]), dpu_idx);
		if (rc)
			DRM_DEBUG("init v12 dma ops failed\n");
		break;
	case REG_DMA_VER_2_0:
		rc = init_v2(&(reg_dma[dpu_idx]), dpu_idx);
		if (rc)
			DRM_DEBUG("init v2 dma ops failed\n");
		break;
	case REG_DMA_VER_3_0:
		rc = init_v3(&(reg_dma[dpu_idx]), dpu_idx);
		if (rc)
			DRM_DEBUG("init v3 dma ops failed\n");
		break;
	default:
		break;
	}

	return rc;
}

struct sde_hw_reg_dma_ops *sde_reg_dma_get_ops(u32 dpu_idx)
{
	if (dpu_idx >= DPU_MAX) {
		DRM_ERROR("invalid dpu idx %d\n", dpu_idx);
		return NULL;
	}

	return (reg_dma[dpu_idx].ops.check_support ? &(reg_dma[dpu_idx].ops) : NULL);
}

void sde_reg_dma_deinit(u32 dpu_idx)
{
	if (dpu_idx >= DPU_MAX) {
		DRM_ERROR("invalid dpu idx %d\n", dpu_idx);
		return;
	}

	if (!reg_dma[dpu_idx].drm_dev || !reg_dma[dpu_idx].caps)
		return;

	switch (reg_dma[dpu_idx].caps->version) {
	case REG_DMA_VER_1_0:
	case REG_DMA_VER_1_1:
	case REG_DMA_VER_1_2:
	case REG_DMA_VER_2_0:
		deinit_v1(dpu_idx);
		break;
	default:
		break;
	}
	memset(&(reg_dma[dpu_idx]), 0, sizeof(struct sde_hw_reg_dma));
	set_default_dma_ops(&(reg_dma[dpu_idx]));
}
