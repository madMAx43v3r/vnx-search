#!/bin/bash

while true; do
	ssh -i ~/mad-ovh -C -N ubuntu@fcknwo.com -L 9999:fcknwo.com:9999
	sleep 60
done

