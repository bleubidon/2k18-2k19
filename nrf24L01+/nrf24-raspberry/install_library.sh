#!/bin/bash
sudo apt-get update
cd
git clone https://github.com/TMRh20/RF24.git
cd RF24
make
sudo make install
