#! /usr/bin/awk -f
BEGIN{ FS = ","
       OFS = "---"
}
{
    if(NR != 1 && $2 ~ /(happy)/){
        print $2,$3,$4
    }
}

