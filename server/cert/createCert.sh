#!/bin/bash
#
# Usage: ./createCert.sh [prefix] [ips]

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
$2
"
# If you get a bad handshake problem, you are probably missing your IP in the list

ROOT_CA_CRT=rootCA.crt
ROOT_CA_KEY=rootCA.key

if [[ ! -f "$ROOT_CA_CRT" ]]; then
    ROOT_CA_CRT="/usr/share/inf3995/$ROOT_CA_CRT"
fi

if [[ ! -f "$ROOT_CA_KEY" ]]; then
    ROOT_CA_KEY="/usr/share/inf3995/$ROOT_CA_KEY"
fi

openssl req -new -sha256 -nodes -out "$1.csr" -newkey rsa:2048 -keyout "$1.key" \
    -config <(printf "%s" "$conf")

if [[ -z ${ROOT_CA_PASS+x} ]]; then
    with_pass=()
else
    with_pass=("-passin" "pass:$ROOT_CA_PASS")
fi

openssl x509 -req -in "$1.csr" -CA "$ROOT_CA_CRT" -CAkey "$ROOT_CA_KEY" -CAcreateserial \
    -out "$1.crt" -days 500 -sha256 "${with_pass[@]}" -extfile <(printf "%s" "$ext")
