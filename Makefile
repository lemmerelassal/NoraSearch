# Makefile for GCC / MSYS2

EXTENSION = my_extension
MODULES = my_extension
DATA = my_extension--1.0.sql

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)

# Force GCC
CC = gcc
CFLAGS += -Wall -O2 \
          -DWIN32 \
          -D_WINDOWS \
          -D__WINDOWS__ \
          -D__WIN32__ \
          -D_CRT_SECURE_NO_DEPRECATE \
          -D_CRT_NONSTDC_NO_DEPRECATE
