#!/bin/bash
#
# Usage: ./createCert.sh [prefix]

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
CN = Ip addresses running on FPGA
"

ext=\
"
authorityKeyIdentifier=keyid,issuer
basicConstraints=CA:FALSE
keyUsage = digitalSignature, nonRepudiation, keyEncipherment, dataEncipherment
subjectAltName = @alt_names

[alt_names]
DNS.1 = localhost
IP.1 = 192.168.0.160
IP.2 = 192.168.1.19
IP.3 = 10.200.7.63
"
# If you get a bad handshake problem, you are probably missing your IP in the list

openssl req -new -sha256 -nodes -out $1.csr -newkey rsa:2048 -keyout $1.key \
    -config <(printf "$conf")
openssl x509 -req -in $1.csr -CA rootCA.crt -CAkey rootCA.key -CAcreateserial \
    -out $1.crt -days 500 -sha256 -extfile <(printf "$ext")
