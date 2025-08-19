#!/bin/bash
# the header should have the line:
# /* prototypes */
# after the header protection, the #includes, the #defines, the structures, etc.
# but before #endif

[[ -z "$1" ]] && echo "usage: $0 <header>.h"
which cproto > /dev/null || 
	alias cproto="docker run jbarratt42/cproto \
		-v $PWD:/app
		-v /usr/include:/usr/include"
tmp=$(mktemp)
sed '/\/\* prototypes \*\//q' $1 > $tmp
cproto -F"int\tf(a, b)" *.c | \
	   sed -E 's/^([a-z_]+) /\1\t/' | \
	   sed 's/^int\t/int\t\t/' | \
	   grep -v 'main(' >> $tmp
echo '#endif' >> $tmp
cp $tmp $1
