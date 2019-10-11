#!/bin/bash
#
# Usage: ./createCert.sh [prefix]

set -o xtrace

# Password is equipe01
# Inspired from: https://gist.github.com/fntlnz/cf14feb5a46b2eda428e000157447309
# openssl genrsa -out $1.key 2048
# openssl req -new -key $1.key -out $1.csr -config cert.conf
# openssl x509 -req -in $1.csr -CA rootCA.crt -CAkey rootCA.key -CAcreateserial \
#     -out $1.crt -days 500 -sha256

# openssl genrsa -out $1.key 2048
# openssl req -new -sha256 -key $1.key \
#     -subj "/C=CA/ST=QC/O=Polymtl/OU=Projet 3 info/CN=$2" -reqexts SAN \
#     -config <(cat /etc/ssl/openssl.cnf <(printf "\n[SAN]\nsubjectAltName=DNS:$2")) \
#     -out $1.csr
# openssl x509 -req -extfile <(printf "subjectAltName=DNS:$2") \
#     -days 120 -in $1.csr -CA rootCA.crt -CAkey rootCA.key -CAcreateserial \
#     -out $1.crt -sha256

openssl req -new -sha256 -nodes -out $1.csr -newkey rsa:2048 -keyout $1.key \
    -config <( cat cert.conf )
openssl x509 -req -in $1.csr -CA rootCA.crt -CAkey rootCA.key -CAcreateserial \
    -out $1.crt -days 500 -sha256 -extfile cert.ext

