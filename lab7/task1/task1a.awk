#! /usr/bin/awk -f

BEGIN {   
    for(i in ARGV)
        print "ARGV[" i "] = " ARGV[i]
}
