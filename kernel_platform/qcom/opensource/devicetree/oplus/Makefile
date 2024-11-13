SUN_BASE_DTB += ../qcom/sun.dtb ../qcom/sun-v2.dtb ../qcom/sunp.dtb ../qcom/sunp-v2.dtb \
                ../qcom/sun-tp.dtb ../qcom/sun-tp-v2.dtb ../qcom/sunp-tp.dtb ../qcom/sunp-tp-v2.dtb

OPLUS_SUN_BOARDS += dodge-23821-sun-overlay.dtbo\
                    dodge-23821-sun-overlay-T0.dtbo


dtb-y += $(OPLUS_SUN_BOARDS) $(SUN_BASE_DTB)

always-y        := $(dtb-y)
subdir-y        := $(dts-dirs)
clean-files     := *.dtb *.dtbo
