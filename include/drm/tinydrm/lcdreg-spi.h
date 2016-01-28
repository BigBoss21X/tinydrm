/*
 * Copyright (C) 2016 Noralf Trønnes
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef __LINUX_LCDREG_SPI_H
#define __LINUX_LCDREG_SPI_H

#include <drm/tinydrm/lcdreg.h>
#include <linux/spi/spi.h>

enum lcdreg_spi_mode {
	LCDREG_SPI_4WIRE, /* 8-bit + D/CX line, MIPI DBI Type C option 3 */
	LCDREG_SPI_3WIRE, /* 9-bit inc. D/CX bit, MIPI DBI Type C option 1 */
	LCDREG_SPI_STARTBYTE,
};

struct lcdreg_spi_config {
	enum lcdreg_spi_mode mode;
	unsigned def_width;
	bool readable;
	u32 id;
	char *dc_name;
	struct gpio_desc *dc;
	struct gpio_desc *reset;
	u8 (*startbyte)(struct lcdreg *reg, struct lcdreg_transfer *tr,
			bool read);
};

struct lcdreg *devm_lcdreg_spi_init(struct spi_device *sdev,
				    const struct lcdreg_spi_config *config);
int devm_lcdreg_spi_of_parse(struct device *dev,
			     struct lcdreg_spi_config *cfg);

static inline struct lcdreg *devm_lcdreg_spi_init_of(struct spi_device *sdev,
						     enum lcdreg_spi_mode mode)
{
	struct lcdreg_spi_config cfg = {
		.mode = mode,
	};
	int ret;

	ret = devm_lcdreg_spi_of_parse(&sdev->dev, &cfg);
	if (ret)
		return ERR_PTR(ret);

	return devm_lcdreg_spi_init(sdev, &cfg);
}

#endif /* __LINUX_LCDREG_SPI_H */
