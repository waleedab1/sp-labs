#! /usr/bin/awk -f
BEGIN{ FS = "," 
    count = 0 
    print "======================="
    print "“Success Student List”"
    print "=======================\n"
}
{
    if(NR != 1 && $6 >= 80 && $7 >= 80 && $8 >= 80){
        print "Student Info: " $0
        count += 1;
    }
}
END {
    print "\nThe number of students : " count
}

