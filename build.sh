#!/bin/bash

 g++ -o obx main.cc Tokenizer.cc Parser.cc ASTNode.cc
 strip obx
 