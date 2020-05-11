export DanOS_DIR=`pwd`
export PATH="$DanOS_DIR/3rdParty/bin:$PATH"
export PATH="$DanOS_DIR/3rdParty/cmake-3.3.1-Linux-x86_64/bin:$PATH"
export PATH="$DanOS_DIR/3rdParty/protoc-3.3.0-linux-x86_64/bin:$PATH"
export PATH="/opt/freescale/usr/local/gcc-4.1.78-eglibc-2.5.78-1/powerpc-e300c3-linux-gnu/bin:$PATH"

alias mkw="mk -w"
alias sdk="cd ${DanOS_DIR}/3rdParty/pm5440_sdk_v3_12/applib"
alias hal="cd ${DanOS_DIR}/HAL"
alias dbf="cd ${DanOS_DIR}/DBF"
alias sfm="cd ${DanOS_DIR}/SFM"
alias eqm="cd ${DanOS_DIR}/EQM"
alias mxp="cd ${DanOS_DIR}/../"
alias gen="cd ${DanOS_DIR}/xml/; ./CodeGen.py; cd -"
alias run="cd ${DanOS_DIR}; ./build_x86/bin/APP"

