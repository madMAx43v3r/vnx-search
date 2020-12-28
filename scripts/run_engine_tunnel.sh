#!/bin/bash

while true; do
	ssh -i ~/mad-ovh -C -N -R 8989:localhost:8989 ubuntu@fcknwo.com
	sleep 60
done

