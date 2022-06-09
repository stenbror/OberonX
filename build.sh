#!/bin/bash

echo "Building the Gnu G++ version"
 g++ -o obx main.cc Tokenizer.cc Parser.cc ASTNode.cc
 strip obx
 
 echo "Building the clang++ version"
 clang++ -o obx_clang main.cc Tokenizer.cc Parser.cc ASTNode.cc
 strip obx_clang

 ls -la obx*
