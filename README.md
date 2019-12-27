# Mapper
This is going to be used to find the distance of specific known devices from a list (known.txt):

Requires aircrack suite, and the following file directory:

someFolder/{
  code/{
    mapper.cpp, mapper.h
  }
  resc/{
    known.txt //look below for format
    stored/{
      //program will put temporary csv files here from aircrack while running. this directory must exist however.
    }
  }
}






Format for known.txt:
0n= Name of device
0m= 00:00:00:00:00:00 // Mac address of device

1n= ...
1m= ...
...
