#!/bin/bash

systemctl stop inf3995-miner
systemctl stop inf3995-rest

systemctl disable inf3995-miner
systemctl disable inf3995-rest

journalctl --rotate
journalctl --vacuum-time=1s

rm -rf /srv/inf3995/miner/*
rm -rf /srv/inf3995/rest/*
