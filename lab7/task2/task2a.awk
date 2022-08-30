#! /usr/bin/awk -f
BEGIN{ FS = ","}
{
    if(NR != 1 && $4 ~ /(standard)/){
        sum = $6 + $7 + $8
        sum = sum / 3
        print "AVERAGE: " sum
    }
}

