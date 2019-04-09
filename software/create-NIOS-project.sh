#!/bin/bash

##
# @file: create-NIOS-project.sh
# @author: Lucas Martins Mendes
# @date: qua mar 27 11:36:14 -03 2019
# @brief: Script for autocreating NIOS projects
##

DEFAULT_PATH="/home/luctins/repo/NIOS-parallel-processing/fpga/"
SOPC_LOCATION=""

trap _exit INT

function _exit()
{
    echo "exiting"
    #rm -rf $PROJ_PATH
    #later do some cleanup maybe
}

function pick_file()
{
    _PWD=$PWD
    files=$(ls)
    i=0
    for f in files
    do
        files[i]="$i"
        files[i+1]="$f"
        ((i+=2))
    done
    whiptail --title "please select the SOPC location" --menu "Please select the desired file"

}

while [ 1 = 1 ]
do
    FILE=$(whiptail --title 'please select the SOPC location'  --menu "please select the desired file" 14 40 20 "${FILES[@]})

#set up nios terminal
#nios_bash

echo 'please select the SOPC location (relative/absolute)'

while [ 1 = 1 ]
do
    FILES=$(ls $DEFAULT_PATH)
    #SOPC_LOCATION=$(dialog --stdout --title 'please select the SOPC location' --fselect "$DEFAULT_PATH" 20 70)
    SOPC_LOCATION=$(whiptail --title 'please select the SOPC location'  --menu "please select the desired file" 14 40 20 "${FILES[@]}" )
    if [ -f $SOPC_LOCATION ]
    then
        #break if reached a file
        break
    fi
    if [[ $? == 255 ]]
    then
        exit
    fi


done

clear

PROJECT_NAME="default"


while [[ "${PROJECT_NAME}" !=~ '[0-9a-zA-Z\-_]' ]]
do
    echo -n "please insert the project name:"
    read PROJECT_NAME
    if [[  $PROJECT_NAME !=~ [0-9a-zA-Z\-_] ]]
    then
        echo "the project name must only contain alphanumeric characters, '-' and '_' (no whitespace)"
    else
        break
done
