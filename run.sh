
set -e

make all

gdb -x debug.gdb bin/prepare
