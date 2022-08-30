#! /usr/bin/awk -f
BEGIN{  OFS = " | "
        FS = "," 
}
{
    print $4,$3,$2
}
