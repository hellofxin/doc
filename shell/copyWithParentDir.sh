#!/bin/bash

mkdir ../copy

cfgTargetFilePattern="*.[hc]"
cfgWworkingRoot="."

find $cfgWworkingRoot -type f -name ${cfgTargetFilePattern} -exec cp --parents {} ../copy \;

