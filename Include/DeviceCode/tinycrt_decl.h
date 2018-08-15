////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRIVERS_TINYCRT_DECL_H_
#define _DRIVERS_TINYCRT_DECL_H_ 1

//--//

#ifdef GCC
int hal_printf( const char* format, ... )
	__attribute__ ((format (printf, 1, 2)));

int hal_vprintf( const char* format, va_list arg )
	__attribute__ ((format (printf, 1, 0)));

int hal_fprintf( COM_HANDLE stream, const char* format, ... )
	__attribute__ ((format (printf, 2, 3)));

int hal_vfprintf( COM_HANDLE stream, const char* format, va_list arg )
	__attribute__ ((format (printf, 2, 0)));

int hal_snprintf( char* buffer, size_t len, const char* format, ... )
	__attribute__ ((format (printf, 3, 4)));

#else // #ifdef GCC

int hal_printf( const char* format, ... );
int hal_vprintf( const char* format, va_list arg );
int hal_fprintf( COM_HANDLE stream, const char* format, ... );
int hal_vfprintf( COM_HANDLE stream, const char* format, va_list arg );
int hal_snprintf( char* buffer, size_t len, const char* format, ... );
#endif //#ifdef GCC

#ifdef GCC
#if !defined(PLATFORM_EMULATED_FLOATINGPOINT)
int hal_snprintf_float( char* buffer, size_t len, const char* format, float f )
	__attribute__ ((format (printf, 3, 0)));
int hal_snprintf_double( char* buffer, size_t len, const char* format, double d )
	__attribute__ ((format (printf, 3, 0)));

#else

int hal_snprintf_float( char* buffer, size_t len, const char* format, INT32 f )
	__attribute__ ((format (printf, 3, 0)));
int hal_snprintf_double( char* buffer, size_t len, const char* format, INT64& d )
	__attribute__ ((format (printf, 3, 0)));
#endif

#else // #ifdef GCC

#if !defined(PLATFORM_EMULATED_FLOATINGPOINT)
int hal_snprintf_float( char* buffer, size_t len, const char* format, float f );
int hal_snprintf_double( char* buffer, size_t len, const char* format, double d );

#else

int hal_snprintf_float( char* buffer, size_t len, const char* format, INT32 f );
int hal_snprintf_double( char* buffer, size_t len, const char* format, INT64& d );
#endif
#endif  //#ifdef GCC


#if defined(GCC_OLD_VA_LIST)
int hal_vsnprintf( char* buffer, size_t len, const char* format, va_list arg ); 
!ERROR
// We need to force the symbol name of the next function to match RVDS one. This is needed for proper linking to the RVDS precompiled libraries
int hal_vsnprintf( char* buffer, size_t len, const char* format, int* args ) asm("_Z13hal_vsnprintfPcjPKcSt9__va_list");
#else
#ifdef GCC
int hal_vsnprintf( char* buffer, size_t len, const char* format, va_list arg )
	__attribute__ ((format (printf, 3, 0)));
#else // #ifdef GCC
int hal_vsnprintf( char* buffer, size_t len, const char* format, va_list arg );
#endif // #ifdef GCC
#endif

#if defined(PLATFORM_ARM) || defined(PLATFORM_SH)
#define printf     DoNotUse_*printf []
#define sprintf    DoNotUse_*printf []
#define fprintf    DoNotUse_*printf []

#define _printf    DoNotUse_*printf []
#define _sprintf   DoNotUse_*printf []
#define _fprintf   DoNotUse_*printf []

#define snprintf   DoNotUse_*printf []
#define vsnprintf  DoNotUse_*printf []

#define _snprintf  DoNotUse_*printf []
#define _vsnprintf DoNotUse_*printf []
#define _vsnprintf_r DoNotUse_*printf []
#define _svfprintf_r DoNotUse_*printf []
#define _svfiprintf_r DoNotUse_*printf []

#define strcpy    DoNotUse_*strcpy  []
#define strncpy   DoNotUse_*strcpy  []
#define strlen    DoNotUse_*strlen  []
#define strncmp   DoNotUse_*strncmp  []


int hal_strcpy_s ( char* strDst, size_t sizeInBytes, const char* strSrc );
int hal_strncpy_s( char* strDst, size_t sizeInBytes, const char* strSrc, size_t count );
size_t hal_strlen_s (const char * str);
int hal_strncmp_s( const char* str1, const char* str2, size_t num );

#elif defined(PLATFORM_WINDOWS) || defined(PLATFORM_WINCE)

#define hal_strcpy_s(strDst, sizeInBytes, strSrc ) strcpy_s(strDst, sizeInBytes, strSrc)
#define hal_strncpy_s(strDst, sizeInBytes, strSrc, count ) strncpy_s(strDst, sizeInBytes, strSrc, count)
#define hal_strlen_s( str ) strlen(str)
#define hal_strncmp_s( str1, str2, num ) strncmp(str1, str2, num)

#elif defined(PLATFORM_BLACKFIN) 

int hal_strcpy_s( char* strDst, size_t sizeInBytes, const char* strSrc ); 
int hal_strncpy_s( char* strDst, size_t sizeInBytes, const char* strSrc, size_t count );
#define hal_strlen_s( str ) strlen(str)
#define hal_strncmp_s( str1, str2, num ) strncmp(str1, str2, num)



#else
!ERROR
#endif


// Compares 2 ASCII strings case insensitive. Always defined in our code ( tinycrt.cpp )
int hal_stricmp( const char * dst, const char * src );

// For Windows we default to c-runtime impelementation. All other platforms come from use tinycrt.cpp
#if defined(PLATFORM_WINDOWS) || defined(PLATFORM_WINCE)
#define hal_stricmp _stricmp
#endif

//--//

#endif // _DRIVERS_TINYCRT_DECL_H_
