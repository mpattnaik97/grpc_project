Hi, welcome to my small project on grpc c++


This is a grpc server/client project which behaves as follows:

 - It can send 3 types of data, i.e. integer, string and file, to the server and get response from the server.
 - For an integer request, the server returns the square of the integer sent.
 - For a string request, server returns the reverse of the string.
 - For a file, currently server returns a string response. 
   The file content is converted to bytes array and is streamed in fractions to the server by the client.
   The server from the streams creates a file and stores it in the path "emotech/cmake/build/clientfiles/".

 (This could be further modified to make the server send a compressed file, which I plan on working in future on this project)

[
  Note:
  
  Software versions used in this project are as follows:
  1. cmake - 3.20
  2. make - 4.1
  3. grpc - 1.37.1
]

Instructions for use (these are instructions for linux). 
1. Download and extract the zip file.
2. From the terminal, cd into the the project directory. [($ cd emotech) in most cases]
3. Make sure that the tree structure of the extracted zip file is same as below. (You can check it using the tree command)

├── client.cc
├── cmake
│   └── build
│       ├── client
│       ├── clientfiles (make sure this directory is there, otherwise files would not get written on the server side)
│       ├── CMakeCache.txt
│       ├── CMakeFiles
│       │   ├── 3.20.3
│       │   │   ├── CMakeCCompiler.cmake
│       │   │   ├── CMakeCXXCompiler.cmake
│       │   │   ├── CMakeDetermineCompilerABI_C.bin
│       │   │   ├── CMakeDetermineCompilerABI_CXX.bin
│       │   │   ├── CMakeSystem.cmake
│       │   │   ├── CompilerIdC
│       │   │   │   ├── a.out
│       │   │   │   ├── CMakeCCompilerId.c
│       │   │   │   └── tmp
│       │   │   └── CompilerIdCXX
│       │   │       ├── a.out
│       │   │       ├── CMakeCXXCompilerId.cpp
│       │   │       └── tmp
│       │   ├── client.dir
│       │   │   ├── build.make
│       │   │   ├── client.cc.o
│       │   │   ├── client.cc.o.d
│       │   │   ├── cmake_clean.cmake
│       │   │   ├── compiler_depend.internal
│       │   │   ├── compiler_depend.make
│       │   │   ├── compiler_depend.ts
│       │   │   ├── DependInfo.cmake
│       │   │   ├── depend.make
│       │   │   ├── emotech.grpc.pb.cc.o
│       │   │   ├── emotech.grpc.pb.cc.o.d
│       │   │   ├── emotech.pb.cc.o
│       │   │   ├── emotech.pb.cc.o.d
│       │   │   ├── flags.make
│       │   │   ├── link.txt
│       │   │   └── progress.make
│       │   ├── cmake.check_cache
│       │   ├── CMakeDirectoryInformation.cmake
│       │   ├── CMakeError.log
│       │   ├── CMakeOutput.log
│       │   ├── CMakeRuleHashes.txt
│       │   ├── CMakeTmp
│       │   ├── Makefile2
│       │   ├── Makefile.cmake
│       │   ├── progress.marks
│       │   ├── server.dir
│       │   │   ├── build.make
│       │   │   ├── cmake_clean.cmake
│       │   │   ├── compiler_depend.internal
│       │   │   ├── compiler_depend.make
│       │   │   ├── compiler_depend.ts
│       │   │   ├── DependInfo.cmake
│       │   │   ├── depend.make
│       │   │   ├── emotech.grpc.pb.cc.o
│       │   │   ├── emotech.grpc.pb.cc.o.d
│       │   │   ├── emotech.pb.cc.o
│       │   │   ├── emotech.pb.cc.o.d
│       │   │   ├── flags.make
│       │   │   ├── link.txt
│       │   │   ├── progress.make
│       │   │   ├── server.cc.o
│       │   │   └── server.cc.o.d
│       │   └── TargetDirectories.txt
│       ├── cmake_install.cmake
│       ├── emotech.grpc.pb.cc
│       ├── emotech.grpc.pb.h
│       ├── emotech_image.webp
│       ├── emotech.pb.cc
│       ├── emotech.pb.h
│       ├── Makefile
│       ├── server
│       └── test_image.png
├── CMakeLists.txt
├── emotech.proto
├── readme.txt
└── server.cc

4. If its the same, you are good to go.
5. cd into the extracted zip directory (emotech)
6. Write the following commands:

    i. cd cmake/build
    ii. ./server

        You should be able to see an output which says this: 
        Server Listening on port port: 0.0.0.0:5000

    iii. ctrl + shift + t (this will open a new terminal in which you will run the client)
    iv. I have made the client so that it can take data arguments in command line itself. 
        Explanation on running client is as follows:

        Syntax for running client -

        (1) ./client <int value> <string value> <string path>

        Here the order is important. Although, any of the value can be skipped but the order is to be maintained.

        for eg. If I want to send only a string value, I will run -
                ./client "apples are great"

                [
                  
                  Note: the string value should be in double quotes, else the spaces will make them be read as different arguments.
                  for example:
                  
                  ./client apples are great 
                  
                  This command will take this string as 3 different arguments and throw a bad syntax error

                 ]
        
        A few ways to run the client are described as follows.
        
        
        Different ways to run client.

        (1) ./client

            This will run the client without providing any parameters. 
            The client has default values for int, string and file which will be used in this case.
            
            The output should be something like this:

            Server -

            Server Listening on port port: 0.0.0.0:5000
            Server: Received number -> 10
            Server: Sending the square of the number back i.e. -> 100
            Server: Received string -> perseverance is key
            Server: Sending it back to server -> perseverance is key
            Server: Received file. Initializing IO process...
            Server: Setting path to clientfiles/test_image.png
            Server: Creating file ...
            Server: Read complete, closing the file


            Client - 

            Client: No params provided, sending request with test params
            Client: Sending number -> 10
            Client: Received number from server -> 100
            Client: Sending string -> perseverance is key
            Client: Received string from server -> perseverance is key
            Client: Sending file path ->test_image.png
            Client: Streaming file content in bytes
            Client: File contents sent, closing stream
            Client: Response from the server -> File Written Successfully

        Similarly you can test out various other combinations for sending requests -
            
        (2) ./client 10

            This sends an integer request to the server.

        (3) ./client "emotech_image.webp"

            This sends a file request to the server. The file in this path would be streamed.
        
        (4) ./client 10 "apples are good"

            This send an integer and string request.

        (5) ./client 10 "emotech_image.webp"

            This sends an integer and file path request.

        (6) ./client "apples are good" "emotech_image.webp"
            
            This sends a string and file path request.

        (7) ./client 10 "apples are good" "emotech_image.webp

            This will send integer, string and file request to the server.

[

  Note:
  
  If this doesn't work and you see any protobuf, cmake or grpc you might have to setup grpc c++ using cmake in your system.
  You can follow the link https://grpc.io/docs/languages/cpp/quickstart/ and setup the grpc c++ on you system.

  once this is done try doing step 6 again.

  If still doesn't work, do this:
  
  (a) rm -r cmake
  (b) mkdir -p cmake/build

      Before doing the next step, make sure your cmake version is 3.19 or above.
      If not, upgrade to newer version using the steps given in the link https://grpc.io/docs/languages/cpp/quickstart/
  
  (c) cmake -DCMAKE_PREFEIX_PATH=$MY_INSTALL_DIR ../..

      (If you had followed the setting up of grpc c++ from this link https://grpc.io/docs/languages/cpp/quickstart/, 
      $MY_INSTALL_DIR should have the path $HOME/.local)

  (d) make

      This will create all the autogenerated files for headers and implementation of grpc methods and messages.
      It will also create executable server and client files which can be run by using ./server and ./client commands.

  (e) mkdir clientfiles (this step is important as the files would be written by server in this directory)

  (f) Try doing the step number 6 from the instructions above, it should work now.

]
