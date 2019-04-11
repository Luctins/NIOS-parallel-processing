#!/bin/bash

##
# @file: create-NIOS-project.sh
# @author: Lucas Martins Mendes
# @date: qua mar 27 11:36:14 -03 2019
# @brief: Script for autocreating NIOS projects
##

DEFAULT_PATH="/home/luctins/repo/NIOS-parallel-processing/fpga/"
DEFAULT_PROJ_DIR="/home/luctins/repo/NIOS-parallel-processing/software"
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
    cd $DEFAULT_PATH
    RESULT=""
    while [ ! -f "${PWD}/${RESULT}" ]
    do
        DIR=$(ls -a)
        i=0
        unset files
        for f in $DIR ; do
            if [ $f = "." ]
            then
                continue
            fi
            files[i]="$(( i / 2 ))"
            files[i+1]="$f"
            let "i= $i + 2"
        done
        echo "files: ${files[@]}"
        eval `resize`
        RSL=$(whiptail --title "file selector" --menu "Please select the $FNAME" $LINES $COLUMNS $(( $LINES - 8 )) "${files[@]}" 3>&1 1>&2 2>&3)

        if [ $? != 0 ]
        then
            break
        fi

        let "RSL= $RSL + $RSL"
        let "RSL= $RSL + 1"

        echo "RSL: $RSL"

        RESULT="${files[${RSL}]}"

        echo "result: $RESULT"

        if [ -f $RESULT ] && [ $RESULT != ".." ]; then
            break
            echo "found file : ${PWD}/${RESULT}"
        else
            cd $RESULT
        fi
    done
    RESULT="${PWD}/${RESULT}"
    cd $_PWD;

}

#set up nios terminal
#nios_bash

FNAME="SOPC location"
pick_file
SOPC_LOC=$RESULT

echo -n "please insert the project name: "
read project_name

mkdir "${DEFAULT_PROJ_DIR}/${project_name}"
mkdir "${DEFAULT_PROJ_DIR}/${project_name}/app"
mkdir "${DEFAULT_PROJ_DIR}/${project_name}/bsp"
cp template/Makefile "${DEFAULT_PROJ_DIR}/${project_name}/"

cd "$DEFAULT_PROJ_DIR/${project_name}"
sed -i -e "s/^PROJ_NAME :=/PROJ_NAME :=${project_name}/g" Makefile

#create NIOS nios Project
nios2-swexample-create --name="$project_name" --type=blank_project  --sopc-file="$SOPC_LOC" --bsp-dir=./bsp/ --app-dir=./app/

cd bsp
./create-this-bsp
cd ../app
echo "void main(void) { } " >> main.c
./create-this-app
sed -i -e "s/^C_SRCS :=/C_SRCS :=main.c/g" Makefile
