#!/bin/bash
cp libocl.h /usr/include
cp libocl.so /usr/lib
chmod 0755 /usr/lib/libocl.so
/sbin/ldconfig 
/sbin/ldconfig -p | grep ocl
unset LD_LIBRARY_PATH

exit 
