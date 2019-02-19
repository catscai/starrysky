#!/bin/bash
declare -i TOTALNUM=0
FILELIST=

if [ $# -eq 0 ];then
    echo "Please input a folder";
    exit 1;
fi

FOLDERNAME=$1
RET=`echo ${FOLDERNAME} | grep ".*/$"`
if [ -z "$RET" ];then
    FOLDERNAME=`echo "${FOLDERNAME}/"`
fi
FILELIST=`ls ${FOLDERNAME} | egrep "\.[h|c](pp)?"`

for f in ${FILELIST}
do
   declare -i LINES=`wc -l "${FOLDERNAME}${f}" | cut -d' ' -f1`;
   TOTALNUM=`expr $TOTALNUM + $LINES`;
done

echo ${TOTALNUM};
exit 0;
