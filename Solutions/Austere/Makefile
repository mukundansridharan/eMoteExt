#Makefile for building extensions to Emote OS
#Author: Mukundan Sridharan
#The Samraksh Company

#on windows, install mingw or cygwin or WSL to get make

############## Modify this section to suite your needs ########################
#Setup your tool chain dir

GCC_VER=GCC5.4
GCC_DIR=GCC5.4.1
TOOL_DIR=../../Compilers/$(GCC_DIR)
SOLUTION=Austere
BUILD=Debug
FLAVER=FLASH

OUTDIR=BuildOutput/$(BUILD)/bin
NEWLIB=../../GCCLibs/GCC5.4/newlib/armv7-m
MISCTOOLS=../../Tools

########Boilerplate begin
%.obj: %.cpp
	$(CXX) $(CFLAGS) $(INC) -o $@ -c $^ 
%.obj: %.cc
	$(CXX) $(CFLAGS) $(INC) -o $@ -c $^ 
%.obj: %.c
	$(CC) $(CFLAGS) $(INC) -o $@ -c $^ 
########Boilerplate end


#####This is the library you are adding, it will create a library by name "ext.lib"####
DLLDIR=../../Extensions/SMO/SMO/Stubs
DLLOBJS=$(DLLDIR)/SMO.obj $(DLLDIR)/SMO_InferenceEngine_SMO.obj $(DLLDIR)/SMO_InferenceEngine_SMO_mshl.obj


all: binary 

ext.lib: $(DLLOBJS)
	$(AR) -rcs $@ $(DLLOBJS)

############## Dont modify the rest unless you know what you are doing####################

MKDIR=mkdir -p
CXX="$(TOOL_DIR)/bin/arm-none-eabi-g++.exe"
CC="$(TOOL_DIR)/bin/arm-none-eabi-gcc.exe"
AR="$(TOOL_DIR)/bin/arm-none-eabi-ar.exe"
LD="$(TOOL_DIR)/bin/arm-none-eabi-ld.exe"
OBJCOPY="$(TOOL_DIR)/bin/arm-none-eabi-objcopy.exe"
NM="$(TOOL_DIR)/bin/arm-none-eabi-nm.exe"

LIBS=../../Libs/THUMB2/$(GCC_VER)/le

LIBS_SOLUTION=$(LIBS)/$(FLAVER)/$(BUILD)/$(SOLUTION)/lib
LIBS_ANY= Core.lib CLRStartup.lib CorLib.lib SPOT.lib SecurityPkcs11.lib SPOT_Update.lib Debugger.lib Debugger_full.lib Graphics_stub.lib Hardware.lib SPOT_Hardware.lib IOPort.lib Messaging.lib SPOT_Messaging_stub.lib Diagnostics.lib I2C.lib InterruptHandler.lib SPOT_Hardware_SerialPort.lib CRC.lib HeapPersistence_stub.lib RPC_stub.lib Serialization_stub.lib SPOT_Serialization_stub.lib WireProtocol.lib Debugger_full.lib 
LIBS_ANY_DIR= $(LIBS)/ANY_MEDIA/$(BUILD)/lib
IAT_DIR=$(LIBS)/$(FLAVER)/$(BUILD)/$(SOLUTION)/obj/Solutions/$(SOLUTION)/TinyCLR


#AM_LIBLIST := $(wildcard $(LIBS_ANY_DIR)/*.lib)
AM_LIBLIST := $(foreach f,$(LIBS_ANY), $(LIBS_ANY_DIR)/$(f))
OBJS_CLRSTART=$(LIBS)/$(FLAVER)/$(BUILD)/$(SOLUTION)/obj/Solutions/$(SOLUTION)/TinyCLR

INC=-I../../Include/DeviceCode -I../../Include/CLR -I../../Include/Support -I../../Include/SMONative -I$(LIBS_ANY_DIR) -I.

CFLAGS	= -xc++ -std=c++11 -D_ISOC99_SOURCE -Wno-invalid-offsetof -fcheck-new  -fno-threadsafe-statics   -DSAMRAKSH_CUSTOM_NEWLIB -mslow-flash-data -fdata-sections -ffunction-sections -fdollars-in-identifiers -fno-exceptions -mcpu=cortex-m3 -funsigned-char -mstructure-size-boundary=8  -DTINYCLR_ENABLE_SOURCELEVELDEBUGGING  -mthumb -DCOMPILE_THUMB2 -Wno-psabi -DVERSION_MAJOR=4 -DVERSION_MINOR=3 -DVERSION_BUILD=0 -DVERSION_REVISION=0  -DMF_VERSION="\"4.3.1.0\"" -DOEMSYSTEMINFOSTRING="\"Samraksh Copyright (C) 2015 Samraksh and Microsoft.\"" -DPLATFORM_ARM_STM32F10x -DPLATFORM_ARM_AUSTERE -DTARGETLOCATION_FLASH  -DSAM_APP_TINYCLR  -DLITTLE_ENDIAN -DGCC    -DSAM_STACK_BOTTOM=0x20000000  -DSAM_STACK_TOP=0x20001000  -DSAM_HEAP_BEGIN=0x2000F5AC  -DSAM_HEAP_END=0x20017DF8  -DSAM_CUSTOM_HEAP_BEGIN=0x20017E00  -DSAM_CUSTOM_HEAP_END=0x20017FF8  -DSAM_PROFILER_BUFFER_BEGIN=0x20017F00  -DSAM_PROFILER_BUFFER_END=0x20017FF8    -mfloat-abi=soft    -mlittle-endian  -g3 -Og -DSAM_FORCE_GCC_CMDLINE_OPTS -DDEBUG  -D_DEBUG    

.phony: $(OUTDIR) $(OUTDIR)/tinyclr.bin $(OUTDIR)/tinyclr.hex

outputdir: $(OUTDIR) 

$(OUTDIR):
	$(MKDIR) "$(OUTDIR)"

$(OUTDIR)/tinyclr.bin:
	$(shell mkdir "$(OUTDIR)/tinyclr.bin")

$(OUTDIR)/tinyclr.hex:
	$(shell mkdir "$(OUTDIR)/tinyclr.hex")
	

InteropAssembliesTable.lib: $(IAT_DIR)/CLR_RT_InteropAssembliesTable.obj $(IAT_DIR)/CLR_RT_RuntimeMemory.obj
	$(AR) -rcs $@ $(IAT_DIR)/CLR_RT_InteropAssembliesTable.obj $(IAT_DIR)/CLR_RT_RuntimeMemory.obj
	
linker: outputdir ext.lib InteropAssembliesTable.lib
	$(LD)  -static --gc-sections --no-warn-mismatch --library-path=$(LIBS_SOLUTION) --library-path=$(LIBS_ANY_DIR) --allow-multiple-definition -u ARM_Vectors -u _start --Map $(OUTDIR)/tinyclr.map --output=$(OUTDIR)/tinyclr.axf --script=tinyclr_scatterfile.ldf $(OBJS_CLRSTART)/tinyclr_dat.obj $(OBJS_CLRSTART)/allocator.obj $(OBJS_CLRSTART)/tinyclr.obj -\( $(AM_LIBLIST) $(LIBS_SOLUTION)/static_tinyclr.lib InteropAssembliesTable.lib ext.lib -\) -\( $(NEWLIB)/libm.a $(NEWLIB)/libc.a $(TOOL_DIR)/lib/gcc/arm-none-eabi/5.4.1/armv7-m/libgcc.a -\)
  
binary: linker $(OUTDIR)/tinyclr.bin $(OUTDIR)/tinyclr.hex
	$(OBJCOPY) -S -j ER_FLASH -j ER_IRAM_TC -j ER_FLASH_CONT -j ER_RAM_RO -j ER_RAM_RW  -O binary $(OUTDIR)/tinyclr.axf $(OUTDIR)/tinyclr.bin/ER_FLASH
	$(OBJCOPY) -S -j ER_DAT -O binary $(OUTDIR)/tinyclr.axf $(OUTDIR)/tinyclr.bin/ER_DAT
	$(OBJCOPY) -S -j ER_CONFIG -O binary $(OUTDIR)/tinyclr.axf $(OUTDIR)/tinyclr.bin/ER_CONFIG
	$(OBJCOPY) -S -j ER_FLASH -j ER_IRAM_TC -j ER_FLASH_CONT -j ER_RAM_RO -j ER_RAM_RW  -j ER_IRAM -O srec --gap-fill 0 --srec-forceS3  $(OUTDIR)/tinyclr.axf $(OUTDIR)/tinyclr.hex/ER_FLASH
	$(OBJCOPY) -S -j ER_DAT -O srec --srec-forceS3  $(OUTDIR)/tinyclr.axf $(OUTDIR)/tinyclr.hex/ER_DAT
	$(OBJCOPY) -S -j ER_CONFIG -O srec --srec-forceS3  $(OUTDIR)/tinyclr.axf $(OUTDIR)/tinyclr.hex/ER_CONFIG
	$(OBJCOPY) -j ER_IFLASH -O srec --srec-forceS3  $(OUTDIR)/tinyclr.axf $(OUTDIR)/tinyclr.hex/ER_IFLASH
	$(NM) $(OUTDIR)/tinyclr.axf >> $(OUTDIR)/tinyclr.dump
	$(MISCTOOLS)/CreateSymdef.exe     $(OUTDIR)/tinyclr.dump $(OUTDIR)/tinyclr.symdefs
	
	
clean:
	rm -rf BuildOutput/$(BUILD) $(IAT_DIR)/CLR_RT_InteropAssembliesTable.obj $(LIBS_SOLUTION)/InteropAssembliesTable.lib InteropAssembliesTable.lib ext.lib $(DLLOBJS)
