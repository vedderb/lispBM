
include $(LISPBM)/src/lispbm.mk

LISPBM_SRC = $(LISPBM_BASE_SRC)

ifeq ($(LISPBM_USE_COMPRESSION), )
LISPBM_SRC += $(LISPBM_COMPRESSION_SRC)
endif

ifeq ($(LISPBM_USE_EC_EVAL), )
LISPBM_SRC += $(LISPBM_EC_EVAL_SRC)
endif

ifeq ($(LISPBM_USE_EVAL_CPS), )
LISPBM_SRC += $(LISPBM_EVAL_CPS_SRC)
endif

ifeq ($(LISPBM_USE_PRELUDE), )
LISPBM_SRC += $(LISPBM_PRELUDE_SRC)
endif

ifeq ($(LISPBM_USE_EXTENSIONS), )
LISPBM_SRC += $(LISPBM_EXTENSIONS_SRC)
endif 
