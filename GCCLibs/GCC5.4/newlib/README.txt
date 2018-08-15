A custom built newlib. Warning, takes forever. The real magic is "MALLOC_PROVIDED". Everything else is the same so headers match.

Configure call:
../configure \
	--target=arm-none-eabi \
	--enable-newlib-io-long-long \
	--enable-newlib-register-fini \
	--disable-newlib-supplied-syscalls \
	--disable-nls \
	--host=arm-none-eabi \
	CC=arm-none-eabi-gcc \
	CFLAGS="-Os -ffunction-sections -fdata-sections -DMALLOC_PROVIDED"
	
