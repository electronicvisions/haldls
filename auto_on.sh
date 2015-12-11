#!/bin/bash

FPGA=1
POWER_ON=1

screen -mDL expect auto_on.expect --fpga=$FPGA $POWER_ON

EXIT_LINE=`tail -1 screenlog.0`

if [ $EXIT_LINE == "OKOKMAGICOKOK" ]; then
    echo "seems to work..."
    exit 0
else
    echo "did not work?... pasting last line..."
    echo $EXIT_LINE
    exit 1
fi
