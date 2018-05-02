install microhttpd
------------------
 - ./configure
 - make
 - make install
 
 in cygwin: rename the /usr/local/lib/libmicrohttpd.dll.a to libmicrohttpd.dll

 
install teng
------------
 - ./configure
 - change the following line in the makefile: 
	CXXFLAGS =
	to:
	CXXFLAGS = -D_DEFAULT_SOURCE
 - make
 - make install
 
 
google test
-----------
 - execute the googlemock/scripts/fuse_gmock_files.py 
 
 all of the neccessary include files and source files are copied to the following 3 files, add these files to the project and compile it:
  - gmock/gmock.h
  - gtest/gtest.h
  - gmock-gtest-all.cc
 
