#!/bin/bash
#################################################
# @file: create-NIOS-project.sh
# @author: Lucas Martins Mendes
# @date: qua mar 27 11:36:14 -03 2019
# @brief: Script for autocreating NIOS projects
##################################################

DEFAULT_PATH="/home/luctins/repo/NIOS-parallel-processing/fpga/"
DEFAULT_PROJ_DIR="/home/luctins/repo/NIOS-parallel-processing/software"
SOPC_LOCATION=""

trap _exit INT

function _exit()
{
    echo "exiting"
    exit
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
        #echo "files: ${files[@]}"
        eval `resize`
        RSL=$(whiptail --title "file selector" --menu "Please select the $FNAME" $LINES $COLUMNS $(( $LINES - 8 )) "${files[@]}" 3>&1 1>&2 2>&3)

        if [ $? != 0 ]
        then
            break
        fi

        let "RSL= $RSL + $RSL"
        let "RSL= $RSL + 1"

        #echo "RSL: $RSL"

        RESULT="${files[${RSL}]}"

        #echo "result: $RESULT"

        if [ -f $RESULT ] && [ $RESULT != ".." ]; then
            echo "found file : ${PWD}/${RESULT}"
            break
        else
            cd $RESULT
        fi
    done
    RESULT="${PWD}/${RESULT}"
    cd $_PWD;

}

#set up nios terminal
#nios_bash

#pick SOPC
FNAME="SOPC location"
pick_file
SOPC_LOC=$RESULT

#read project name
echo -n "please insert the project name: "
read project_name

#create project folders
mkdir "${project_name}"
mkdir "${project_name}/app"
mkdir "${project_name}/bsp"

#copy default Makefile
cp -v template/Makefile "${project_name}/"
#copy main and dir-locals
cp -v template/main.c "${project_name}/app/"

cp -v template/dir-locals.el "${project_name}/app/.dir-locals.el"

cd "${DEFAULT_PROJ_DIR}/${project_name}"

#insert project name in the makefile
echo "inserted proj name into Makefile"
sed -i -e "s/^PROJ_NAME :=/PROJ_NAME :=${project_name}/g" Makefile

#create NIOS Project
nios2-swexample-create --name="$project_name" --type=blank_project  --sopc-file="$SOPC_LOC" --bsp-dir="./bsp/" --app-dir="./app/" --cpu-name=

#make BSP and app
cd bsp
echo "run create-this-bsp"
./create-this-bsp
cd ../app
echo "run create-this-app"
./create-this-app

#insert main as a target into the makefile
echo "edit app/Makefile to add target"
sed  -i -e "s/^C_SRCS :=/C_SRCS :=main.c/g" Makefile
make -s
#insert include folders
echo "edit app/Makefile to add include dir"
sed -i -e "s/^ALT_INCLUDE_DIRS :=/ALT_INCLUDE_DIRS := \/home\/luctins\/repo\/NIOS-parallel-processing\/software\/common/g " Makefile
echo "done"

#cd ../../
#echo "((c-mode . (( flycheck-clang-include-path . (( \"$(realpath common/)\" )  (\" $(realpath ${project_name}/bsp/drivers/inc )\")  (  \" $(realpath ${project_name}/bsp/) \") (\"$(realpath ${project_name}/bsp/HAL/inc)\"))))))" > ${project_name}/app/.dir-locals.el
#cat ${project_name}/app/.dir-locals.el
