#
# This file is generated during the build process. Do not modify or check into source control.
#
set remotetimeout 20
shell "/Applications/ModusToolbox 1.0/tools/modus-shell-1.0/bin/bash" -c 'set -m; trap "" INT; cd "/Applications/ModusToolbox 1.0/tools/wiced-tools-1.0/OpenOCD" && bin/openocd-all-brcm-libftdi -s scripts -f cmsis-dap.cfg -f psoc62.cfg -f "psoc62_gdb_jtag.cfg" &'
