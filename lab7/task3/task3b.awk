#! /usr/bin/awk -f
BEGIN{ FS = ","
       count = 0
}
{
    if(NR != 1 && $0 ~ /(fun|fan)/){
        count += 1
    }
}
END {
    print "The number of poem that have fun or fan is : " count 
}

