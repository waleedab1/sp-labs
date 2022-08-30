#! /usr/bin/awk -f
BEGIN{ FS = ","}
{
    if(NR != 1 && $1 >= 50 && $1 <= 70){
        print "Movie Name: " $5
    }
}

