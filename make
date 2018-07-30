#!/usr/bin/env bash

BUILD_DIR=${BUILD_DIR:-build}
BUILD_ARCH=${1:-ar71xx}
BUILD_TARGET=${2:-all}

test -d ${BUILD_DIR} || mkdir ${BUILD_DIR}

EXIT_STATUS=0
cd ${BUILD_DIR}
    # Cleanup when switching arch
    grep -iq ${BUILD_ARCH} ./toolchain.cmake || \
        rm -rf ./*
    #Build
    cmake .. -D${BUILD_ARCH}=true && \
        make ${BUILD_TARGET}
        
    EXIT_STATUS=$?
cd - >/dev/null

exit ${EXIT_STATUS}