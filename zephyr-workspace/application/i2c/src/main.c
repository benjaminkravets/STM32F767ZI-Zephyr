/*
 * Copyright (c) 2024 Open Pixel Systems
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/i2c.h>

#define SLEEP_TIME_MS 5000

static const struct device *bus = DEVICE_DT_GET(DT_NODELABEL(i2c1));
static char last_byte;

//static const struct i2c_dt_spec dev_i2c = I2C_DT_SPEC_GET(DT_NODELABEL(i2c1));
/*
 * @brief Callback which is called when a write request is received from the master.
 * @param config Pointer to the target configuration.
 */
int sample_target_write_requested_cb(struct i2c_target_config *config)
{
	printk("sample target write requested\n");
	return 0;
}

/*
 * @brief Callback which is called when a write is received from the master.
 * @param config Pointer to the target configuration.
 * @param val The byte received from the master.
 */
int sample_target_write_received_cb(struct i2c_target_config *config, uint8_t val)
{
	printk("sample target write received: 0x%02x\n", val);
	last_byte = val;
	return 0;
}

/*
 * @brief Callback which is called when a read request is received from the master.
 * @param config Pointer to the target configuration.
 * @param val Pointer to the byte to be sent to the master.
 */
int sample_target_read_requested_cb(struct i2c_target_config *config, uint8_t *val)
{
	printk("sample target read request: 0x%02x\n", *val);
	*val = 0x42;
	return 0;
}

/*
 * @brief Callback which is called when a read is processed from the master.
 * @param config Pointer to the target configuration.
 * @param val Pointer to the next byte to be sent to the master.
 */
int sample_target_read_processed_cb(struct i2c_target_config *config, uint8_t *val)
{
	printk("sample target read processed: 0x%02x\n", *val);
	*val = 0x43;
	return 0;
}

/*
 * @brief Callback which is called when the master sends a stop condition.
 * @param config Pointer to the target configuration.
 */
int sample_target_stop_cb(struct i2c_target_config *config)
{
	printk("sample target stop callback\n");
	return 0;
}

static struct i2c_target_callbacks sample_target_callbacks = {
	.write_requested = sample_target_write_requested_cb,
	.write_received = sample_target_write_received_cb,
	.read_requested = sample_target_read_requested_cb,
	.read_processed = sample_target_read_processed_cb,
	.stop = sample_target_stop_cb,
};

int main(void)
{
	struct i2c_target_config target_cfg = {
		.address = 0x60,
		.callbacks = &sample_target_callbacks,
	};

	printk("i2c custom target sample\n");

	if (i2c_target_register(bus, &target_cfg) < 0) {
		printk("Failed to register target\n");
		return -1;
	}

	k_msleep(5000);

	if (i2c_target_unregister(bus, &target_cfg) < 0) {
		printk("Failed to unregister target\n");
		return -1;
	}

	//transaction 
	// if(!i2c_is_ready_dt(&dev_i2c)) {
	// 	return -ENODEV;
	// }
	struct i2c_msg msg;
	uint8_t buf[2];

	msg.buf = buf;
	msg.len = 2;
	msg.flags = I2C_MSG_WRITE;
	buf[0] = 0x80;
	i2c_transfer(bus, &msg, 1, 0x01);

	// msg.buf = buf;
	// msg.len = 2;
	// msg.flags = I2C_MSG_READ | I2C_MSG_RESTART;
	// i2c_transfer(bus, &msg, 1, 0x01);
	

	
	//transaction

	while (1) {
		i2c_transfer(bus, &msg, 1, 0x01);

		printf("End zone \r\n");
		k_msleep(SLEEP_TIME_MS);
	}

	return 0;
}
