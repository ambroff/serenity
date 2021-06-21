#!/usr/bin/env -S bash ../.port_include.sh

port="iperf"
version="3.10.1"
useconfigure="true"
files="https://downloads.es.net/pub/iperf/iperf-${version}.tar.gz iperf-${version}.tar.gz  03bc9760cc54a245191d46bfc8edaf8a4750f0e87abca6764486972044d6715a"
auth_type="sha256"

export CFLAGS="-D_BSD_SOURCE"
export LDFLAGS="-lcore -lcrypt"
