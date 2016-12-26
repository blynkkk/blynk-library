#!/bin/bash

if [ -z "$ARDUINO_IDE_PATH" ]; then
  echo "ARDUINO_IDE_PATH not defined"
  exit 1
fi

if [ ! -d "$ARDUINO_IDE_PATH" ]; then
  echo "$ARDUINO_IDE_PATH does not exist"
  exit 1
fi

IDE_LIB_PATH=$ARDUINO_IDE_PATH/lib

TGT_DIR=./$1/tool/
TMP_DIR=./build_$1/
rm -rf $TGT_DIR $TMP_DIR
mkdir -p $TGT_DIR $TMP_DIR
javac -target 1.8 -cp "$IDE_LIB_PATH/pde.jar:$IDE_LIB_PATH/arduino-core.jar" -d $TMP_DIR $1.java && jar cvf $TGT_DIR/$1.jar -C $TMP_DIR .
rm -rf $TMP_DIR
