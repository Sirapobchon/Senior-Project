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
FINAL_IMAGE=${DISTDIR}/RTOS_v8.2.0.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/RTOS_v8.2.0.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
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
SOURCEFILES_QUOTED_IF_SPACED=apptasks.c main.c Source/croutine.c Source/event_groups.c Source/list.c Source/queue.c Source/tasks.c Source/timers.c Source/portable/MemMang/heap_1.c Source/portable/GCC/ATMega328/port.c Drivers/led.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/apptasks.o ${OBJECTDIR}/main.o ${OBJECTDIR}/Source/croutine.o ${OBJECTDIR}/Source/event_groups.o ${OBJECTDIR}/Source/list.o ${OBJECTDIR}/Source/queue.o ${OBJECTDIR}/Source/tasks.o ${OBJECTDIR}/Source/timers.o ${OBJECTDIR}/Source/portable/MemMang/heap_1.o ${OBJECTDIR}/Source/portable/GCC/ATMega328/port.o ${OBJECTDIR}/Drivers/led.o
POSSIBLE_DEPFILES=${OBJECTDIR}/apptasks.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/Source/croutine.o.d ${OBJECTDIR}/Source/event_groups.o.d ${OBJECTDIR}/Source/list.o.d ${OBJECTDIR}/Source/queue.o.d ${OBJECTDIR}/Source/tasks.o.d ${OBJECTDIR}/Source/timers.o.d ${OBJECTDIR}/Source/portable/MemMang/heap_1.o.d ${OBJECTDIR}/Source/portable/GCC/ATMega328/port.o.d ${OBJECTDIR}/Drivers/led.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/apptasks.o ${OBJECTDIR}/main.o ${OBJECTDIR}/Source/croutine.o ${OBJECTDIR}/Source/event_groups.o ${OBJECTDIR}/Source/list.o ${OBJECTDIR}/Source/queue.o ${OBJECTDIR}/Source/tasks.o ${OBJECTDIR}/Source/timers.o ${OBJECTDIR}/Source/portable/MemMang/heap_1.o ${OBJECTDIR}/Source/portable/GCC/ATMega328/port.o ${OBJECTDIR}/Drivers/led.o

# Source Files
SOURCEFILES=apptasks.c main.c Source/croutine.c Source/event_groups.c Source/list.c Source/queue.c Source/tasks.c Source/timers.c Source/portable/MemMang/heap_1.c Source/portable/GCC/ATMega328/port.c Drivers/led.c



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

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/RTOS_v8.2.0.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=ATmega328P
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/apptasks.o: apptasks.c  .generated_files/flags/default/a0ecbda2872eadf4823283d8fd5d19ceffef25fc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/apptasks.o.d 
	@${RM} ${OBJECTDIR}/apptasks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./Source -I./Drivers -I./Source/include -I./Source/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/apptasks.o.d" -MT "${OBJECTDIR}/apptasks.o.d" -MT ${OBJECTDIR}/apptasks.o -o ${OBJECTDIR}/apptasks.o apptasks.c 
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/d4268373eced80a618f4c962605bdad5d5475ded .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./Source -I./Drivers -I./Source/include -I./Source/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/main.o.d" -MT "${OBJECTDIR}/main.o.d" -MT ${OBJECTDIR}/main.o -o ${OBJECTDIR}/main.o main.c 
	
${OBJECTDIR}/Source/croutine.o: Source/croutine.c  .generated_files/flags/default/90978c9d1b4e6e5b33808e6a4fb8e25a0dfcafc1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Source" 
	@${RM} ${OBJECTDIR}/Source/croutine.o.d 
	@${RM} ${OBJECTDIR}/Source/croutine.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./Source -I./Drivers -I./Source/include -I./Source/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/Source/croutine.o.d" -MT "${OBJECTDIR}/Source/croutine.o.d" -MT ${OBJECTDIR}/Source/croutine.o -o ${OBJECTDIR}/Source/croutine.o Source/croutine.c 
	
${OBJECTDIR}/Source/event_groups.o: Source/event_groups.c  .generated_files/flags/default/48352e73e21cc214f66a59e91f0f3c26693cbf5e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Source" 
	@${RM} ${OBJECTDIR}/Source/event_groups.o.d 
	@${RM} ${OBJECTDIR}/Source/event_groups.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./Source -I./Drivers -I./Source/include -I./Source/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/Source/event_groups.o.d" -MT "${OBJECTDIR}/Source/event_groups.o.d" -MT ${OBJECTDIR}/Source/event_groups.o -o ${OBJECTDIR}/Source/event_groups.o Source/event_groups.c 
	
${OBJECTDIR}/Source/list.o: Source/list.c  .generated_files/flags/default/a6e20f79c4f8ec740b3f5e79408a84edbaa98a12 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Source" 
	@${RM} ${OBJECTDIR}/Source/list.o.d 
	@${RM} ${OBJECTDIR}/Source/list.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./Source -I./Drivers -I./Source/include -I./Source/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/Source/list.o.d" -MT "${OBJECTDIR}/Source/list.o.d" -MT ${OBJECTDIR}/Source/list.o -o ${OBJECTDIR}/Source/list.o Source/list.c 
	
${OBJECTDIR}/Source/queue.o: Source/queue.c  .generated_files/flags/default/82d93a8d0155cadcd425134fe48cca08b5707f6d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Source" 
	@${RM} ${OBJECTDIR}/Source/queue.o.d 
	@${RM} ${OBJECTDIR}/Source/queue.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./Source -I./Drivers -I./Source/include -I./Source/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/Source/queue.o.d" -MT "${OBJECTDIR}/Source/queue.o.d" -MT ${OBJECTDIR}/Source/queue.o -o ${OBJECTDIR}/Source/queue.o Source/queue.c 
	
${OBJECTDIR}/Source/tasks.o: Source/tasks.c  .generated_files/flags/default/4dacd84c9baf4775084b90d6137dd6cd2624fd1d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Source" 
	@${RM} ${OBJECTDIR}/Source/tasks.o.d 
	@${RM} ${OBJECTDIR}/Source/tasks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./Source -I./Drivers -I./Source/include -I./Source/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/Source/tasks.o.d" -MT "${OBJECTDIR}/Source/tasks.o.d" -MT ${OBJECTDIR}/Source/tasks.o -o ${OBJECTDIR}/Source/tasks.o Source/tasks.c 
	
${OBJECTDIR}/Source/timers.o: Source/timers.c  .generated_files/flags/default/92c10f6b9102c7b34afa72cbabc87ab8580e7351 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Source" 
	@${RM} ${OBJECTDIR}/Source/timers.o.d 
	@${RM} ${OBJECTDIR}/Source/timers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./Source -I./Drivers -I./Source/include -I./Source/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/Source/timers.o.d" -MT "${OBJECTDIR}/Source/timers.o.d" -MT ${OBJECTDIR}/Source/timers.o -o ${OBJECTDIR}/Source/timers.o Source/timers.c 
	
${OBJECTDIR}/Source/portable/MemMang/heap_1.o: Source/portable/MemMang/heap_1.c  .generated_files/flags/default/3e8fb2a67f9d6aa888bc652357640461c3824a73 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Source/portable/MemMang" 
	@${RM} ${OBJECTDIR}/Source/portable/MemMang/heap_1.o.d 
	@${RM} ${OBJECTDIR}/Source/portable/MemMang/heap_1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./Source -I./Drivers -I./Source/include -I./Source/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/Source/portable/MemMang/heap_1.o.d" -MT "${OBJECTDIR}/Source/portable/MemMang/heap_1.o.d" -MT ${OBJECTDIR}/Source/portable/MemMang/heap_1.o -o ${OBJECTDIR}/Source/portable/MemMang/heap_1.o Source/portable/MemMang/heap_1.c 
	
${OBJECTDIR}/Source/portable/GCC/ATMega328/port.o: Source/portable/GCC/ATMega328/port.c  .generated_files/flags/default/e0c01e31a897a08442193a1dcffc8cd141b9491b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Source/portable/GCC/ATMega328" 
	@${RM} ${OBJECTDIR}/Source/portable/GCC/ATMega328/port.o.d 
	@${RM} ${OBJECTDIR}/Source/portable/GCC/ATMega328/port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./Source -I./Drivers -I./Source/include -I./Source/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/Source/portable/GCC/ATMega328/port.o.d" -MT "${OBJECTDIR}/Source/portable/GCC/ATMega328/port.o.d" -MT ${OBJECTDIR}/Source/portable/GCC/ATMega328/port.o -o ${OBJECTDIR}/Source/portable/GCC/ATMega328/port.o Source/portable/GCC/ATMega328/port.c 
	
${OBJECTDIR}/Drivers/led.o: Drivers/led.c  .generated_files/flags/default/aa250340c497600525479bc199c1bbe2f7b85172 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Drivers" 
	@${RM} ${OBJECTDIR}/Drivers/led.o.d 
	@${RM} ${OBJECTDIR}/Drivers/led.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./Source -I./Drivers -I./Source/include -I./Source/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/Drivers/led.o.d" -MT "${OBJECTDIR}/Drivers/led.o.d" -MT ${OBJECTDIR}/Drivers/led.o -o ${OBJECTDIR}/Drivers/led.o Drivers/led.c 
	
else
${OBJECTDIR}/apptasks.o: apptasks.c  .generated_files/flags/default/c04d4ad02c106bbefed56baca5a3985f403fb246 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/apptasks.o.d 
	@${RM} ${OBJECTDIR}/apptasks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./Source -I./Drivers -I./Source/include -I./Source/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/apptasks.o.d" -MT "${OBJECTDIR}/apptasks.o.d" -MT ${OBJECTDIR}/apptasks.o -o ${OBJECTDIR}/apptasks.o apptasks.c 
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/cef469e46aee64f11e7de16af411deb45845f5c9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./Source -I./Drivers -I./Source/include -I./Source/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/main.o.d" -MT "${OBJECTDIR}/main.o.d" -MT ${OBJECTDIR}/main.o -o ${OBJECTDIR}/main.o main.c 
	
${OBJECTDIR}/Source/croutine.o: Source/croutine.c  .generated_files/flags/default/46d0d497432d403952c64528bd8478cea8b03019 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Source" 
	@${RM} ${OBJECTDIR}/Source/croutine.o.d 
	@${RM} ${OBJECTDIR}/Source/croutine.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./Source -I./Drivers -I./Source/include -I./Source/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/Source/croutine.o.d" -MT "${OBJECTDIR}/Source/croutine.o.d" -MT ${OBJECTDIR}/Source/croutine.o -o ${OBJECTDIR}/Source/croutine.o Source/croutine.c 
	
${OBJECTDIR}/Source/event_groups.o: Source/event_groups.c  .generated_files/flags/default/389c2269bb5bd109f47647a246effcf22c9ecc92 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Source" 
	@${RM} ${OBJECTDIR}/Source/event_groups.o.d 
	@${RM} ${OBJECTDIR}/Source/event_groups.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./Source -I./Drivers -I./Source/include -I./Source/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/Source/event_groups.o.d" -MT "${OBJECTDIR}/Source/event_groups.o.d" -MT ${OBJECTDIR}/Source/event_groups.o -o ${OBJECTDIR}/Source/event_groups.o Source/event_groups.c 
	
${OBJECTDIR}/Source/list.o: Source/list.c  .generated_files/flags/default/36c079de6ec7b7b6b44b09e4ca3afa10b3184a73 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Source" 
	@${RM} ${OBJECTDIR}/Source/list.o.d 
	@${RM} ${OBJECTDIR}/Source/list.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./Source -I./Drivers -I./Source/include -I./Source/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/Source/list.o.d" -MT "${OBJECTDIR}/Source/list.o.d" -MT ${OBJECTDIR}/Source/list.o -o ${OBJECTDIR}/Source/list.o Source/list.c 
	
${OBJECTDIR}/Source/queue.o: Source/queue.c  .generated_files/flags/default/3051ba3745f6698dd423a46f4c63385a1ddd70cb .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Source" 
	@${RM} ${OBJECTDIR}/Source/queue.o.d 
	@${RM} ${OBJECTDIR}/Source/queue.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./Source -I./Drivers -I./Source/include -I./Source/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/Source/queue.o.d" -MT "${OBJECTDIR}/Source/queue.o.d" -MT ${OBJECTDIR}/Source/queue.o -o ${OBJECTDIR}/Source/queue.o Source/queue.c 
	
${OBJECTDIR}/Source/tasks.o: Source/tasks.c  .generated_files/flags/default/2dc9b68de562993e84f07869eb902d1a5c6808ae .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Source" 
	@${RM} ${OBJECTDIR}/Source/tasks.o.d 
	@${RM} ${OBJECTDIR}/Source/tasks.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./Source -I./Drivers -I./Source/include -I./Source/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/Source/tasks.o.d" -MT "${OBJECTDIR}/Source/tasks.o.d" -MT ${OBJECTDIR}/Source/tasks.o -o ${OBJECTDIR}/Source/tasks.o Source/tasks.c 
	
${OBJECTDIR}/Source/timers.o: Source/timers.c  .generated_files/flags/default/1423a24fc17d582f32edfa5a0ea60d35a5c8d58 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Source" 
	@${RM} ${OBJECTDIR}/Source/timers.o.d 
	@${RM} ${OBJECTDIR}/Source/timers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./Source -I./Drivers -I./Source/include -I./Source/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/Source/timers.o.d" -MT "${OBJECTDIR}/Source/timers.o.d" -MT ${OBJECTDIR}/Source/timers.o -o ${OBJECTDIR}/Source/timers.o Source/timers.c 
	
${OBJECTDIR}/Source/portable/MemMang/heap_1.o: Source/portable/MemMang/heap_1.c  .generated_files/flags/default/b62b63e352a6993fb52bbd1465e8ed8574a1dca1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Source/portable/MemMang" 
	@${RM} ${OBJECTDIR}/Source/portable/MemMang/heap_1.o.d 
	@${RM} ${OBJECTDIR}/Source/portable/MemMang/heap_1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./Source -I./Drivers -I./Source/include -I./Source/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/Source/portable/MemMang/heap_1.o.d" -MT "${OBJECTDIR}/Source/portable/MemMang/heap_1.o.d" -MT ${OBJECTDIR}/Source/portable/MemMang/heap_1.o -o ${OBJECTDIR}/Source/portable/MemMang/heap_1.o Source/portable/MemMang/heap_1.c 
	
${OBJECTDIR}/Source/portable/GCC/ATMega328/port.o: Source/portable/GCC/ATMega328/port.c  .generated_files/flags/default/3be7e7c1b48e26f8d7ccca97478a07339265105f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Source/portable/GCC/ATMega328" 
	@${RM} ${OBJECTDIR}/Source/portable/GCC/ATMega328/port.o.d 
	@${RM} ${OBJECTDIR}/Source/portable/GCC/ATMega328/port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./Source -I./Drivers -I./Source/include -I./Source/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/Source/portable/GCC/ATMega328/port.o.d" -MT "${OBJECTDIR}/Source/portable/GCC/ATMega328/port.o.d" -MT ${OBJECTDIR}/Source/portable/GCC/ATMega328/port.o -o ${OBJECTDIR}/Source/portable/GCC/ATMega328/port.o Source/portable/GCC/ATMega328/port.c 
	
${OBJECTDIR}/Drivers/led.o: Drivers/led.c  .generated_files/flags/default/8629a43edb9cb3f574794c64706ef860891735ed .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/Drivers" 
	@${RM} ${OBJECTDIR}/Drivers/led.o.d 
	@${RM} ${OBJECTDIR}/Drivers/led.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem -I./Source -I./Drivers -I./Source/include -I./Source/portable/GCC/ATMega328      -MD -MP -MF "${OBJECTDIR}/Drivers/led.o.d" -MT "${OBJECTDIR}/Drivers/led.o.d" -MT ${OBJECTDIR}/Drivers/led.o -o ${OBJECTDIR}/Drivers/led.o Drivers/led.c 
	
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
${DISTDIR}/RTOS_v8.2.0.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/RTOS_v8.2.0.X.${IMAGE_TYPE}.map  -D__DEBUG=1  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"   -gdwarf-2 -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -I./Source -I./Drivers -I./Source/include -I./Source/portable/GCC/ATMega328  -gdwarf-3 -mno-const-data-in-progmem     $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/RTOS_v8.2.0.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  -o ${DISTDIR}/RTOS_v8.2.0.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -Wl,--start-group  -Wl,-lm -Wl,--end-group  -Wl,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1
	@${RM} ${DISTDIR}/RTOS_v8.2.0.X.${IMAGE_TYPE}.hex 
	
	
else
${DISTDIR}/RTOS_v8.2.0.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/RTOS_v8.2.0.X.${IMAGE_TYPE}.map  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -I./Source -I./Drivers -I./Source/include -I./Source/portable/GCC/ATMega328  -gdwarf-3 -mno-const-data-in-progmem     $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/RTOS_v8.2.0.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  -o ${DISTDIR}/RTOS_v8.2.0.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -Wl,--start-group  -Wl,-lm -Wl,--end-group 
	${MP_CC_DIR}\\avr-objcopy -O ihex "${DISTDIR}/RTOS_v8.2.0.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "${DISTDIR}/RTOS_v8.2.0.X.${IMAGE_TYPE}.hex"
	
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
