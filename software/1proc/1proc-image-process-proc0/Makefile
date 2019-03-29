.PHONY: update-bsp download

PROJECT_NAME:=1proc-image-process-proc0

update-bsp:
	nios2-bsp-update-settings settings.bsp

download:
	nios2-download ./${PROJECT_NAME}_app/${PROJECT_NAME}.elf && nios2-terminal
