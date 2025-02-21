/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2021-2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * Copyright (c) 2015-2021, The Linux Foundation. All rights reserved.
 */

#ifndef _SDE_HW_SSPP_H
#define _SDE_HW_SSPP_H

#include "sde_hw_catalog.h"
#include "sde_hw_mdss.h"
#include "sde_hw_util.h"
#include "sde_reg_dma.h"
#include "sde_formats.h"
#include "sde_color_processing.h"
#include "sde_hw_vbif.h"

struct sde_hw_pipe;

/**
 * Flags
 */
#define SDE_SSPP_SECURE_OVERLAY_SESSION 0x1
#define SDE_SSPP_FLIP_LR	 0x2
#define SDE_SSPP_FLIP_UD	 0x4
#define SDE_SSPP_SOURCE_ROTATED_90 0x8
#define SDE_SSPP_ROT_90  0x10
#define SDE_SSPP_SOLID_FILL 0x20
#define SDE_SSPP_RIGHT	 0x40

/**
 * Define all scaler feature bits in catalog
 */
#define SDE_SSPP_SCALER ((1UL << SDE_SSPP_SCALER_QSEED2) | \
	(1UL << SDE_SSPP_SCALER_QSEED3) | \
	(1UL << SDE_SSPP_SCALER_QSEED3LITE))

/**
 * Component indices
 */
enum {
	SDE_SSPP_COMP_0,
	SDE_SSPP_COMP_1_2,
	SDE_SSPP_COMP_2,
	SDE_SSPP_COMP_3,

	SDE_SSPP_COMP_MAX
};

enum sde_sspp_multirect_mode {
	SDE_SSPP_MULTIRECT_NONE = 0,
	SDE_SSPP_MULTIRECT_PARALLEL,
	SDE_SSPP_MULTIRECT_TIME_MX,
};

enum {
	SDE_FRAME_LINEAR,
	SDE_FRAME_TILE_A4X,
	SDE_FRAME_TILE_A5X,
};

enum sde_hw_filter {
	SDE_SCALE_FILTER_NEAREST = 0,
	SDE_SCALE_FILTER_BIL,
	SDE_SCALE_FILTER_PCMN,
	SDE_SCALE_FILTER_CA,
	SDE_SCALE_FILTER_MAX
};

enum sde_hw_filter_alpa {
	SDE_SCALE_ALPHA_PIXEL_REP,
	SDE_SCALE_ALPHA_BIL
};

enum sde_hw_filter_yuv {
	SDE_SCALE_2D_4X4,
	SDE_SCALE_2D_CIR,
	SDE_SCALE_1D_SEP,
	SDE_SCALE_BIL
};

enum sde_sspp_ucsc_gc {
	UCSC_GC_MODE_DISABLE = 0,
	UCSC_GC_MODE_SRGB,
	UCSC_GC_MODE_PQ,
	UCSC_GC_MODE_GAMMA2_2,
	UCSC_GC_MODE_HLG,
};

enum sde_sspp_ucsc_igc {
	UCSC_IGC_MODE_DISABLE = 0,
	UCSC_IGC_MODE_SRGB,
	UCSC_IGC_MODE_REC709,
	UCSC_IGC_MODE_GAMMA2_2,
	UCSC_IGC_MODE_HLG,
	UCSC_IGC_MODE_PQ,
};

enum {
	SDE_CAC_NONE = 0,
	SDE_CAC_UNPACK,
	SDE_CAC_FETCH
};

struct sde_hw_sharp_cfg {
	u32 strength;
	u32 edge_thr;
	u32 smooth_thr;
	u32 noise_thr;
};

struct sde_hw_pixel_ext {
	/* scaling factors are enabled for this input layer */
	uint8_t enable_pxl_ext;

	int init_phase_x[SDE_MAX_PLANES];
	int phase_step_x[SDE_MAX_PLANES];
	int init_phase_y[SDE_MAX_PLANES];
	int phase_step_y[SDE_MAX_PLANES];

	/*
	 * Number of pixels extension in left, right, top and bottom direction
	 * for all color components. This pixel value for each color component
	 * should be sum of fetch + repeat pixels.
	 */
	int num_ext_pxls_left[SDE_MAX_PLANES];
	int num_ext_pxls_right[SDE_MAX_PLANES];
	int num_ext_pxls_top[SDE_MAX_PLANES];
	int num_ext_pxls_btm[SDE_MAX_PLANES];

	/*
	 * Number of pixels needs to be overfetched in left, right, top and
	 * bottom directions from source image for scaling.
	 */
	int left_ftch[SDE_MAX_PLANES];
	int right_ftch[SDE_MAX_PLANES];
	int top_ftch[SDE_MAX_PLANES];
	int btm_ftch[SDE_MAX_PLANES];

	/*
	 * Number of pixels needs to be repeated in left, right, top and
	 * bottom directions for scaling.
	 */
	int left_rpt[SDE_MAX_PLANES];
	int right_rpt[SDE_MAX_PLANES];
	int top_rpt[SDE_MAX_PLANES];
	int btm_rpt[SDE_MAX_PLANES];

	uint32_t roi_w[SDE_MAX_PLANES];
	uint32_t roi_h[SDE_MAX_PLANES];

	/*
	 * Filter type to be used for scaling in horizontal and vertical
	 * directions
	 */
	enum sde_hw_filter horz_filter[SDE_MAX_PLANES];
	enum sde_hw_filter vert_filter[SDE_MAX_PLANES];

};

/**
 * struct sde_hw_pipe_cfg : Pipe description
 * @layout:    format layout information for programming buffer to hardware
 * @src_rect:  src ROI, caller takes into account the different operations
 *             such as decimation, flip etc to program this field
 * @dest_rect: destination ROI.
 * @src_rect_extn: extension source rect values
 * @dst_rect_extn: extension destination rect values
 * @ horz_decimation : horizontal decimation factor( 0, 2, 4, 8, 16)
 * @ vert_decimation : vertical decimation factor( 0, 2, 4, 8, 16)
 *              2: Read 1 line/pixel drop 1 line/pixel
 *              4: Read 1 line/pixel drop 3  lines/pixels
 *              8: Read 1 line/pixel drop 7 lines/pixels
 *              16: Read 1 line/pixel drop 15 line/pixels
 */
struct sde_hw_pipe_cfg {
	struct sde_hw_fmt_layout layout;
	struct sde_rect src_rect;
	struct sde_rect dst_rect;
	struct sde_rect src_rect_extn;
	struct sde_rect dst_rect_extn;
	u8 horz_decimation;
	u8 vert_decimation;
};

/**
 * struct sde_hw_pipe_qos_cfg : Source pipe QoS configuration
 * @danger_lut: LUT for generate danger level based on fill level
 * @safe_lut: LUT for generate safe level based on fill level
 * @creq_lut: LUT for generate creq level based on fill level
 * @creq_vblank: creq value generated to vbif during vertical blanking
 * @danger_vblank: danger value generated during vertical blanking
 * @vblank_en: enable creq_vblank and danger_vblank during vblank
 * @danger_safe_en: enable danger safe generation
 */
struct sde_hw_pipe_qos_cfg {
	u32 danger_lut;
	u32 safe_lut;
	u64 creq_lut;
	u32 creq_vblank;
	u32 danger_vblank;
	bool vblank_en;
	bool danger_safe_en;
};

/**
 * enum CDP preload ahead address size
 */
enum {
	SDE_SSPP_CDP_PRELOAD_AHEAD_32,
	SDE_SSPP_CDP_PRELOAD_AHEAD_64
};

/**
 * struct sde_hw_pipe_cdp_cfg : CDP configuration
 * @enable: true to enable CDP
 * @ubwc_meta_enable: true to enable ubwc metadata preload
 * @tile_amortize_enable: true to enable amortization control for tile format
 * @preload_ahead: number of request to preload ahead
 *	SDE_SSPP_CDP_PRELOAD_AHEAD_32,
 *	SDE_SSPP_CDP_PRELOAD_AHEAD_64
 */
struct sde_hw_pipe_cdp_cfg {
	bool enable;
	bool ubwc_meta_enable;
	bool tile_amortize_enable;
	u32 preload_ahead;
};

/**
 * enum system cache rotation operation mode
 */
enum {
	SDE_PIPE_SC_OP_MODE_OFFLINE,
	SDE_PIPE_SC_OP_MODE_INLINE_SINGLE,
	SDE_PIPE_SC_OP_MODE_INLINE_LEFT,
	SDE_PIPE_SC_OP_MODE_INLINE_RIGHT,
};

/**
 * enum system cache read operation type
 */
enum {
	SDE_PIPE_SC_RD_OP_TYPE_CACHEABLE,
	SDE_PIPE_SC_RD_OP_TYPE_RESERVED,
	SDE_PIPE_SC_RD_OP_TYPE_INVALIDATE,
	SDE_PIPE_SC_RD_OP_TYPE_EVICTION,
};

/**
 * struct sde_hw_pipe_sc_cfg - system cache configuration
 * @op_mode: rotation operating mode
 * @rd_en: system cache read enable
 * @rd_scid: system cache read block id
 * @rd_noallocate: system cache read no allocate attribute
 * @rd_op_type: system cache read operation type
 * @flags: dirty flags to change the configuration
 * @type: sys cache type
 */
struct sde_hw_pipe_sc_cfg {
	u32 op_mode;
	bool rd_en;
	u32 rd_scid;
	bool rd_noallocate;
	u32 rd_op_type;
	u32 flags;
	enum sde_sys_cache_type type;
};

/**
 * struct sde_hw_pipe_uidle_cfg - uidle configuration
 * @enable: disables uidle
 * @fal_allowed_threshold: minimum fl to allow uidle
 * @fal10_exit_threshold: number of lines to indicate fal_10_exit is okay
 * @fal10_threshold: number of lines where fal_10_is okay
 * @fal1_threshold: number of lines where fal_1 is okay
 * @fill_level_scale: scale factor on the fal10 threshold
 */
struct sde_hw_pipe_uidle_cfg {
	u32 enable;
	u32 fal_allowed_threshold;
	u32 fal10_exit_threshold;
	u32 fal10_threshold;
	u32 fal1_threshold;
	u32 fill_level_scale;
};

/**
 * struct sde_hw_pipe_ts_cfg - traffic shaper configuration
 * @size: size to prefill in bytes, or zero to disable
 * @time: time to prefill in usec, or zero to disable
 */
struct sde_hw_pipe_ts_cfg {
	u64 size;
	u64 time;
};

/**
 * Maximum number of stream buffer plane
 */
#define SDE_PIPE_SBUF_PLANE_NUM	2

/**
 * struct sde_hw_pipe_line_insertion_cfg - line insertion config
 * @enable: line insertion is enabled
 * @dummy_lines: dummy lines before active lines
 * @first_active_lines: number of active lines before first dummy lines
 * @active_lines: active lines
 * @dst_h: total active lines plus dummy lines
 */
struct sde_hw_pipe_line_insertion_cfg {
	bool enable;
	u32 dummy_lines;
	u32 first_active_lines;
	u32 active_lines;
	u32 dst_h;
};

/**
 * struct sde_hw_sspp_ops - interface to the SSPP Hw driver functions
 * Caller must call the init function to get the pipe context for each pipe
 * Assumption is these functions will be called after clocks are enabled
 */
struct sde_hw_sspp_ops {
	/**
	 * setup_format - setup pixel format cropping rectangle, flip
	 * @ctx: Pointer to pipe context
	 * @fmt: Pointer to sde_format structure
	 * @blend_enabled: flag indicating blend enabled or disabled on plane
	 * @flags: Extra flags for format config
	 * @index: rectangle index in multirect
	 */
	void (*setup_format)(struct sde_hw_pipe *ctx,
			const struct sde_format *fmt,
			bool blend_enabled, u32 flags,
			enum sde_sspp_multirect_index index);

	/**
	 * setup_rects - setup pipe ROI rectangles
	 * @ctx: Pointer to pipe context
	 * @cfg: Pointer to pipe config structure
	 * @index: rectangle index in multirect
	 */
	void (*setup_rects)(struct sde_hw_pipe *ctx,
			struct sde_hw_pipe_cfg *cfg,
			enum sde_sspp_multirect_index index);

	/**
	 * setup_pe - setup pipe pixel extension
	 * @ctx: Pointer to pipe context
	 * @pe_ext: Pointer to pixel ext settings
	 * @cac_en : Boolean to indicate cac is enabled or disabled
	 */
	void (*setup_pe)(struct sde_hw_pipe *ctx,
			struct sde_hw_pixel_ext *pe_ext, bool cac_en);

	/**
	 * setup_excl_rect - setup pipe exclusion rectangle
	 * @ctx: Pointer to pipe context
	 * @excl_rect: Pointer to exclclusion rect structure
	 * @index: rectangle index in multirect
	 */
	void (*setup_excl_rect)(struct sde_hw_pipe *ctx,
			struct sde_rect *excl_rect,
			enum sde_sspp_multirect_index index);

	/**
	 * setup_sourceaddress - setup pipe source addresses
	 * @ctx: Pointer to pipe context
	 * @cfg: Pointer to pipe config structure
	 * @index: rectangle index in multirect
	 */
	void (*setup_sourceaddress)(struct sde_hw_pipe *ctx,
			struct sde_hw_pipe_cfg *cfg,
			enum sde_sspp_multirect_index index);

	/* get_sourceaddress - get pipe current source addresses of a plane
	 * @ctx: Pointer to pipe context
	 * @is_virtual: If true get address programmed for R1 in multirect
	 */
	u32 (*get_sourceaddress)(struct sde_hw_pipe *ctx, bool is_virtual);

	/**
	 * setup_csc - setup color space conversion
	 * @ctx: Pointer to pipe context
	 * @data: Pointer to config structure
	 */
	void (*setup_csc)(struct sde_hw_pipe *ctx, struct sde_csc_cfg *data);

	/**
	 * setup_solidfill - enable/disable colorfill
	 * @ctx: Pointer to pipe context
	 * @const_color: Fill color value
	 * @flags: Pipe flags
	 * @index: rectangle index in multirect
	 */
	void (*setup_solidfill)(struct sde_hw_pipe *ctx, u32 color,
			enum sde_sspp_multirect_index index);

	/**
	 * update_multirect - update multirect configuration
	 * @ctx: Pointer to pipe context
	 * @enable: Boolean to indicate enable or disable of given config
	 * @index: rectangle index in multirect
	 * @mode: parallel fetch / time multiplex multirect mode
	 */

	void (*update_multirect)(struct sde_hw_pipe *ctx,
			bool enable,
			enum sde_sspp_multirect_index index,
			enum sde_sspp_multirect_mode mode);

	/**
	 * setup_sharpening - setup sharpening
	 * @ctx: Pointer to pipe context
	 * @cfg: Pointer to config structure
	 */
	void (*setup_sharpening)(struct sde_hw_pipe *ctx,
			struct sde_hw_sharp_cfg *cfg);


	/**
	 * setup_pa_hue(): Setup source hue adjustment
	 * @ctx: Pointer to pipe context
	 * @cfg: Pointer to hue data
	 */
	void (*setup_pa_hue)(struct sde_hw_pipe *ctx, void *cfg);

	/**
	 * setup_pa_sat(): Setup source saturation adjustment
	 * @ctx: Pointer to pipe context
	 * @cfg: Pointer to saturation data
	 */
	void (*setup_pa_sat)(struct sde_hw_pipe *ctx, void *cfg);

	/**
	 * setup_pa_val(): Setup source value adjustment
	 * @ctx: Pointer to pipe context
	 * @cfg: Pointer to value data
	 */
	void (*setup_pa_val)(struct sde_hw_pipe *ctx, void *cfg);

	/**
	 * setup_pa_cont(): Setup source contrast adjustment
	 * @ctx: Pointer to pipe context
	 * @cfg: Pointer contrast data
	 */
	void (*setup_pa_cont)(struct sde_hw_pipe *ctx, void *cfg);

	/**
	 * setup_pa_memcolor - setup source color processing
	 * @ctx: Pointer to pipe context
	 * @type: Memcolor type (Skin, sky or foliage)
	 * @cfg: Pointer to memory color config data
	 */
	void (*setup_pa_memcolor)(struct sde_hw_pipe *ctx,
			enum sde_memcolor_type type, void *cfg);

	/**
	 * setup_vig_gamut - setup 3D LUT Gamut in VIG pipes
	 * @ctx: Pointer to pipe context
	 * @cfg: Pointer to vig gamut data
	 */
	void (*setup_vig_gamut)(struct sde_hw_pipe *ctx, void *cfg);

	/**
	 * setup_vig_igc - setup 1D LUT IGC in VIG pipes
	 * @ctx: Pointer to pipe context
	 * @cfg: Pointer to vig igc data
	 */
	void (*setup_vig_igc)(struct sde_hw_pipe *ctx, void *cfg);

	/**
	 * setup_dma_igc - setup 1D LUT IGC in DMA pipes
	 * @ctx: Pointer to pipe context
	 * @cfg: Pointer to dma igc data
	 * @idx: multirect index
	 */
	void (*setup_dma_igc)(struct sde_hw_pipe *ctx, void *cfg,
				enum sde_sspp_multirect_index idx);

	/**
	 * setup_dma_gc - setup 1D LUT GC in DMA pipes
	 * @ctx: Pointer to pipe context
	 * @cfg: Pointer to dma gc data
	 * @idx: multirect index
	 */
	void (*setup_dma_gc)(struct sde_hw_pipe *ctx, void *cfg,
				enum sde_sspp_multirect_index idx);

	/**
	 * setup_qos_lut - setup danger, safe, creq LUTs
	 * @ctx: Pointer to pipe context
	 * @cfg: Pointer to pipe QoS configuration
	 *
	 */
	void (*setup_qos_lut)(struct sde_hw_pipe *ctx,
			struct sde_hw_pipe_qos_cfg *cfg);

	/**
	 * setup_qos_ctrl - setup QoS control
	 * @ctx: Pointer to pipe context
	 * @cfg: Pointer to pipe QoS configuration
	 *
	 */
	void (*setup_qos_ctrl)(struct sde_hw_pipe *ctx,
			struct sde_hw_pipe_qos_cfg *cfg);

	/**
	 * setup_histogram - setup histograms
	 * @ctx: Pointer to pipe context
	 * @cfg: Pointer to histogram configuration
	 */
	void (*setup_histogram)(struct sde_hw_pipe *ctx,
			void *cfg);

	/**
	 * setup_scaler - setup scaler
	 * @ctx: Pointer to pipe context
	 * @pipe_cfg: Pointer to pipe configuration
	 * @pe_cfg: Pointer to pixel extension configuration
	 * @scaler_cfg: Pointer to scaler configuration
	 */
	void (*setup_scaler)(struct sde_hw_pipe *ctx,
		struct sde_hw_pipe_cfg *pipe_cfg,
		struct sde_hw_pixel_ext *pe_cfg,
		void *scaler_cfg);

	/**
	 * setup_scaler_lut - setup scaler lut
	 * @buf: Defines structure for reg dma ops on the reg dma buffer.
	 * @scaler3_cfg: QSEEDv3 configuration
	 * @offset: Scaler Offset
	 * @dpu_idx: dpu index
	 */
	void (*setup_scaler_lut)(struct sde_reg_dma_setup_ops_cfg *buf,
			struct sde_hw_scaler3_cfg *scaler3_cfg,
			u32 offset, u32 dpu_idx);

	/**
	 * setup_pre_downscale - setup pre-downscaler for inline rotation
	 * @ctx: Pointer to pipe context
	 * @pre_down: Pointer to pre-downscaler configuration
	 */
	void (*setup_pre_downscale)(struct sde_hw_pipe *ctx,
		struct sde_hw_inline_pre_downscale_cfg *pre_down);

	/**
	 * setup_sys_cache - setup system cache configuration
	 * @ctx: Pointer to pipe context
	 * @cfg: Pointer to system cache configuration
	 */
	void (*setup_sys_cache)(struct sde_hw_pipe *ctx,
			struct sde_hw_pipe_sc_cfg *cfg);

	 /**
	  * setup_uidle - set uidle configuration
	  * @ctx: Pointer to pipe context
	  * @cfg: Pointer to uidle configuration
	  * @index: rectangle index in multirect
	  */
	 void (*setup_uidle)(struct sde_hw_pipe *ctx,
			 struct sde_hw_pipe_uidle_cfg *cfg,
			 enum sde_sspp_multirect_index index);

	/**
	 * setup_uidle_fill_scale - set uidle fill scale factor
	 * @ctx: Pointer to pipe context
	 * @cfg: Pointer to uidle configuration
	 */
	void (*setup_uidle_fill_scale)(struct sde_hw_pipe *ctx,
			 struct sde_hw_pipe_uidle_cfg *cfg);

	/**
	 * setup_ts_prefill - setup prefill traffic shaper
	 * @ctx: Pointer to pipe context
	 * @cfg: Pointer to traffic shaper configuration
	 * @index: rectangle index in multirect
	 */
	void (*setup_ts_prefill)(struct sde_hw_pipe *ctx,
			struct sde_hw_pipe_ts_cfg *cfg,
			enum sde_sspp_multirect_index index);

	/**
	 * setup_cdp - setup client driven prefetch
	 * @ctx: Pointer to pipe context
	 * @cfg: Pointer to cdp configuration
	 * @index: rectangle index in multirect
	 */
	void (*setup_cdp)(struct sde_hw_pipe *ctx,
			struct sde_hw_pipe_cdp_cfg *cfg,
			enum sde_sspp_multirect_index index);

	/**
	 * setup_secure_address - setup secureity status of the source address
	 * @ctx: Pointer to pipe context
	 * @index: rectangle index in multirect
	 * @enable: enable content protected buffer state
	 */
	void (*setup_secure_address)(struct sde_hw_pipe *ctx,
			enum sde_sspp_multirect_index index,
		bool enable);

	/**
	 * set_src_split_order - setup source split order priority
	 * @ctx: Pointer to pipe context
	 * @index: rectangle index in multirect
	 * @enable: enable src split order
	 */
	void (*set_src_split_order)(struct sde_hw_pipe *ctx,
			enum sde_sspp_multirect_index index, bool enable);

	/**
	 * setup_inverse_pma - enable/disable alpha unmultiply unit (PMA)
	 * @ctx: Pointer to pipe context
	 * @index: Rectangle index in multirect
	 * @enable: PMA enable/disable settings
	 */
	void (*setup_inverse_pma)(struct sde_hw_pipe *ctx,
			enum sde_sspp_multirect_index index, u32 enable);

	/**
	 * setup_dgm_csc - setup DGM color space conversion block and update lut
	 * @ctx: Pointer to pipe context
	 * @index: Rectangle index in multirect
	 * @data: Pointer to config structure
	 */
	void (*setup_dgm_csc)(struct sde_hw_pipe *ctx,
		enum sde_sspp_multirect_index index, struct sde_csc_cfg *data);

	/**
	 * clear_meta_error - clear the meta error-code registers
	 * @ctx: Pointer to pipe context
	 * @multirect_index: rec in use
	 */
	void (*clear_meta_error)(struct sde_hw_pipe *ctx,
			enum sde_sspp_multirect_index multirect_index);

	/**
	 * get_meta_error - get the meta error-code
	 * @ctx: Pointer to pipe context
	 * @multirect_index: rec in use
	 */
	u32 (*get_meta_error)(struct sde_hw_pipe *ctx,
			enum sde_sspp_multirect_index multirect_index);

	/**
	 * clear_ubwc_error - clear the ubwc error-code registers
	 * @ctx: Pointer to pipe context
	 * @multirect_index: rec in use
	 */
	void (*clear_ubwc_error)(struct sde_hw_pipe *ctx,
			enum sde_sspp_multirect_index multirect_index);

	/**
	 * get_ubwc_error - get the ubwc error-code
	 * @ctx: Pointer to pipe context
	 * @multirect_index: rec in use
	 */
	u32 (*get_ubwc_error)(struct sde_hw_pipe *ctx,
			enum sde_sspp_multirect_index multirect_index);

	/**
	 * get_ubwc_stats_data - get ubwc stats data
	 * @ctx: Pointer to pipe context
	 * @multirect_index: rec in use
	 * @data: Pointer to ubwc data to populate
	 */
	void (*get_ubwc_stats_data)(struct sde_hw_pipe *ctx,
			enum sde_sspp_multirect_index multirect_index,
			struct sde_drm_ubwc_stats_data *data);

	/**
	 * set_ubwc_stats_roi - set ubwc stats roi
	 * @ctx: Pointer to pipe context
	 * @multirect_index: rec in use
	 * @roi: roi to be programmed
	 */
	void (*set_ubwc_stats_roi)(struct sde_hw_pipe *ctx,
			enum sde_sspp_multirect_index multirect_index,
			struct sde_drm_ubwc_stats_roi *roi);

	/**
	 * setup_fp16_csc - set FP16 CSC cp block
	 * @ctx: Pointer to pipe object
	 * @index: Pipe rectangle to operate on
	 * @data: Pointer to sde_hw_cp_cfg object holding drm_msm_fp16_csc data
	 */
	void (*setup_fp16_csc)(struct sde_hw_pipe *ctx,
		enum sde_sspp_multirect_index index, void *data);

	/**
	 * sde_setup_fp16_gcv1 - set FP16 GC cp block
	 * @ctx: Pointer to pipe object
	 * @index: Pipe rectangle to operate on
	 * @data: Pointer to sde_hw_cp_cfg object holding drm_msm_fp16_gc data
	 */
	void (*setup_fp16_gc)(struct sde_hw_pipe *ctx,
		enum sde_sspp_multirect_index index, void *data);

	/**
	 * sde_setup_fp16_igcv1 - set FP16 IGC cp block
	 * @ctx: Pointer to pipe object
	 * @index: Pipe rectangle to operate on
	 * @data: Pointer to sde_hw_cp_cfg object containing bool data
	 */
	void (*setup_fp16_igc)(struct sde_hw_pipe *ctx,
		enum sde_sspp_multirect_index index, void *data);

	/**
	 * sde_setup_fp16_unmultv1 - set FP16 UNMULT cp block
	 * @ctx: Pointer to pipe object
	 * @index: Pipe rectangle to operate on
	 * @data: Pointer to sde_hw_cp_cfg object containing bool data
	 */
	void (*setup_fp16_unmult)(struct sde_hw_pipe *ctx,
		enum sde_sspp_multirect_index index, void *data);

	/**
	 * setup_line_insertion - setup line insertion
	 * @ctx: Pointer to pipe context
	 * @cfg: Pointer to line insertion configuration
	 */
	void (*setup_line_insertion)(struct sde_hw_pipe *ctx,
				     enum sde_sspp_multirect_index index,
				     struct sde_hw_pipe_line_insertion_cfg *cfg);

	/**
	 * setup_ucsc_csc - set UCSC CSC cp block
	 * @ctx: Pointer to pipe object
	 * @index: Pipe rectangle to operate on
	 * @data: Pointer to sde_hw_cp_cfg object holding drm_msm_ucsc_csc data
	 */
	void (*setup_ucsc_csc)(struct sde_hw_pipe *ctx,
		enum sde_sspp_multirect_index index, void *data);

	/**
	 * sde_setup_ucsc_gcv1 - set UCSC GC cp block
	 * @ctx: Pointer to pipe object
	 * @index: Pipe rectangle to operate on
	 * @mode: Pointer to sde_hw_cp_cfg object holding GC mode data
	 */
	void (*setup_ucsc_gc)(struct sde_hw_pipe *ctx,
		enum sde_sspp_multirect_index index, void *data);

	/**
	 * sde_setup_ucsc_igcv1 - set UCSC IGC cp block
	 * @ctx: Pointer to pipe object
	 * @index: Pipe rectangle to operate on
	 * @mode: Pointer to sde_hw_cp_cfg object containing IGC mode data
	 */
	void (*setup_ucsc_igc)(struct sde_hw_pipe *ctx,
		enum sde_sspp_multirect_index index, void *data);

	/**
	 * sde_setup_ucsc_unmultv1 - set UCSC UNMULT cp block
	 * @ctx: Pointer to pipe object
	 * @index: Pipe rectangle to operate on
	 * @data: Pointer to sde_hw_cp_cfg object containing bool data
	 */
	void (*setup_ucsc_unmult)(struct sde_hw_pipe *ctx,
		enum sde_sspp_multirect_index index, void *data);

	/**
	 * sde_setup_ucsc_alpha_ditherv1 - set UCSC ALPHA DITHER cp block
	 * @ctx: Pointer to pipe object
	 * @index: Pipe rectangle to operate on
	 * @data: Pointer to sde_hw_cp_cfg object containing bool data
	 */
	void (*setup_ucsc_alpha_dither)(struct sde_hw_pipe *ctx,
		enum sde_sspp_multirect_index index, void *data);

	/**
	 * setup_cac_ctrl - set CAC mode for each sspp
	 * @ctx: Pointer to pipe object
	 * @cac_mode: cac mode for that particular pipe
	 */
	void (*setup_cac_ctrl)(struct sde_hw_pipe *ctx, u32 cac_mode);

	/**
	 * setup_scaler_cac - set CAC scaler params for each sspp
	 * @ctx: Pointer to pipe object
	 * @cac_cfg: cac scaler config for each sspp
	 */
	void (*setup_scaler_cac)(struct sde_hw_pipe *ctx,
		struct sde_hw_cac_cfg *cac_cfg);

	/**
	 * setup_img_size - set img size params for CAC
	 * @ctx: Pointer to pipe object
	 * @img_rec: Pointer to image rect structure
	 */
	void (*setup_img_size)(struct sde_hw_pipe *ctx,
		struct sde_rect *img_rec);
};

/**
 * struct sde_hw_pipe - pipe description
 * @base: hardware block base structure
 * @hw: block hardware details
 * @catalog: back pointer to catalog
 * @mdp: pointer to associated mdp portion of the catalog
 * @idx: pipe index
 * @cap: pointer to layer_cfg
 * @ops: pointer to operations possible for this pipe
 * @dpu_idx: dpu index
 */
struct sde_hw_pipe {
	struct sde_hw_blk_reg_map hw;
	struct sde_mdss_cfg *catalog;
	struct sde_mdp_cfg *mdp;

	/* Pipe */
	enum sde_sspp idx;
	struct sde_sspp_cfg *cap;

	/* Ops */
	struct sde_hw_sspp_ops ops;
	struct sde_hw_ctl *ctl;

	u32 dpu_idx;
};

/**
 * sde_hw_sspp_init - initializes the sspp hw driver object.
 * Should be called once before accessing every pipe.
 * @idx:  Pipe index for which driver object is required
 * @addr: Mapped register io address of MDP
 * @catalog : Pointer to mdss catalog data
 * @is_virtual_pipe: is this pipe virtual pipe
 * @client: Pointer to VBIF clock client info
 * @dpu_idx: dpu index
 */
struct sde_hw_pipe *sde_hw_sspp_init(enum sde_sspp idx,
		void __iomem *addr, struct sde_mdss_cfg *catalog,
		bool is_virtual_pipe, struct sde_vbif_clk_client *client,
		u32 dpu_idx);

/**
 * sde_hw_sspp_destroy(): Destroys SSPP driver context
 * should be called during Hw pipe cleanup.
 * @ctx:  Pointer to SSPP driver context returned by sde_hw_sspp_init
 */
void sde_hw_sspp_destroy(struct sde_hw_pipe *ctx);

#endif /*_SDE_HW_SSPP_H */

