#!/bin/bash

execdir=/home/klung/LockFreeHash/src/  # Directory containing your script
exe=monitor_all.sh

load_factor=10
num_ops=1000000
delete_ratio=0.1
insert_ratio=0.1
num_threads=4

while getopts n:l:d:i:t: option
do
case "${option}"
in
n) num_ops=${OPTARG};;
l) load_factor=${OPTARG};;
d) delete_ratio=${OPTARG};;
i) insert_ratio=${OPTARG};;
t) num_threads=$OPTARG;;
esac
done

# Run my executable
cd ${execdir}
qsub -v load_factor=${load_factor},num_ops=${num_ops},delete_ratio=${delete_ratio},insert_ratio=${insert_ratio},num_threads=${num_threads} ./${exe}
