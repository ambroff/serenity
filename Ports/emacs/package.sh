#!/usr/bin/env -S bash -x ../.port_include.sh

port=emacs
version=29.3
files=(
    "https://mirrors.tripadvisor.com/gnu/emacs/emacs-${version}.tar.gz#2de8df5cab8ac697c69a1c46690772b0cf58fe7529f1d1999582c67d927d22e4"
)

useconfigure='true'
configopts=(
    "--without-x"
    "--without-gnutls"
    "--host=${SERENITY_ARCH}-pc-serenity"
)

pre_configure() {
    run ./autogen.sh
}

launcher_name='GNU Emacs'
launcher_category='&Editors'
launcher_command='/usr/local/bin/emacs'
