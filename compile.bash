#!/bin/bash

case "$OSTYPE" in
  darwin*)  g++ -std=c++11 -o MC MC.cpp Core/*.cpp Math/*.cpp -I. -framework OpenGL -framework GLUT ;; 
  *)        g++ -std=c++11 -o MC MC.cpp Core/*.cpp Math/*.cpp -I. -lglut -lGL -lGLU ;;
esac

