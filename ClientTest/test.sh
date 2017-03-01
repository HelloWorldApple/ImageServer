#!/bin/bash
start=$(date "+%s")
for a in $(seq 1 1000)
do
{

./Client;
} &
done

wait
now=$(date "+%s")
time=$((now-start))
echo "time:"
echo $time
