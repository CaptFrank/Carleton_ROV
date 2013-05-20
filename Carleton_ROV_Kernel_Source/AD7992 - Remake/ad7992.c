/*
 * Driver for Analog Devices 2 - channel, 12-Bit ADC with I2C-Compatible Interface in 10-Lead MSOP
 *
 *
 * 
 * Derived from:
 *
 *  Driver for Linear Technology LTC2990 Quad I2C Voltage, Current
 * and Temperature Monitor
 *  Copyright (C) 2011 AppearTV AS.
 *
 * Datasheet: http://www.analog.com/static/imported-files/data_sheets/AD7992.pdf
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
#define AD7992_CONVERSION    	0x00
#define AD7992_ALERT   			0x01
#define AD7992_CONFIGURATION   	0x02
#define AD7992_TIMER     		0x03
#define AD7992_DATA1_L   		0x04
#define AD7992_DATA1_H   		0x05
#define AD7992_HYSTERESIS1    	0x06
#define AD7992_DATA2_L      	0x07
#define AD7992_DATA2_H      	0x08
#define AD7992_HYSTERESIS2  	0x09

struct ad7992_data {
        struct device *hwmon_dev;

        struct mutex update_lock;
        bool valid;
        unsigned long last_updated; /* in jiffies */

        /* Registers */
        u8 regs[10];
};

static struct ad7992_data *ad7992_update_device(struct device *dev)
{
        struct i2c_client *client = to_i2c_client(dev);
        struct ad7992_data *data = i2c_get_clientdata(client);
        struct ad7992_data *ret = data;

        mutex_lock(&data->update_lock);

        /*
         * The chip's A/D updates 6 times per second
         * (Conversion Rate 6 - 9 Hz)
         */
        if (time_after(jiffies, data->last_updated + HZ / 6) || !data->valid) {
                int i;

                dev_dbg(&client->dev, "Starting ad7992 update\n");

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
static int ad7992_get_value(struct ad7992_data *data, u8 reg)
{
        u32 val;
		/*
		 * Resolution 
		 */
        
        switch (reg) {
			case AD7992_DATA1_L:
				val = data->regs[reg];
				break;
			case AD7992_DATA1_H:
				val = data->regs[reg];
				break;
			case AD7992_HYSTERESIS1:
				val = data->regs[reg];
				break;
			case AD7992_DATA2_L:
				val = data->regs[reg];
				break;
			case AD7992_DATA2_H:
				val = data->regs[reg];
				break;
			case AD7992_HYSTERESIS2:
				val = data->regs[reg];
				break;
			default:
			/* If we get here, the developer messed up */
        	    WARN_ON_ONCE(1);
        	    val = 0;
                break;
        }

        return val;
}


static ssize_t ad7992_show_value(struct device *dev,
                                  struct device_attribute *da, char *buf)
{
        struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
        struct ad7992_data *data = ad7992_update_device(dev);
        int value;

        if (IS_ERR(data))
                return PTR_ERR(data);

        value = ad7992_get_value(data, attr->index);
        return snprintf(buf, PAGE_SIZE, "%d\n", value);
}

/*
 * Monitor Input.
 */
static SENSOR_DEVICE_ATTR(data1_L_input, S_IRUGO, \
        ad7992_show_value, NULL, AD7992_DATA1_L);
static SENSOR_DEVICE_ATTR(data1_H_input, S_IRUGO, \
        ad7992_show_value, NULL, AD7992_DATA1_H);
static SENSOR_DEVICE_ATTR(hysteresis1_input, S_IRUGO, \
        ad7992_show_value, NULL, AD7992_HYSTERESIS1);
static SENSOR_DEVICE_ATTR(data2_L_input, S_IRUGO, \
        ad7992_show_value, NULL, AD7992_DATA2_L);
static SENSOR_DEVICE_ATTR(data2_H_input, S_IRUGO, \
        ad7992_show_value, NULL, AD7992_DATA2_H);
static SENSOR_DEVICE_ATTR(hysteresis2_input, S_IRUGO, \
        ad7992_show_value, NULL, AD7992_HYSTERESIS2);


 /*
 * Finally, construct an array of pointers to members of the above objects,
 * as required for sysfs_create_group()
 */
static struct attribute *ad7992_attributes[] = {
        &sensor_dev_attr_data1_L_input.dev_attr.attr,
        &sensor_dev_attr_data1_H_input.dev_attr.attr,
		&sensor_dev_attr_hysteresis1_input.dev_attr.attr,
		&sensor_dev_attr_data2_L_input.dev_attr.attr,
		&sensor_dev_attr_data2_H_input.dev_attr.attr,
		&sensor_dev_attr_hysteresis2_input.dev_attr.attr,
		
        NULL,
};

static const struct attribute_group ad7992_group = {
        .attrs = ad7992_attributes,
};

static int ad7992_probe(struct i2c_client *client,
                         const struct i2c_device_id *id)
{
        struct i2c_adapter *adapter = client->adapter;
        struct ad7992_data *data;
        int ret;

        if (!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_BYTE_DATA))
                return -ENODEV;

        data = devm_kzalloc(&client->dev, sizeof(*data), GFP_KERNEL);
        if (!data)
                return -ENOMEM;

        i2c_set_clientdata(client, data);
        mutex_init(&data->update_lock);

        /* Register sysfs hooks */
        ret = sysfs_create_group(&client->dev.kobj, &ad7992_group);
        if (ret)
                return ret;

        data->hwmon_dev = hwmon_device_register(&client->dev);
        if (IS_ERR(data->hwmon_dev)) {
                ret = PTR_ERR(data->hwmon_dev);
                goto out_hwmon_device_register;
        }

        return 0;
 
out_hwmon_device_register:
        sysfs_remove_group(&client->dev.kobj, &ad7992_group);
        return ret;
}

static int ad7992_remove(struct i2c_client *client)
{
        struct ad7992_data *data = i2c_get_clientdata(client);

        hwmon_device_unregister(data->hwmon_dev);
        sysfs_remove_group(&client->dev.kobj, &ad7992_group);

        return 0;
}

static const struct i2c_device_id ad7992_id[] = {
        { "ad7992", 0 },
        { }
};
MODULE_DEVICE_TABLE(i2c, ad7992_id);

/* This is the driver that will be inserted */
static struct i2c_driver ad7992_driver = {
        .driver = {
                .name   = "ad7992",
        },
        .probe          = ad7992_probe,
        .remove         = ad7992_remove,
        .id_table       = ad7992_id,
};

module_i2c_driver(ad7992_driver);

MODULE_AUTHOR("Rafael Guerra<faelwar@gmail.com>");
MODULE_DESCRIPTION("AD7992 driver");
MODULE_LICENSE("GPL");
