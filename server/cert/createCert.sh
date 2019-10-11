#!/bin/bash
#
# Usage: ./createCert.sh [prefix] [ip]

set -o xtrace

# Password is equipe01
# Inspired from: https://gist.github.com/fntlnz/cf14feb5a46b2eda428e000157447309

conf=\
"
[req]
default_bits = 2048
prompt = no
default_md = sha256
distinguished_name = dn

[dn]
C=CA
ST=QC
L=Montreal
O=Polymtl
OU=Projet 3 info
emailAddress=francis.granger@polymtl.ca
CN = $2
"

ext=\
"
authorityKeyIdentifier=keyid,issuer
basicConstraints=CA:FALSE
keyUsage = digitalSignature, nonRepudiation, keyEncipherment, dataEncipherment
subjectAltName = @alt_names

[alt_names]
IP.1 = $2
DNS.1 = localhost
"

openssl req -new -sha256 -nodes -out $1.csr -newkey rsa:2048 -keyout $1.key \
    -config <(printf "$conf")
openssl x509 -req -in $1.csr -CA rootCA.crt -CAkey rootCA.key -CAcreateserial \
    -out $1.crt -days 500 -sha256 -extfile <(printf "$ext")

