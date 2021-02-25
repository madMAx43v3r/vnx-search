#!/bin/bash

while true; do
	vnxservice -n :8989 -x QueryEngine query test
	sleep 60
done

