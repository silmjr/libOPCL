#!/bin/bash
cd library
make 
cp libocl.so /usr/lib
chmod 0755 /usr/lib/libocl.so
ldconfig 
/sbin/ldconfig -p | grep ocl
unset LD_LIBRARY_PATH

exit 