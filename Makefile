BOARD        = pro
BOARD_SUB    = 16MHzatmega328
PORT         = /some/port
EXTRA_FLAGS  =

ifdef ARD_HOME
$(info Using ARD_HOME = $(ARD_HOME))
endif

ifeq ($(debug),1)
EXTRA_FLAGS += -DDEBUG=1
endif

include _Makefile.master
