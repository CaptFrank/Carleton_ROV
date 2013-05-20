#!/bin/sh

#  UART1(GPS)_tty.sh
#  Cansat_2013
#
#  Created by Francis Papineau on 2012-08-13.
#  Copyright (c) 2012 Francis Papineau. All rights reserved.

# Setting the UART 1 portmap

echo "****** INSTALLING UART 1 MUX PIN ******

echo 20 > /sys/kernel/debug/omap_mux/uart1_rxd
echo 0 > /sys/kernel/debug/omap_mux/uart1_txd

echo "UART 1 (TX):"
cat /sys/kernel/debug/omap_mux/uart1_rxd
echo

echo "UART 1 (RX):"
cat /sys/kernel/debug/omap_mux/uart1_txd
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