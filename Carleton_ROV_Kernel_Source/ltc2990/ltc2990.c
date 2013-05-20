/*
 * Driver for Linear Technology LTC2990 Quad I2C Voltage, Current
 * and Temperature Monitor
 *
 *
 * 
 * Derived from:
 *
 *  Driver for Linear Technology LTC2990 Quad I2C Voltage, Current
 * and Temperature Monitor
 *  Copyright (C) 2011 AppearTV AS.
 *
 * Datasheet: http://cds.linear.com/docs/en/datasheet/2990fc.pdf
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/hwmon.h>
#include <linux/hwmon-sysfs.h>
#include <linux/jiffies.h>

/* chip registers */
#define LTC2990_STATUS    0x00
#define LTC2990_CONTROL   0x01
#define LTC2990_TRIGGER   0x02
#define LTC2990_EMPTY     0x03
#define LTC2990_T_INT_H   0x04
#define LTC2990_T_INT_L   0x05
#define LTC2990_V1_H      0x06
#define LTC2990_V1_L      0x07
#define LTC2990_V2_H      0x08
#define LTC2990_V2_L      0x09
#define LTC2990_V3_H      0x0A
#define LTC2990_V3_L      0x0B
#define LTC2990_V4_H      0x0C
#define LTC2990_V4_L      0x0D
#define LTC2990_VCC_H     0x0E
#define LTC2990_VCC_L     0x0F

struct ltc2990_data {
        struct device *hwmon_dev;

        struct mutex update_lock;
        bool valid;
        unsigned long last_updated; /* in jiffies */

        /* Registers */
        u8 regs[16];
};

static struct ltc2990_data *ltc2990_update_device(struct device *dev)
{
        struct i2c_client *client = to_i2c_client(dev);
        struct ltc2990_data *data = i2c_get_clientdata(client);
        struct ltc2990_data *ret = data;

        mutex_lock(&data->update_lock);

        /*
         * The chip's A/D updates 6 times per second
         * (Conversion Rate 6 - 9 Hz)
         */
        if (time_after(jiffies, data->last_updated + HZ / 6) || !data->valid) {
                int i;

                dev_dbg(&client->dev, "Starting ltc2990 update\n");

                /* Read all registers */
                for (i = 0; i < ARRAY_SIZE(data->regs); i++) {
                        int val;

                        val = i2c_smbus_read_byte_data(client, i);
                        if (unlikely(val < 0)) {
                                dev_dbg(dev,
                                        "Failed to read ADC value: error %d\n",
                                        val);
                                ret = ERR_PTR(val);
                                goto abort;
                        }
                        data->regs[i] = val;
                }
                data->last_updated = jiffies;
                data->valid = 1;
        }
abort:
        mutex_unlock(&data->update_lock);
        return ret;
}

/* Return the voltage from the given register in mV */
static int ltc2990_get_value(struct ltc2990_data *data, u8 reg)
{
        u32 val;
		/*
		 * Temperature sensor has a resolution of 0.0625
		 */
        
        switch (reg) {
		case LTC2990_T_INT_H:
			val = (data->regs[reg] << 8) + (data->regs[reg + 1]);
			val = val * 0.0625;
			break;
		case LTC2990_V1_H:
			val = (data->regs[reg] << 8) + (data->regs[reg + 1]);
			val = val * 0.0625;
			break;
		case LTC2990_V2_H:
			val = (data->regs[reg] << 8) + (data->regs[reg + 1]);
			val = val * 0.0625;
			break;
		case LTC2990_V3_H:
			val = (data->regs[reg] << 8) + (data->regs[reg + 1]);
			val = val * 0.0625;
			break;
		case LTC2990_V4_H:
			val = (data->regs[reg] << 8) + (data->regs[reg + 1]);
			val = val * 0.0625;
			break;
		case LTC2990_VCC_H:
			//305.18uV resolution.
			//Vcc = RESULT + 2.5V
			val = (data->regs[reg] << 8) + (data->regs[reg + 1]);
			val = (val * 305.18e-6) + 2.5;
			break;
		default:
			/* If we get here, the developer messed up */
        	        WARN_ON_ONCE(1);
        	        val = 0;
                	break;
        }

        return val;
}

static ssize_t ltc2990_show_value(struct device *dev,
                                  struct device_attribute *da, char *buf)
{
        struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
        struct ltc2990_data *data = ltc2990_update_device(dev);
        int value;

        if (IS_ERR(data))
                return PTR_ERR(data);

        value = ltc2990_get_value(data, attr->index);
        return snprintf(buf, PAGE_SIZE, "%d\n", value);
}

/*
 * Monitor Input.
 */
static SENSOR_DEVICE_ATTR(v1_input, S_IRUGO, \
        ltc2990_show_value, NULL, LTC2990_V1_H);
static SENSOR_DEVICE_ATTR(v2_input, S_IRUGO, \
        ltc2990_show_value, NULL, LTC2990_V2_H);
static SENSOR_DEVICE_ATTR(v3_input, S_IRUGO, \
        ltc2990_show_value, NULL, LTC2990_V3_H);
static SENSOR_DEVICE_ATTR(v4_input, S_IRUGO, \
        ltc2990_show_value, NULL, LTC2990_V4_H);
		
/* Vcc Input */
static SENSOR_DEVICE_ATTR(vcc_input, S_IRUGO, \
        ltc2990_show_value, NULL, LTC2990_VCC_H);

 /*
 * Finally, construct an array of pointers to members of the above objects,
 * as required for sysfs_create_group()
 */
static struct attribute *ltc2990_attributes[] = {
        &sensor_dev_attr_v1_input.dev_attr.attr,
        &sensor_dev_attr_v2_input.dev_attr.attr,
		&sensor_dev_attr_v3_input.dev_attr.attr,
		&sensor_dev_attr_v4_input.dev_attr.attr,
		
        &sensor_dev_attr_vcc_input.dev_attr.attr,

        NULL,
};

static const struct attribute_group ltc2990_group = {
        .attrs = ltc2990_attributes,
};

static int ltc2990_probe(struct i2c_client *client,
                         const struct i2c_device_id *id)
{
        struct i2c_adapter *adapter = client->adapter;
        struct ltc2990_data *data;
        int ret;

        if (!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_BYTE_DATA))
                return -ENODEV;

        data = devm_kzalloc(&client->dev, sizeof(*data), GFP_KERNEL);
        if (!data)
                return -ENOMEM;

        i2c_set_clientdata(client, data);
        mutex_init(&data->update_lock);

        /* Register sysfs hooks */
        ret = sysfs_create_group(&client->dev.kobj, &ltc2990_group);
        if (ret)
                return ret;

        data->hwmon_dev = hwmon_device_register(&client->dev);
        if (IS_ERR(data->hwmon_dev)) {
                ret = PTR_ERR(data->hwmon_dev);
                goto out_hwmon_device_register;
        }

        return 0;
 
out_hwmon_device_register:
        sysfs_remove_group(&client->dev.kobj, &ltc2990_group);
        return ret;
}

static int ltc2990_remove(struct i2c_client *client)
{
        struct ltc2990_data *data = i2c_get_clientdata(client);

        hwmon_device_unregister(data->hwmon_dev);
        sysfs_remove_group(&client->dev.kobj, &ltc2990_group);

        return 0;
}

static const struct i2c_device_id ltc2990_id[] = {
        { "ltc2990", 0 },
        { }
};
MODULE_DEVICE_TABLE(i2c, ltc2990_id);

/* This is the driver that will be inserted */
static struct i2c_driver ltc2990_driver = {
        .driver = {
                .name   = "ltc2990",
        },
        .probe          = ltc2990_probe,
        .remove         = ltc2990_remove,
        .id_table       = ltc2990_id,
};

module_i2c_driver(ltc2990_driver);

MODULE_AUTHOR("Rafael Guerra<faelwar@gmail.com>");
MODULE_DESCRIPTION("LTC2990 driver");
MODULE_LICENSE("GPL");
