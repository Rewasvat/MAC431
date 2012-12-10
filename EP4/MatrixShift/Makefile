DEPTH = ../../../../..

include $(DEPTH)/make/openclsdkdefs.mk 

####
#
#  Targets
#
####

OPENCL			= 1
SAMPLE_EXE		= 1
EXE_TARGET 		= MatrixShift
EXE_TARGET_INSTALL   	= MatrixShift

####
#
#  C/CPP files
#
####

FILES 	= MatrixShift
CLFILES	= MatrixShift_Kernel.cl

LLIBS  += SDKUtil

include $(DEPTH)/make/openclsdkrules.mk 

