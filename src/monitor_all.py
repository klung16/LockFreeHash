# Script to run all monitor 

import sys, argparse, subprocess

DEFAULT_LOAD_FACTOR = 10
DEFAULT_NUM_TEST_VALUES = 1000000
DEFAULT_DELETE_RATIO = 0.1
DEFAULT_INSERT_RATIO = 0.1
DEFAULT_NUM_THREADS = 4

if __name__ == "__main__":
    
    print("\n")
    print("-------------------------- Starting monitor_all --------------------------")
    print("\n")
    
    parser = argparse.ArgumentParser()
    parser.add_argument("-n", dest="num_ops", default=DEFAULT_NUM_TEST_VALUES, type=int,
                        help="Number of operations (Default to 1000000)")
    parser.add_argument("-l", dest="load_factor", default=DEFAULT_LOAD_FACTOR, type=int,
                        help="Load factor (number of data points / number of buckets)")
    parser.add_argument("-d", dest="delete_ratio", default=DEFAULT_DELETE_RATIO, type=float,
                        help="Delete ratio for total number of operations (0.0 <= DELETE_RATIO + INSERT_RATIO <= 1.0)")
    parser.add_argument("-i", dest="insert_ratio", default=DEFAULT_INSERT_RATIO, type=float,
                        help="Insert ratio for total number of operations (0.0 <= DELETE_RATIO + INSERT_RATIO <= 1.0)")
    parser.add_argument("-t", dest="num_threads", default=DEFAULT_NUM_THREADS, type=int,
                        help="Set number of threads")
    parser.add_argument("-s", action="store_false", dest="seq", default=True, help="Turn off sequential implementation")
    args = parser.parse_args()
    
    #Legality Check
    write_ratio = args.delete_ratio + args.insert_ratio
    if (args.num_ops <= 0 or args.load_factor <= 0 or args.num_threads <= 0 or
        args.delete_ratio < 0 or args.insert_ratio < 0 or write_ratio < 0 or write_ratio > 1):
        print("Invalid arguments!")
        parser.print_help()
        parser.exit()
        
    if (args.seq):
        print("  --------------------------")
        print("  Running seq-monitor")    
        subprocess.call(["./seq-monitor", "-n", str(args.num_ops), "-l", str(args.load_factor),
                         "-d", str(args.delete_ratio), "-i", str(args.insert_ratio), "-t", str(args.num_threads)])
        print("  Complete!")
        print("  --------------------------")
        print("\n")
        
    print("  --------------------------")
    print("  Running coarse-grained-monitor")    
    subprocess.call(["./coarse-grained-monitor", "-n", str(args.num_ops), "-l", str(args.load_factor),
                     "-d", str(args.delete_ratio), "-i", str(args.insert_ratio), "-t", str(args.num_threads)])
    print("  Complete!")
    print("  --------------------------")
    print("\n")
        
    print("  --------------------------")
    print("  Running fine-grained-monitor")    
    subprocess.call(["./fine-grained-monitor", "-n", str(args.num_ops), "-l", str(args.load_factor),
                     "-d", str(args.delete_ratio), "-i", str(args.insert_ratio), "-t", str(args.num_threads)])
    print("  Complete!")
    print("  --------------------------")
    print("\n")
        
    print("  --------------------------")
    print("  Running lf-monitor")    
    subprocess.call(["./lf-monitor", "-n", str(args.num_ops), "-l", str(args.load_factor),
                     "-d", str(args.delete_ratio), "-i", str(args.insert_ratio), "-t", str(args.num_threads)])
    print("  Complete!")
    print("  --------------------------")
    print("\n")
    
    print("  --------------------------")
    print("  Running lfback-monitor")    
    subprocess.call(["./lfback-monitor", "-n", str(args.num_ops), "-l", str(args.load_factor),
                     "-d", str(args.delete_ratio), "-i", str(args.insert_ratio), "-t", str(args.num_threads)])
    print("  Complete!")
    print("  --------------------------")
    print("\n")
    
    print("-------------------------- All Benchmarking Tests Complete --------------------------")
    
        
    
        
        
        
    
        