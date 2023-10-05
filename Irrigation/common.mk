

CXXFLAGS        = -std=gnu++11 -Wall -fmessage-length=0 -MMD -MP 

# DEBUG flags
#CXXFLAGS        += -O0 -g3

# RELEASE flags
CXXFLAGS        += -O3

INCLUDES        += -isystem $(PROJECT_ROOT)/3p

SENDINBLUE_SMTP_KEY = $(shell printenv SENDINBLUE_SMTP_KEY)

ifndef SENDINBLUE_SMTP_KEY
$(error Error! SENDINBLUE_SMTP_KEY is not set)
endif
