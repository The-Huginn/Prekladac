# Prekladac

This repo contains solution for school project (see /docs/IFJ_zadanie.pdf)

Authors: xpojez00, xbudin05, xhribi00, xchupa03

# Installation guide:
#   Linux:
for cloning repo:

    $ git clone https://github.com/The-Huginn/Prekladac.git
        
to generate Makefile:
        
    $ cmake .

run Makefile:
    
    $ make

more about targets and arguments for executing to come...

#   Windows:
Open Visual Studio and clone The-Huginn/Prekladac repository

Download Google Tests zip file, extract in ../The-Huginn/Prekladac/

open command prompt and navigate to ../The-Huginn/Prekladac/googletest-master

to generate VS enviroment for Google Tests:
    
    $ cmake .

open googletest-distribution.sln and compile for required configuration (ideally both)

open command prompt and navigate to ../The-Huginn/Prekladac/build (if build is missing create it)

to generate VS enviroment for Prekladac:
    
    $ cmake ..

open Prekladac.sln
