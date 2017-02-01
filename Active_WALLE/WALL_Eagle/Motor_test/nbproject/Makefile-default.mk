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
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Motor_test.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Motor_test.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
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
SOURCEFILES_QUOTED_IF_SPACED=main.c ../../../../../CMPE118/src/AD.c ../../../../../CMPE118/src/BOARD.c ../../../../../CMPE118/src/IO_Ports.c ../../../../../CMPE118/src/LED.c ../../../../../CMPE118/src/pwm.c ../../../../../CMPE118/src/serial.c ../../../../../CMPE118/src/timers.c motors.c ../../../../../CMPE118/src/RC_Servo.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/main.o ${OBJECTDIR}/_ext/1953262111/AD.o ${OBJECTDIR}/_ext/1953262111/BOARD.o ${OBJECTDIR}/_ext/1953262111/IO_Ports.o ${OBJECTDIR}/_ext/1953262111/LED.o ${OBJECTDIR}/_ext/1953262111/pwm.o ${OBJECTDIR}/_ext/1953262111/serial.o ${OBJECTDIR}/_ext/1953262111/timers.o ${OBJECTDIR}/motors.o ${OBJECTDIR}/_ext/1953262111/RC_Servo.o
POSSIBLE_DEPFILES=${OBJECTDIR}/main.o.d ${OBJECTDIR}/_ext/1953262111/AD.o.d ${OBJECTDIR}/_ext/1953262111/BOARD.o.d ${OBJECTDIR}/_ext/1953262111/IO_Ports.o.d ${OBJECTDIR}/_ext/1953262111/LED.o.d ${OBJECTDIR}/_ext/1953262111/pwm.o.d ${OBJECTDIR}/_ext/1953262111/serial.o.d ${OBJECTDIR}/_ext/1953262111/timers.o.d ${OBJECTDIR}/motors.o.d ${OBJECTDIR}/_ext/1953262111/RC_Servo.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/main.o ${OBJECTDIR}/_ext/1953262111/AD.o ${OBJECTDIR}/_ext/1953262111/BOARD.o ${OBJECTDIR}/_ext/1953262111/IO_Ports.o ${OBJECTDIR}/_ext/1953262111/LED.o ${OBJECTDIR}/_ext/1953262111/pwm.o ${OBJECTDIR}/_ext/1953262111/serial.o ${OBJECTDIR}/_ext/1953262111/timers.o ${OBJECTDIR}/motors.o ${OBJECTDIR}/_ext/1953262111/RC_Servo.o

# Source Files
SOURCEFILES=main.c ../../../../../CMPE118/src/AD.c ../../../../../CMPE118/src/BOARD.c ../../../../../CMPE118/src/IO_Ports.c ../../../../../CMPE118/src/LED.c ../../../../../CMPE118/src/pwm.c ../../../../../CMPE118/src/serial.c ../../../../../CMPE118/src/timers.c motors.c ../../../../../CMPE118/src/RC_Servo.c


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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/Motor_test.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX320F128H
MP_LINKER_FILE_OPTION=,--script="..\..\..\..\..\CMPE118\bootloader320.ld"
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
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1953262111/AD.o: ../../../../../CMPE118/src/AD.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1953262111" 
	@${RM} ${OBJECTDIR}/_ext/1953262111/AD.o.d 
	@${RM} ${OBJECTDIR}/_ext/1953262111/AD.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1953262111/AD.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/1953262111/AD.o.d" -o ${OBJECTDIR}/_ext/1953262111/AD.o ../../../../../CMPE118/src/AD.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1953262111/BOARD.o: ../../../../../CMPE118/src/BOARD.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1953262111" 
	@${RM} ${OBJECTDIR}/_ext/1953262111/BOARD.o.d 
	@${RM} ${OBJECTDIR}/_ext/1953262111/BOARD.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1953262111/BOARD.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/1953262111/BOARD.o.d" -o ${OBJECTDIR}/_ext/1953262111/BOARD.o ../../../../../CMPE118/src/BOARD.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1953262111/IO_Ports.o: ../../../../../CMPE118/src/IO_Ports.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1953262111" 
	@${RM} ${OBJECTDIR}/_ext/1953262111/IO_Ports.o.d 
	@${RM} ${OBJECTDIR}/_ext/1953262111/IO_Ports.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1953262111/IO_Ports.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/1953262111/IO_Ports.o.d" -o ${OBJECTDIR}/_ext/1953262111/IO_Ports.o ../../../../../CMPE118/src/IO_Ports.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1953262111/LED.o: ../../../../../CMPE118/src/LED.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1953262111" 
	@${RM} ${OBJECTDIR}/_ext/1953262111/LED.o.d 
	@${RM} ${OBJECTDIR}/_ext/1953262111/LED.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1953262111/LED.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/1953262111/LED.o.d" -o ${OBJECTDIR}/_ext/1953262111/LED.o ../../../../../CMPE118/src/LED.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1953262111/pwm.o: ../../../../../CMPE118/src/pwm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1953262111" 
	@${RM} ${OBJECTDIR}/_ext/1953262111/pwm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1953262111/pwm.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1953262111/pwm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/1953262111/pwm.o.d" -o ${OBJECTDIR}/_ext/1953262111/pwm.o ../../../../../CMPE118/src/pwm.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1953262111/serial.o: ../../../../../CMPE118/src/serial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1953262111" 
	@${RM} ${OBJECTDIR}/_ext/1953262111/serial.o.d 
	@${RM} ${OBJECTDIR}/_ext/1953262111/serial.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1953262111/serial.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/1953262111/serial.o.d" -o ${OBJECTDIR}/_ext/1953262111/serial.o ../../../../../CMPE118/src/serial.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1953262111/timers.o: ../../../../../CMPE118/src/timers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1953262111" 
	@${RM} ${OBJECTDIR}/_ext/1953262111/timers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1953262111/timers.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1953262111/timers.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/1953262111/timers.o.d" -o ${OBJECTDIR}/_ext/1953262111/timers.o ../../../../../CMPE118/src/timers.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/motors.o: motors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/motors.o.d 
	@${RM} ${OBJECTDIR}/motors.o 
	@${FIXDEPS} "${OBJECTDIR}/motors.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/motors.o.d" -o ${OBJECTDIR}/motors.o motors.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1953262111/RC_Servo.o: ../../../../../CMPE118/src/RC_Servo.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1953262111" 
	@${RM} ${OBJECTDIR}/_ext/1953262111/RC_Servo.o.d 
	@${RM} ${OBJECTDIR}/_ext/1953262111/RC_Servo.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1953262111/RC_Servo.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/1953262111/RC_Servo.o.d" -o ${OBJECTDIR}/_ext/1953262111/RC_Servo.o ../../../../../CMPE118/src/RC_Servo.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
else
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1953262111/AD.o: ../../../../../CMPE118/src/AD.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1953262111" 
	@${RM} ${OBJECTDIR}/_ext/1953262111/AD.o.d 
	@${RM} ${OBJECTDIR}/_ext/1953262111/AD.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1953262111/AD.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/1953262111/AD.o.d" -o ${OBJECTDIR}/_ext/1953262111/AD.o ../../../../../CMPE118/src/AD.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1953262111/BOARD.o: ../../../../../CMPE118/src/BOARD.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1953262111" 
	@${RM} ${OBJECTDIR}/_ext/1953262111/BOARD.o.d 
	@${RM} ${OBJECTDIR}/_ext/1953262111/BOARD.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1953262111/BOARD.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/1953262111/BOARD.o.d" -o ${OBJECTDIR}/_ext/1953262111/BOARD.o ../../../../../CMPE118/src/BOARD.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1953262111/IO_Ports.o: ../../../../../CMPE118/src/IO_Ports.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1953262111" 
	@${RM} ${OBJECTDIR}/_ext/1953262111/IO_Ports.o.d 
	@${RM} ${OBJECTDIR}/_ext/1953262111/IO_Ports.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1953262111/IO_Ports.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/1953262111/IO_Ports.o.d" -o ${OBJECTDIR}/_ext/1953262111/IO_Ports.o ../../../../../CMPE118/src/IO_Ports.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1953262111/LED.o: ../../../../../CMPE118/src/LED.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1953262111" 
	@${RM} ${OBJECTDIR}/_ext/1953262111/LED.o.d 
	@${RM} ${OBJECTDIR}/_ext/1953262111/LED.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1953262111/LED.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/1953262111/LED.o.d" -o ${OBJECTDIR}/_ext/1953262111/LED.o ../../../../../CMPE118/src/LED.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1953262111/pwm.o: ../../../../../CMPE118/src/pwm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1953262111" 
	@${RM} ${OBJECTDIR}/_ext/1953262111/pwm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1953262111/pwm.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1953262111/pwm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/1953262111/pwm.o.d" -o ${OBJECTDIR}/_ext/1953262111/pwm.o ../../../../../CMPE118/src/pwm.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1953262111/serial.o: ../../../../../CMPE118/src/serial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1953262111" 
	@${RM} ${OBJECTDIR}/_ext/1953262111/serial.o.d 
	@${RM} ${OBJECTDIR}/_ext/1953262111/serial.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1953262111/serial.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/1953262111/serial.o.d" -o ${OBJECTDIR}/_ext/1953262111/serial.o ../../../../../CMPE118/src/serial.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1953262111/timers.o: ../../../../../CMPE118/src/timers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1953262111" 
	@${RM} ${OBJECTDIR}/_ext/1953262111/timers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1953262111/timers.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1953262111/timers.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/1953262111/timers.o.d" -o ${OBJECTDIR}/_ext/1953262111/timers.o ../../../../../CMPE118/src/timers.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/motors.o: motors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/motors.o.d 
	@${RM} ${OBJECTDIR}/motors.o 
	@${FIXDEPS} "${OBJECTDIR}/motors.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/motors.o.d" -o ${OBJECTDIR}/motors.o motors.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1953262111/RC_Servo.o: ../../../../../CMPE118/src/RC_Servo.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1953262111" 
	@${RM} ${OBJECTDIR}/_ext/1953262111/RC_Servo.o.d 
	@${RM} ${OBJECTDIR}/_ext/1953262111/RC_Servo.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1953262111/RC_Servo.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"." -I"C:/CMPE118/include" -MMD -MF "${OBJECTDIR}/_ext/1953262111/RC_Servo.o.d" -o ${OBJECTDIR}/_ext/1953262111/RC_Servo.o ../../../../../CMPE118/src/RC_Servo.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD) 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/Motor_test.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../../../../../CMPE118/bootloader320.ld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/Motor_test.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)    -mreserve=boot@0x1FC02000:0x1FC02FEF -mreserve=boot@0x1FC02000:0x1FC024FF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,--defsym=_min_heap_size=10,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/Motor_test.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../../../../../CMPE118/bootloader320.ld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/Motor_test.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=10,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/Motor_test.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
