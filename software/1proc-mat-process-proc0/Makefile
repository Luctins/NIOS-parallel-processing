################################################
# @author: Lucas Martins Mendes
# @date: 04/10/19
# @brief: top level makefile for nios Projects
################################################

NIOS_TARGETS :=  all bsp libs flash mem_init_generate help
NIOS_TARGETS += clean_all clean clean_bsp clean_libs download-elf program-flash

#############
# Variables
############

PROJ_NAME :=1proc-mat-process-proc0

CUSTOM_TARGETS := update-bsp config-bsp
BSP_SETINGS := "${CURDIR}/$(PROJ_NAME)_bsp/settings.bsp"

##########
# Include
##########

#############
# Targets
#############

.PHONY: $(CUSTOM_TARGETS)

edit-bsp:
	nios2-bsp-editor --settings=$(BSP_SETTINGS)

update-bsp:
	nios2-bsp-update-settings --settings=$(BSP_SETTINGS)

${NIOS_TARGETS} :
	make -C $(PROJ_NAME)_app/ $@
