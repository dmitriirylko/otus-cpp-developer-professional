#!/bin/sh
BINARY_HOME=./bin
INPUT_HOME=./input
INPUT=${INPUT_HOME}/AB_NYC_2019.csv

cat ${INPUT} | ${BINARY_HOME}/mapper_var "$(cat output_mean)" | ${BINARY_HOME}/reducer_var > output_var
cat output_var