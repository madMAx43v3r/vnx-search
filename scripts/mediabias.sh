#!/bin/bash

echo -n "	" >> out

wget -q -O - $1 | grep "<p>Source:" | grep -o '"http[s]\?://[^"]*"' | head -n 1 | awk 'NF{print $0 ","}' >> out

