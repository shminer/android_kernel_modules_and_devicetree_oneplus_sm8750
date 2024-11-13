/*
 *   <author>     <data>      <desc>
 *   LiFenfen   2024/09/09  , add for select BDF by device-tree , bug id 7902090
 */

#include <linux/delay.h>
#include <linux/devcoredump.h>
#include <linux/elf.h>
#include <linux/jiffies.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#include <linux/pm_wakeup.h>
#include <linux/reboot.h>
#include <linux/rwsem.h>
#include <linux/suspend.h>
#include <linux/timer.h>
#include <linux/thermal.h>
#include <linux/version.h>

#include <soc/oplus/system/boot_mode.h>
#include <soc/oplus/system/oplus_project.h>

#define OPLUS_WIFI_BDF_NODE "oplus_bdf"
#define OPLUS_WIFI_BDF_NAME "bdf_name"
#define OPLUS_WIFI_BDF_TYPE "project_type"

#define OPLUS_WIFI_BDF_NAME_RFID "rfid"
#define OPLUS_WIFI_BDF_NAME_LENGTH 64

struct wifi_data {
	struct platform_device *plat_dev;
	struct device_node *of_node;
	const char *bdf_name_default;
	const char *bdf_name;
	char project_value[OPLUS_WIFI_BDF_NAME_LENGTH];
};

static struct wifi_data *plat_env;

static void oplus_wifi_set_wifi_data(struct wifi_data *wifi_data)
{
	plat_env = wifi_data;
}

struct wifi_data *oplus_wifi_get_wifi_data(void)
{
	return plat_env;
}

static void oplus_wifi_clear_wifi_data(void)
{
	if (plat_env != NULL) {
		devm_kfree(&plat_env->plat_dev->dev, plat_env);
		plat_env = NULL;
	}
}

static void get_project_value(const char *key, const char **value) {
	int ret;
	char config[OPLUS_WIFI_BDF_NAME_LENGTH];
	if (key == NULL || strlen(key) == 0)	{
		pr_err("get_project_value key is null\n");
		return;
	}

	// custom config for different project
	if (strcmp(key, OPLUS_WIFI_BDF_NAME_RFID) == 0) {
		ret = get_Modem_Version();
		pr_err("get_project_value key: %s ret: %d\n", key, ret);
		if (ret >= 0) {
			memset(config, 0, sizeof(config));
			snprintf(config, sizeof(config), "%d", ret);
			*value = config;
			return;
		}
	}

	return;
}

static const char* get_oplus_wifi_value_of_node(struct device_node *of_node, const char *name) {
	const char *ret;

	if (!name || !of_node) {
		pr_err("get_oplus_wifi_value_of_node NULL node or name\n");
		return NULL;
	}

	if (of_property_read_string(of_node, name, &ret)) {
		pr_err("get_oplus_wifi_value_of_node not find property %s\n", name);
		return NULL;
	} else {
		if (ret) {
			pr_err("get_oplus_wifi_value_of_node name %s, value: %s\n", name, ret);
			return ret;
		}
		return NULL;
	}
	return NULL;
}

static const char* oplus_wifi_bdf_name_update_by_dts(struct wifi_data *wifi_data) {
	const char *name;
	const char *project_type;
	struct device_node *child_node;
	char child_node_name[OPLUS_WIFI_BDF_NAME_LENGTH];
	struct device_node *node;
	const char *project_value;

	if (!wifi_data) {
		return NULL;
	}

	node = wifi_data->of_node;
	while (node) {
		pr_err("oplus_wifi_bdf_name_update_by_dts node %s property %s\n", node->name, OPLUS_WIFI_BDF_TYPE);
		project_type = get_oplus_wifi_value_of_node(node, OPLUS_WIFI_BDF_TYPE);
		if(project_type) {
			project_value = wifi_data->project_value;
			get_project_value(project_type, &project_value);
			if (project_value) {
				/* get next node bdf name of oplus DT Entries */
				memset(child_node_name, 0, sizeof(child_node_name));
				scnprintf(child_node_name, sizeof(child_node_name), "%s_%s", project_type, project_value);
				child_node = of_get_child_by_name(node, child_node_name);
				if (child_node) {
					pr_err("oplus_wifi_bdf_name_update_by_dts find node %s\n", child_node->name);
					name = get_oplus_wifi_value_of_node(child_node, OPLUS_WIFI_BDF_NAME);
					if(name) {
						pr_err("oplus_wifi_bdf_name_update_by_dts find node %s property %s, value: %s\n", child_node->name, OPLUS_WIFI_BDF_NAME, name);
						of_node_put(child_node);
						break;
					} else {
						pr_err("oplus_wifi_bdf_name_update_by_dts not find node %s property %s, try next node\n", child_node->name, OPLUS_WIFI_BDF_NAME);
					}
					node = child_node;
					of_node_put(child_node);
				} else {
					pr_err("oplus_wifi_bdf_name_update_by_dts %s not find chid node %s\n", node->name, child_node_name);
					break;
				}
			} else {
				pr_err("oplus_wifi_bdf_name_update_by_dts %s read project_type %s without valid result, use default bdf name\n", node->name, project_type);
				break;
			}
		} else {
			pr_err("oplus_wifi_bdf_name_update_by_dts node %s not find property %s\n", node->name, OPLUS_WIFI_BDF_TYPE);
			break;
		}
	}

	return name;
}

static void oplus_wifi_bdf_init(struct wifi_data *wifi_data) {
	const char *bdf_name;
	struct device_node *child_node;
	struct device *dev = &wifi_data->plat_dev->dev;

	/* get default bdf name of oplus DT Entries */
	child_node = of_get_child_by_name(dev->of_node, OPLUS_WIFI_BDF_NODE);
	if (child_node) {
		pr_err("oplus_wifi_bdf_init find node %s\n", child_node->name);
		wifi_data->of_node = child_node;
		bdf_name = get_oplus_wifi_value_of_node(child_node, OPLUS_WIFI_BDF_NAME);
		if(bdf_name) {
			wifi_data->bdf_name_default = bdf_name;
		}
		of_node_put(child_node);
	} else {
		pr_err("plus_wifi_bdf_init not find node %s\n", OPLUS_WIFI_BDF_NODE);
	}
	return;
}

const char* get_oplus_wifi_bdf(void) {
	struct wifi_data *wifi_data = oplus_wifi_get_wifi_data();
	wifi_data->bdf_name = oplus_wifi_bdf_name_update_by_dts(wifi_data);

	if (wifi_data && wifi_data->bdf_name) {
		return wifi_data->bdf_name;
	} else if (wifi_data && wifi_data->bdf_name_default){
		return wifi_data->bdf_name_default;
	}
	return NULL;
}

int oplus_wifi_init(struct platform_device *plat_dev)
{
	int ret = 0;
	struct wifi_data *wifi_data;

	if (oplus_wifi_get_wifi_data()) {
		pr_err("Driver is already initialized!\n");
		ret = -EEXIST;
		goto out;
	}

	wifi_data = devm_kzalloc(&plat_dev->dev, sizeof(*wifi_data),
				 GFP_KERNEL);
	if (!wifi_data) {
		ret = -ENOMEM;
		goto out;
	}

	wifi_data->plat_dev = plat_dev;
	oplus_wifi_bdf_init(wifi_data);
	oplus_wifi_set_wifi_data(wifi_data);
	pr_info("oplus_wifi_init successfully.\n");

	return 0;

out:
	return ret;
}

void oplus_wifi_deinit(void)
{
	oplus_wifi_clear_wifi_data();
}