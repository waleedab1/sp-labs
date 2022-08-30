#! /usr/bin/awk -f

#{print $0}
#{print $3"\t"$4}
#$2 ~ /[a]/ {print $2"\t"$4}
#{print $1"\t"$4"\t"$3"\t"$2}
#    /English/ {cnt++}
#END{
#    print "count:", cnt
#}
#$4>87 {print $0}
