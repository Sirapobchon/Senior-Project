#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/RTOS_NoUpload.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/RTOS_NoUpload.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=Task/blink.c Task/numpad.c Task/pwm.c main.c FreeRTOS/croutine.c FreeRTOS/event_groups.c FreeRTOS/list.c FreeRTOS/queue.c FreeRTOS/tasks.c FreeRTOS/timers.c FreeRTOS/portable/GCC/ATMega328/port.c FreeRTOS/portable/MemMang/heap_4.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/Task/blink.o ${OBJECTDIR}/Task/numpad.o ${OBJECTDIR}/Task/pwm.o ${OBJECTDIR}/main.o ${OBJECTDIR}/FreeRTOS/croutine.o ${OBJECTDIR}/FreeRTOS/event_groups.o ${OBJECTDIR}/FreeRTOS/list.o ${OBJECTDIR}/FreeRTOS/queue.o ${OBJECTDIR}/FreeRTOS/tasks.o ${OBJECTDIR}/FreeRTOS/timers.o ${OBJECTDIR}/FreeRTOS/portable/GCC/ATMega328/port.o ${OBJECTDIR}/FreeRTOS/portable/MemMang/heap_4.o
POSSIBLE_DEPFILES=${OBJECTDIR}/Task/blink.o.d ${OBJECTDIR}/Task/numpad.o.d ${OBJECTDIR}/Task/pwm.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/FreeRTOS/croutine.o.d ${OBJECTDIR}/FreeRTOS/event_groups.o.d ${OBJECTDIR}/FreeRTOS/list.o.d ${OBJECTDIR}/FreeRTOS/queue.o.d ${OBJECTDIR}/FreeRTOS/tasks.o.d ${OBJECTDIR}/FreeRTOS/timers.o.d ${OBJECTDIR}/FreeRTOS/portable/GCC/ATMega328/port.o.d ${OBJECTDIR}/FreeRTOS/portable/MemMang/heap_4.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/Task/blink.o ${OBJECTDIR}/Task/numpad.o ${OBJECTDIR}/Task/pwm.o ${OBJECTDIR}/main.o ${OBJECTDIR}/FreeRTOS/croutine.o ${OBJECTDIR}/FreeRTOS/event_groups.o ${OBJECTDIR}/FreeRTOS/list.o ${OBJECTDIR}/FreeRTOS/queue.o ${OBJECTDIR}/FreeRTOS/tasks.o ${OBJECTDIR}/FreeRTOS/timers.o ${OBJECTDIR}/FreeRTOS/portable/GCC/ATMega328/port.o ${OBJECTDIR}/FreeRTOS/portable/MemMang/heap_4.o

# Source Files
SOURCEFILES=Task/blink.c Task/numpad.c Task/pwm.c main.c FreeRTOS/croutine.c FreeRTOS/event_groups.c FreeRTOS/list.c FreeRTOS/queue.c FreeRTOS/tasks.c FreeRTOS/timers.c FreeRTOS/portable/GCC/ATMega328/port.c FreeRTOS/portable/MemMang/heap_4.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

# The following macros may be used in the pre and post step lines
_/_=\\
ShExtension=.bat
Device=ATmega328P
ProjectDir="C:\Users\Sirapob-ASUSTUF\MPLABXProjects\RTOS_NoUpload.X"
ProjectName=RTOS_NoUpload
ConfName=default
ImagePath="dist\default\${IMAGE_TYPE}\RTOS_NoUpload.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}"
ImageDir="dist\default\${IMAGE_TYPE}"
ImageName="RTOS_NoUpload.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}"
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IsDebug="true"
else
IsDebug="false"
endif

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/RTOS_NoUpload.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
	@echo "--------------------------------------"
	@echo "User defined post-build step: [call post_build_noup.bat ]"
	@call post_build_noup.bat 
	@echo "--------------------------------------"

MP_PROCESSOR_OPTION=ATmega328P
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/Task/blink.o: Task/blink.c  .generated_files/flags/default/7a63fa4a6a53048f58b84bbeb660a292877f827c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Task" 
	@${RM} ${OBJECTDIR}/Task/blink.o.d 
	@${RM} ${OBJECTDIR}/Task/blink.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./FreeRTOS -I./FreeRTOS/include -I./FreeRTOS/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/Task/blink.o.d" -MT "${OBJECTDIR}/Task/blink.o.d" -MT ${OBJECTDIR}/Task/blink.o -o ${OBJECTDIR}/Task/blink.o Task/blink.c 
	
${OBJECTDIR}/Task/numpad.o: Task/numpad.c  .generated_files/flags/default/6b579b3d09ffd2aecc2bb3d9b7936d61bc5b28a6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Task" 
	@${RM} ${OBJECTDIR}/Task/numpad.o.d 
	@${RM} ${OBJECTDIR}/Task/numpad.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./FreeRTOS -I./FreeRTOS/include -I./FreeRTOS/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/Task/numpad.o.d" -MT "${OBJECTDIR}/Task/numpad.o.d" -MT ${OBJECTDIR}/Task/numpad.o -o ${OBJECTDIR}/Task/numpad.o Task/numpad.c 
	
${OBJECTDIR}/Task/pwm.o: Task/pwm.c  .generated_files/flags/default/502b064584d1585f16ebd4acd3197a0f45a1310 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Task" 
	@${RM} ${OBJECTDIR}/Task/pwm.o.d 
	@${RM} ${OBJECTDIR}/Task/pwm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./FreeRTOS -I./FreeRTOS/include -I./FreeRTOS/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/Task/pwm.o.d" -MT "${OBJECTDIR}/Task/pwm.o.d" -MT ${OBJECTDIR}/Task/pwm.o -o ${OBJECTDIR}/Task/pwm.o Task/pwm.c 
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/1bbbcc39009d73503e5318d6c61d54990c255c0d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./FreeRTOS -I./FreeRTOS/include -I./FreeRTOS/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/main.o.d" -MT "${OBJECTDIR}/main.o.d" -MT ${OBJECTDIR}/main.o -o ${OBJECTDIR}/main.o main.c 
	
${OBJECTDIR}/FreeRTOS/croutine.o: FreeRTOS/croutine.c  .generated_files/flags/default/d9cfaaeb7ae406bb64223107272f6247da8a23ca .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS" 
	@${RM} ${OBJECTDIR}/FreeRTOS/croutine.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/croutine.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./FreeRTOS -I./FreeRTOS/include -I./FreeRTOS/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/FreeRTOS/croutine.o.d" -MT "${OBJECTDIR}/FreeRTOS/croutine.o.d" -MT ${OBJECTDIR}/FreeRTOS/croutine.o -o ${OBJECTDIR}/FreeRTOS/croutine.o FreeRTOS/croutine.c 
	
${OBJECTDIR}/FreeRTOS/event_groups.o: FreeRTOS/event_groups.c  .generated_files/flags/default/d46a5798ea592f48642e7b8f37d4d891433816fc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS" 
	@${RM} ${OBJECTDIR}/FreeRTOS/event_groups.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/event_groups.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./FreeRTOS -I./FreeRTOS/include -I./FreeRTOS/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/FreeRTOS/event_groups.o.d" -MT "${OBJECTDIR}/FreeRTOS/event_groups.o.d" -MT ${OBJECTDIR}/FreeRTOS/event_groups.o -o ${OBJECTDIR}/FreeRTOS/event_groups.o FreeRTOS/event_groups.c 
	
${OBJECTDIR}/FreeRTOS/list.o: FreeRTOS/list.c  .generated_files/flags/default/57076bd63ccb293800fa922e921acf5b6264a022 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS" 
	@${RM} ${OBJECTDIR}/FreeRTOS/list.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/list.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./FreeRTOS -I./FreeRTOS/include -I./FreeRTOS/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/FreeRTOS/list.o.d" -MT "${OBJECTDIR}/FreeRTOS/list.o.d" -MT ${OBJECTDIR}/FreeRTOS/list.o -o ${OBJECTDIR}/FreeRTOS/list.o FreeRTOS/list.c 
	
${OBJECTDIR}/FreeRTOS/queue.o: FreeRTOS/queue.c  .generated_files/flags/default/4a19d0dae6eaa4684f5ab7b53d7a582ca5999e62 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS" 
	@${RM} ${OBJECTDIR}/FreeRTOS/queue.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/queue.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./FreeRTOS -I./FreeRTOS/include -I./FreeRTOS/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/FreeRTOS/queue.o.d" -MT "${OBJECTDIR}/FreeRTOS/queue.o.d" -MT ${OBJECTDIR}/FreeRTOS/queue.o -o ${OBJECTDIR}/FreeRTOS/queue.o FreeRTOS/queue.c 
	
${OBJECTDIR}/FreeRTOS/tasks.o: FreeRTOS/tasks.c  .generated_files/flags/default/3b7224d32e2f163c7a5bb7709081e0556d471552 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS" 
	@${RM} ${OBJECTDIR}/FreeRTOS/tasks.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/tasks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./FreeRTOS -I./FreeRTOS/include -I./FreeRTOS/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/FreeRTOS/tasks.o.d" -MT "${OBJECTDIR}/FreeRTOS/tasks.o.d" -MT ${OBJECTDIR}/FreeRTOS/tasks.o -o ${OBJECTDIR}/FreeRTOS/tasks.o FreeRTOS/tasks.c 
	
${OBJECTDIR}/FreeRTOS/timers.o: FreeRTOS/timers.c  .generated_files/flags/default/ccc91d38d6ab58ba66d4c943afffdee4149b35af .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS" 
	@${RM} ${OBJECTDIR}/FreeRTOS/timers.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/timers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./FreeRTOS -I./FreeRTOS/include -I./FreeRTOS/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/FreeRTOS/timers.o.d" -MT "${OBJECTDIR}/FreeRTOS/timers.o.d" -MT ${OBJECTDIR}/FreeRTOS/timers.o -o ${OBJECTDIR}/FreeRTOS/timers.o FreeRTOS/timers.c 
	
${OBJECTDIR}/FreeRTOS/portable/GCC/ATMega328/port.o: FreeRTOS/portable/GCC/ATMega328/port.c  .generated_files/flags/default/7a35c1b51c5555d2f5aa0f566f2da34a7db95091 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS/portable/GCC/ATMega328" 
	@${RM} ${OBJECTDIR}/FreeRTOS/portable/GCC/ATMega328/port.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/portable/GCC/ATMega328/port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./FreeRTOS -I./FreeRTOS/include -I./FreeRTOS/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/FreeRTOS/portable/GCC/ATMega328/port.o.d" -MT "${OBJECTDIR}/FreeRTOS/portable/GCC/ATMega328/port.o.d" -MT ${OBJECTDIR}/FreeRTOS/portable/GCC/ATMega328/port.o -o ${OBJECTDIR}/FreeRTOS/portable/GCC/ATMega328/port.o FreeRTOS/portable/GCC/ATMega328/port.c 
	
${OBJECTDIR}/FreeRTOS/portable/MemMang/heap_4.o: FreeRTOS/portable/MemMang/heap_4.c  .generated_files/flags/default/4a36f06110c546a89b70411d6b4b348418b317cd .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS/portable/MemMang" 
	@${RM} ${OBJECTDIR}/FreeRTOS/portable/MemMang/heap_4.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/portable/MemMang/heap_4.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./FreeRTOS -I./FreeRTOS/include -I./FreeRTOS/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/FreeRTOS/portable/MemMang/heap_4.o.d" -MT "${OBJECTDIR}/FreeRTOS/portable/MemMang/heap_4.o.d" -MT ${OBJECTDIR}/FreeRTOS/portable/MemMang/heap_4.o -o ${OBJECTDIR}/FreeRTOS/portable/MemMang/heap_4.o FreeRTOS/portable/MemMang/heap_4.c 
	
else
${OBJECTDIR}/Task/blink.o: Task/blink.c  .generated_files/flags/default/8cd53c84d1bc823e0bb6f0be0c984aa9275c2198 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Task" 
	@${RM} ${OBJECTDIR}/Task/blink.o.d 
	@${RM} ${OBJECTDIR}/Task/blink.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./FreeRTOS -I./FreeRTOS/include -I./FreeRTOS/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/Task/blink.o.d" -MT "${OBJECTDIR}/Task/blink.o.d" -MT ${OBJECTDIR}/Task/blink.o -o ${OBJECTDIR}/Task/blink.o Task/blink.c 
	
${OBJECTDIR}/Task/numpad.o: Task/numpad.c  .generated_files/flags/default/8e8cb074a56f63f00de32ab2efef976e780537f6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Task" 
	@${RM} ${OBJECTDIR}/Task/numpad.o.d 
	@${RM} ${OBJECTDIR}/Task/numpad.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./FreeRTOS -I./FreeRTOS/include -I./FreeRTOS/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/Task/numpad.o.d" -MT "${OBJECTDIR}/Task/numpad.o.d" -MT ${OBJECTDIR}/Task/numpad.o -o ${OBJECTDIR}/Task/numpad.o Task/numpad.c 
	
${OBJECTDIR}/Task/pwm.o: Task/pwm.c  .generated_files/flags/default/2666b0c2f01dcb6097e763aaac650ffc43e8cb61 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Task" 
	@${RM} ${OBJECTDIR}/Task/pwm.o.d 
	@${RM} ${OBJECTDIR}/Task/pwm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./FreeRTOS -I./FreeRTOS/include -I./FreeRTOS/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/Task/pwm.o.d" -MT "${OBJECTDIR}/Task/pwm.o.d" -MT ${OBJECTDIR}/Task/pwm.o -o ${OBJECTDIR}/Task/pwm.o Task/pwm.c 
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/981da9cf9f7a3e303ef1b340dca8a87e15d6e18c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./FreeRTOS -I./FreeRTOS/include -I./FreeRTOS/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/main.o.d" -MT "${OBJECTDIR}/main.o.d" -MT ${OBJECTDIR}/main.o -o ${OBJECTDIR}/main.o main.c 
	
${OBJECTDIR}/FreeRTOS/croutine.o: FreeRTOS/croutine.c  .generated_files/flags/default/e09b2463a66f3724ee5800aff3f55abc2dd706ed .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS" 
	@${RM} ${OBJECTDIR}/FreeRTOS/croutine.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/croutine.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./FreeRTOS -I./FreeRTOS/include -I./FreeRTOS/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/FreeRTOS/croutine.o.d" -MT "${OBJECTDIR}/FreeRTOS/croutine.o.d" -MT ${OBJECTDIR}/FreeRTOS/croutine.o -o ${OBJECTDIR}/FreeRTOS/croutine.o FreeRTOS/croutine.c 
	
${OBJECTDIR}/FreeRTOS/event_groups.o: FreeRTOS/event_groups.c  .generated_files/flags/default/19321b1b3d6a89f6e46e7bb186b78f8fb1fa08bb .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS" 
	@${RM} ${OBJECTDIR}/FreeRTOS/event_groups.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/event_groups.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./FreeRTOS -I./FreeRTOS/include -I./FreeRTOS/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/FreeRTOS/event_groups.o.d" -MT "${OBJECTDIR}/FreeRTOS/event_groups.o.d" -MT ${OBJECTDIR}/FreeRTOS/event_groups.o -o ${OBJECTDIR}/FreeRTOS/event_groups.o FreeRTOS/event_groups.c 
	
${OBJECTDIR}/FreeRTOS/list.o: FreeRTOS/list.c  .generated_files/flags/default/af4f9f85b25d43bd0b14c0cb6d4b4710634153ea .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS" 
	@${RM} ${OBJECTDIR}/FreeRTOS/list.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/list.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./FreeRTOS -I./FreeRTOS/include -I./FreeRTOS/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/FreeRTOS/list.o.d" -MT "${OBJECTDIR}/FreeRTOS/list.o.d" -MT ${OBJECTDIR}/FreeRTOS/list.o -o ${OBJECTDIR}/FreeRTOS/list.o FreeRTOS/list.c 
	
${OBJECTDIR}/FreeRTOS/queue.o: FreeRTOS/queue.c  .generated_files/flags/default/aaadd60847b52a457190c33c1b94f1d8f4d08127 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS" 
	@${RM} ${OBJECTDIR}/FreeRTOS/queue.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/queue.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./FreeRTOS -I./FreeRTOS/include -I./FreeRTOS/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/FreeRTOS/queue.o.d" -MT "${OBJECTDIR}/FreeRTOS/queue.o.d" -MT ${OBJECTDIR}/FreeRTOS/queue.o -o ${OBJECTDIR}/FreeRTOS/queue.o FreeRTOS/queue.c 
	
${OBJECTDIR}/FreeRTOS/tasks.o: FreeRTOS/tasks.c  .generated_files/flags/default/2d8c3adb2175a13fd70f5448d3b0dcc133234974 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS" 
	@${RM} ${OBJECTDIR}/FreeRTOS/tasks.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/tasks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./FreeRTOS -I./FreeRTOS/include -I./FreeRTOS/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/FreeRTOS/tasks.o.d" -MT "${OBJECTDIR}/FreeRTOS/tasks.o.d" -MT ${OBJECTDIR}/FreeRTOS/tasks.o -o ${OBJECTDIR}/FreeRTOS/tasks.o FreeRTOS/tasks.c 
	
${OBJECTDIR}/FreeRTOS/timers.o: FreeRTOS/timers.c  .generated_files/flags/default/79cccec2f6f5aaa4ff81bea0021040f8a18713a5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS" 
	@${RM} ${OBJECTDIR}/FreeRTOS/timers.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/timers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./FreeRTOS -I./FreeRTOS/include -I./FreeRTOS/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/FreeRTOS/timers.o.d" -MT "${OBJECTDIR}/FreeRTOS/timers.o.d" -MT ${OBJECTDIR}/FreeRTOS/timers.o -o ${OBJECTDIR}/FreeRTOS/timers.o FreeRTOS/timers.c 
	
${OBJECTDIR}/FreeRTOS/portable/GCC/ATMega328/port.o: FreeRTOS/portable/GCC/ATMega328/port.c  .generated_files/flags/default/66761a8c9c408feabf83d2db3845ee507894f4b2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS/portable/GCC/ATMega328" 
	@${RM} ${OBJECTDIR}/FreeRTOS/portable/GCC/ATMega328/port.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/portable/GCC/ATMega328/port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./FreeRTOS -I./FreeRTOS/include -I./FreeRTOS/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/FreeRTOS/portable/GCC/ATMega328/port.o.d" -MT "${OBJECTDIR}/FreeRTOS/portable/GCC/ATMega328/port.o.d" -MT ${OBJECTDIR}/FreeRTOS/portable/GCC/ATMega328/port.o -o ${OBJECTDIR}/FreeRTOS/portable/GCC/ATMega328/port.o FreeRTOS/portable/GCC/ATMega328/port.c 
	
${OBJECTDIR}/FreeRTOS/portable/MemMang/heap_4.o: FreeRTOS/portable/MemMang/heap_4.c  .generated_files/flags/default/7e7c1870b7566eea5ebebdae61bed98a9824ee0a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/FreeRTOS/portable/MemMang" 
	@${RM} ${OBJECTDIR}/FreeRTOS/portable/MemMang/heap_4.o.d 
	@${RM} ${OBJECTDIR}/FreeRTOS/portable/MemMang/heap_4.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./FreeRTOS -I./FreeRTOS/include -I./FreeRTOS/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/FreeRTOS/portable/MemMang/heap_4.o.d" -MT "${OBJECTDIR}/FreeRTOS/portable/MemMang/heap_4.o.d" -MT ${OBJECTDIR}/FreeRTOS/portable/MemMang/heap_4.o -o ${OBJECTDIR}/FreeRTOS/portable/MemMang/heap_4.o FreeRTOS/portable/MemMang/heap_4.c 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/RTOS_NoUpload.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/RTOS_NoUpload.X.${IMAGE_TYPE}.map  -D__DEBUG=1  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"   -gdwarf-2 -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -I./FreeRTOS -I./FreeRTOS/include -I./FreeRTOS/portable/GCC/ATMega328  -gdwarf-3 -mno-const-data-in-progmem --memorysummary      $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/RTOS_NoUpload.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  -o ${DISTDIR}/RTOS_NoUpload.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -Wl,--start-group  -Wl,-lm -Wl,--end-group  -Wl,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1
	@${RM} ${DISTDIR}/RTOS_NoUpload.X.${IMAGE_TYPE}.hex 
	
	
else
${DISTDIR}/RTOS_NoUpload.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/RTOS_NoUpload.X.${IMAGE_TYPE}.map  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -I./FreeRTOS -I./FreeRTOS/include -I./FreeRTOS/portable/GCC/ATMega328  -gdwarf-3 -mno-const-data-in-progmem --memorysummary      $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/RTOS_NoUpload.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  -o ${DISTDIR}/RTOS_NoUpload.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -Wl,--start-group  -Wl,-lm -Wl,--end-group 
	${MP_CC_DIR}\\avr-objcopy -O ihex "${DISTDIR}/RTOS_NoUpload.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "${DISTDIR}/RTOS_NoUpload.X.${IMAGE_TYPE}.hex"
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
