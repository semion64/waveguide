#!/bin/bash

g++ main.cpp transmission_matrix.cpp waveguide.cpp materials.cpp visual.cpp stat_analize.cpp agilent_spectr.cpp entities.cpp -std=c++17 -lboost_iostreams -lboost_system -lboost_filesystem -g 

# g++  -IC:\boost\boost_1_84_0 -LC:\boost\boost_1_84_0\stage\lib main.cpp transmission_matrix.cpp waveguide.cpp materials.cpp visual.cpp stat_analize.cpp agilent_spectr.cpp entities.cpp -std=c++17 -lboost_iostreams-mgw11-mt-x64-1_84 -lboost_system-mgw11-mt-x64-1_84 -lboost_filesystem-mgw11-mt-x64-1_84 
# -fsanitize=address
