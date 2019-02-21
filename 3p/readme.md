microhttpd
----------
 - ./configure --disable-shared
 - make
 - make install
 
 
teng
----
 - ./configure
 - change the following line in the makefile: 
	CXXFLAGS =
	to:
	CXXFLAGS = -D_DEFAULT_SOURCE
 - make
 - make install
 
 
pugixml
-------
  - copy the pugiconfig.hpp, pugixml.cpp and pugixml.hpp to the workspace 
    and build it with the project
 

 
ONLY FOR TESTING
----------------
 
google test
-----------
 - execute the googlemock/scripts/fuse_gmock_files.py 
 
 all of the neccessary include files and source files are copied to the following 3 files, add these files to the project and compile it:
  - gmock/gmock.h
  - gtest/gtest.h
  - gmock-gtest-all.cc
 

curl
-----
 - ./configure --disable-shared --disable-verbose --disable-manual --disable-ipv6 --disable-ftp --disable-file --disable-ldap --disable-ldaps --disable-rtsp --disable-proxy --disable-dict --disable-telnet --disable-tftp --disable-pop3 --disable-imap --disable-smb --disable-smtp --disable-gopher --without-ssl --without-zlib
 - make
 - make install
