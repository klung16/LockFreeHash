#!/bin/bash
#PBS -q phi

execdir=/home/klung/LockFreeHash/src/  # Directory containing your script
seq_exe=seq-monitor
coarse_exe=coarse-grained-monitor
fine_exe=fine-grained-monitor
hoh_exe=hoh-monitor
lf_exe=lf-monitor
lfback_exe=lfback-monitor
rtm_exe=rtm-monitor

# Run my executable
cd ${execdir}

echo ""
echo "-------------------------- Starting monitor_all --------------------------"
echo ""

echo "  --------------------------"
echo "  Running seq-monitor"
./${seq_exe} -n ${num_ops} -l ${load_factor} -d ${delete_ratio} -i ${insert_ratio} -t ${num_threads}
echo "  Complete!"
echo "  --------------------------"
echo ""

echo "  --------------------------"
echo "  Running coarse-grained-monitor"
./${coarse_exe} -n ${num_ops} -l ${load_factor} -d ${delete_ratio} -i ${insert_ratio} -t ${num_threads}
echo "  Complete!"
echo "  --------------------------"
echo ""

echo "  --------------------------"
echo "  Running fine-grained-monitor"
./${fine_exe} -n ${num_ops} -l ${load_factor} -d ${delete_ratio} -i ${insert_ratio} -t ${num_threads}
echo "  Complete!"
echo "  --------------------------"
echo ""

echo "  --------------------------"
echo "  Running hoh-monitor"
./${hoh_exe} -n ${num_ops} -l ${load_factor} -d ${delete_ratio} -i ${insert_ratio} -t ${num_threads}
echo "  Complete!"
echo "  --------------------------"
echo ""

echo "  --------------------------"
echo "  Running lf-monitor"
./${lf_exe} -n ${num_ops} -l ${load_factor} -d ${delete_ratio} -i ${insert_ratio} -t ${num_threads}
echo "  Complete!"
echo "  --------------------------"
echo ""

echo "  --------------------------"
echo "  Running lfback-monitor"
./${lfback_exe} -n ${num_ops} -l ${load_factor} -d ${delete_ratio} -i ${insert_ratio} -t ${num_threads}
echo "  Complete!"
echo "  --------------------------"
echo ""

echo "  --------------------------"
echo "  Running rtm-monitor"
./${rtm_exe} -n ${num_ops} -l ${load_factor} -d ${delete_ratio} -i ${insert_ratio} -t ${num_threads}
echo "  Complete!"
echo "  --------------------------"
echo ""

echo "-------------------------- All Benchmarking Tests Complete --------------------------"
