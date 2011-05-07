#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Release
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/RReconstWindow.o \
	${OBJECTDIR}/R3DScene.o \
	${OBJECTDIR}/RExporter.o \
	${OBJECTDIR}/R3DSceneWindow.o \
	${OBJECTDIR}/RCvCamera.o \
	${OBJECTDIR}/RImageWindow.o \
	${OBJECTDIR}/RCalibUtil.o \
	${OBJECTDIR}/RCaptWindow.o \
	${OBJECTDIR}/RPixelQueue.o \
	${OBJECTDIR}/RScanUtil.o \
	${OBJECTDIR}/R3DCamera.o \
	${OBJECTDIR}/R3DUtil.o \
	${OBJECTDIR}/RApp.o \
	${OBJECTDIR}/RCalibWindow.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Release.mk dist/Release/GNU-Linux-x86/reconstapp

dist/Release/GNU-Linux-x86/reconstapp: ${OBJECTFILES}
	${MKDIR} -p dist/Release/GNU-Linux-x86
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/reconstapp ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/RReconstWindow.o: nbproject/Makefile-${CND_CONF}.mk RReconstWindow.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/RReconstWindow.o RReconstWindow.cpp

${OBJECTDIR}/R3DScene.o: nbproject/Makefile-${CND_CONF}.mk R3DScene.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/R3DScene.o R3DScene.cpp

${OBJECTDIR}/RExporter.o: nbproject/Makefile-${CND_CONF}.mk RExporter.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/RExporter.o RExporter.cpp

${OBJECTDIR}/R3DSceneWindow.o: nbproject/Makefile-${CND_CONF}.mk R3DSceneWindow.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/R3DSceneWindow.o R3DSceneWindow.cpp

${OBJECTDIR}/RCvCamera.o: nbproject/Makefile-${CND_CONF}.mk RCvCamera.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/RCvCamera.o RCvCamera.cpp

${OBJECTDIR}/RImageWindow.o: nbproject/Makefile-${CND_CONF}.mk RImageWindow.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/RImageWindow.o RImageWindow.cpp

${OBJECTDIR}/RCalibUtil.o: nbproject/Makefile-${CND_CONF}.mk RCalibUtil.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/RCalibUtil.o RCalibUtil.cpp

${OBJECTDIR}/RCaptWindow.o: nbproject/Makefile-${CND_CONF}.mk RCaptWindow.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/RCaptWindow.o RCaptWindow.cpp

${OBJECTDIR}/RPixelQueue.o: nbproject/Makefile-${CND_CONF}.mk RPixelQueue.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/RPixelQueue.o RPixelQueue.cpp

${OBJECTDIR}/RScanUtil.o: nbproject/Makefile-${CND_CONF}.mk RScanUtil.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/RScanUtil.o RScanUtil.cpp

${OBJECTDIR}/R3DCamera.o: nbproject/Makefile-${CND_CONF}.mk R3DCamera.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/R3DCamera.o R3DCamera.cpp

${OBJECTDIR}/R3DUtil.o: nbproject/Makefile-${CND_CONF}.mk R3DUtil.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/R3DUtil.o R3DUtil.cpp

${OBJECTDIR}/RApp.o: nbproject/Makefile-${CND_CONF}.mk RApp.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/RApp.o RApp.cpp

${OBJECTDIR}/RCalibWindow.o: nbproject/Makefile-${CND_CONF}.mk RCalibWindow.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/RCalibWindow.o RCalibWindow.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Release
	${RM} dist/Release/GNU-Linux-x86/reconstapp

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
