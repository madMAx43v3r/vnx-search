#!/bin/bash

cat $2 | while read in; do bash -c "$1 $in"; done

