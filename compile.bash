#!/bin/bash

g++ -std=c++11 -o MC MC.cpp Core/*.cpp Math/*.cpp -I. -lglut -lGL -lGLU
