#!/bin/bash
cp libopcl.h /usr/include
cp libopcl.so /usr/lib
chmod 0755 /usr/lib/libopcl.so
/sbin/ldconfig 
/sbin/ldconfig -p | grep opcl


exit 
