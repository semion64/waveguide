#!/bin/bash

g++ main.cpp transmission_matrix.cpp waveguide.cpp materials.cpp visual.cpp stat_analize.cpp agilent_spectr.cpp entities.cpp -lboost_iostreams -lboost_system -lboost_filesystem -g 
# -fsanitize=address
