#!/bin/bash

docker build --network=host -t gpol/inf3995:latest  -f Dockerfile .
docker build --network=host -t gpol/inf3995:package -f Dockerfile.package .

until docker push gpol/inf3995:latest;  do :; done
until docker push gpol/inf3995:package; do :; done
