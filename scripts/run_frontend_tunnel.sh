#!/bin/bash

while true; do
	ssh -i ~/mad-ovh -C -N ubuntu@51.89.4.252 -L 5555:51.89.4.252:5555
	sleep 1
done

