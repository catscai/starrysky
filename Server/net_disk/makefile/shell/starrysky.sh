#!/bin/bash

PROGRAM=/root/c/net_disk/makefile/src/app
MYSQL="/usr/local/mysql/support-files/mysql.server start"
MYSQLPARM=/usr/local/mysql/
while :
do
    LINE=`ps -e -o args | grep "^${MYSQLPARM}" | wc -l`;
    if [ ${LINE} -eq 0 ]; then
        ${MYSQL}
    fi
    LINE=`ps -e -o args | grep "^${PROGRAM}" | wc -l`;
    if [ ${LINE} -eq 0 ]; then
        ${PROGRAM};
    fi

    sleep 20
done

exit 0

