export IMPERAS_ARCH=Linux64
ifndef IMPERAS_HOME
  IMPERAS_ERROR := $(error "IMPERAS_HOME not defined, please setup Imperas/OVP environment")
endif
IMPERAS_HOME := $(shell getpath.exe "$(IMPERAS_HOME)")

# Build locally without using a VLNV library
NOVLNV=1

OPTLM=1
SRC=platform.cpp DMA.cpp adaptor.cpp

include ${IMPERAS_HOME}/ImperasLib/buildutils/Makefile.TLM.platform
