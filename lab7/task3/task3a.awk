#! /usr/bin/awk -f
BEGIN{ FS = ","
       OFS = "---"
}
{
    if(NR != 1 && $0 ~ /spring/){
        print $2,$3,$4
    }
}

