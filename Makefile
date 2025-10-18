# PostgreSQL extension Makefile for GCC / MinGW on Windows

EXTENSION = norasearch
MODULES = norasearch
DATA = norasearch--1.0.sql
OBJS = norasearch.o

# Use pg_config from MinGW installation (GCC build)
PG_CONFIG = /mingw64/bin/pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)

# Force GCC instead of MSVC
CC = gcc
CFLAGS += -Wall -O2

