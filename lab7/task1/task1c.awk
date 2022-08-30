#! /usr/bin/awk -f
BEGIN{ FS = ","}
{
    if(NR != 1 && $2 >= 1970){
        print "---------------------------------"
        print "Actor Name: " $4
        print "Movie Name: " $5
    }
}

