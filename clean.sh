#!/bin/bash

find \( -name "*.cpp" -o -name "*.[hc]" -o -name "*.hpp" \) -exec del_trailing_space {} \;

line_count=`find \( -name "*.cpp" -o -name "*.[hc]" -o -name "*.hpp" \) -exec cat {} \; | awk 'BEGIN{c} {if (length>0) c+=1;} END{print c}'`
echo "total line count of source file: $line_count"
