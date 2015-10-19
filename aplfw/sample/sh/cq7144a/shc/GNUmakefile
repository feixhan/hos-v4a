# ----------------------------------------------------------------------------
# Hyper Operating System  Application Framework
#
# Copyright (C) 2007-2008 by Project HOS
# http://sourceforge.jp/projects/hos/
# ----------------------------------------------------------------------------


# %jp{�^�[�Q�b�g��}
TARGET ?= sample

# %jp{�f�B���N�g��}
TOP_DIR             = ../../../../..
KERNEL_DIR          = $(TOP_DIR)/kernel
KERNEL_CFGRTR_DIR   = $(TOP_DIR)/cfgrtr/build/gcc
KERNEL_MAKINC_DIR   = $(KERNEL_DIR)/build/common/gmake
KERNEL_BUILD_DIR    = $(KERNEL_DIR)/build/sh/sh2/shc
HOSAPLFW_DIR        = $(TOP_DIR)/aplfw
HOSAPLFW_INC_DIR    = $(HOSAPLFW_DIR)
HOSAPLFW_BUILD_DIR  = $(HOSAPLFW_DIR)/build/sh/sh2/shc
OBJS_DIR            = objs_$(TARGET)


# %jp{���ʒ�`�Ǎ���}
include $(KERNEL_MAKINC_DIR)/common.inc


# %jp{�R���t�B�M�����[�^��`}
KERNEL_CFGRTR = $(KERNEL_CFGRTR_DIR)/h4acfg-sh2


# %jp{���C�u������`}
HOSAPLFW_LIB = $(HOSAPLFW_BUILD_DIR)/hosaplfw.$(EXT_LIB)
STD_LIBS  = stdlib.lib


# %jp{�������}�b�v}
ifeq ($(MEMMAP),ext)
# %jp{�O��������}
TARGET       := $(TARGET)ext
SECTION_VECT ?= 00400000
SECTION_RO  ?= 00400400
SECTION_RW  ?= 00410000
else
# %jp{����ROM������}
SECTION_VECT ?= 00000000
SECTION_RO  ?= 00000400
SECTION_RW  ?= 00400000
endif


# %jp{�f�o�b�O�ł̒�`�ύX}
ifeq ($(DEBUG),Yes)
TARGET := $(TARGET)dbg
HOSAPLFW_LIB = $(HOSAPLFW_BUILD_DIR)/hosaplfwdbg.$(EXT_LIB)
endif


# %jp{�t���O�ݒ�}
CFLAGS  += -CP=sh2 -DEBug -NOLOGO
AFLAGS  += -CP=sh2 -DEBug -NOLOGO
LNFLAGS += 


# %jp{�o�̓t�@�C����}
TARGET_EXE = $(TARGET).$(EXT_EXE)
TARGET_MOT = $(TARGET).$(EXT_MOT)


# %jp{���s�t�@�C�������p�̐ݒ�Ǎ���}
include $(KERNEL_MAKINC_DIR)/makexe_d.inc

# %jp{shc�p�̐ݒ�Ǎ���}
include $(KERNEL_MAKINC_DIR)/shc_d.inc


# %jp{�C���N���[�h�f�B���N�g��}
INC_DIRS += $(HOSAPLFW_INC_DIR)

# %jp{�\�[�X�f�B���N�g��}
SRC_DIRS += . ..


# %jp{�A�Z���u���t�@�C���̒ǉ�}
ASRCS += ./vcttbl.src		\
         ./startup.src		\


# %jp{C����t�@�C���̒ǉ�}
CSRCS += ./dbsct.c			\
         ../kernel_cfg.c	\
         ../main.c			\
         ../boot.c			\
         ../ostimer.c


# %jp{���C�u�����t�@�C���̒ǉ�}
LIBS += $(HOSAPLFW_LIB) $(STD_LIBS)



# --------------------------------------
#  %jp{���[��}
# --------------------------------------

.PHONY : all
all: make_libs makeexe_all $(TARGET_EXE) $(TARGET_MOT)


.PHONY : make_libs
make_libs:
	$(MAKE) -C $(HOSAPLFW_BUILD_DIR)

.PHONY : clean
clean: makeexe_clean
	rm -f $(TARGET_EXE) $(TARGET_EXE) $(OBJS) ../kernel_cfg.c ../kernel_id.h

.PHONY : mostlyclean
mostlyclean: clean kernel_clean
	$(MAKE) -C $(HOSAPLFW_BUILD_DIR) clean

.PHONY : depend
depend: makeexe_depend

.PHONY : mostlydepend
mostlydepend: depend kernel_depend
	$(MAKE) -C $(HOSAPLFW_BUILD_DIR) depend

.PHONY : srccpy
srccpy: makeexe_srccpy
	$(MAKE) -C $(KERNEL_BUILD_DIR) srccpy
	$(MAKE) -C $(HOSAPLFW_BUILD_DIR) srccpy


$(STD_LIBS):
	lbgsh -OUTPut=$(STD_LIBS) -CP=sh2
#	lbgsh -OUTPut=$(STD_LIBS) -CP=sh2 -REent 


../kernel_cfg.c ../kernel_id.h: ../system.cfg
	cpp -E ../system.cfg ../system.i
	$(KERNEL_CFGRTR) ../system.i -c ../kernel_cfg.c -i ../kernel_id.h



# %jp{���s�t�@�C�������p���[���Ǎ���}
include $(KERNEL_MAKINC_DIR)/makexe_r.inc

# %jp{shc�p�̃��[����`�Ǎ���}
include $(KERNEL_MAKINC_DIR)/shc_r.inc



# --------------------------------------
#  %jp{�ˑ��֌W}
# --------------------------------------

$(OBJS_DIR)/sample.obj: ../sample.c ../kernel_id.h


# end of file