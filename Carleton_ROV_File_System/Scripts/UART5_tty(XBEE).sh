#!/bin/sh

#  UART5_tty.sh
#  Cansat_2013
#
#  Created by Francis Papineau on 2012-08-11.
#  Copyright (c) 2012 Francis Papineau. All rights reserved.

# Setting the UART 5 portmap

echo "****** INSTALLING UART 5 MUX PIN ******

echo 24 > /sys/kernel/debug/omap_mux/lcd_data9
echo 4 > /sys/kernel/debug/omap_mux/lcd_data8

echo "UART 5 (TX):"
cat /sys/kernel/debug/omap_mux/lcd_data9
echo

echo "UART 5 (RX):"
cat /sys/kernel/debug/omap_mux/lcd_data8
echo


# UART - Pin     Filesystem location     Value (hex)     Notes
# UART1 - RX     /sys/kernel/debug/omap_mux/uart1_rxd    20      Mode 0 - Input
# UART1 - TX     /sys/kernel/debug/omap_mux/uart1_txd    0       Mode 0 - Output
# UART2 - RX     /sys/kernel/debug/omap_mux/spi0_d0      21      Mode 1 - Input
# UART2 - TX     /sys/kernel/debug/omap_mux/spi0_sclk    1       Mode 1 - Output
# UART4 - RX     /sys/kernel/debug/omap_mux/gpmc_wait0   26      Mode 6 - Input
# UART4 - TX     /sys/kernel/debug/omap_mux/gpmc_wpn     6       Mode 6 - Output
# UART5 - RX     /sys/kernel/debug/omap_mux/lcd_data9    24      Mode 4 - Input
# UART5 - TX     /sys/kernel/debug/omap_mux/lcd_data8    4       Mode 4 - Output