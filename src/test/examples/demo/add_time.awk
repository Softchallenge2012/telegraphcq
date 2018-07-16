BEGIN {FS=","}
{ print strftime("%Y-%m-%d %H:%M:%S %Z")","$1","$2}
#{ print strftime()","$1","$2}
