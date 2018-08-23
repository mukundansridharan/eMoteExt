# eMoteExt Repository

# Overview
This repo deals with extending the eMote OS. The basic idea is that, Samraksh will build part of the solution and will populate this repo with binary libraries and all header files that go with it. The users of this repo can add whatever stuff they want, native and c#, and can complete the build process (linking and executable creation) to create custom versions of emote OS. All libraries are static. Also, there is very little dynamic memory allocation in eMote, hence do not use 'new' or 'malloc' on the native side code, unless you know exactly what you are doing.

# Details
The build process here uses a simple makefile based method to create extensions outside of MicroFramework4.3 (on which the eMote is based) source tree and uses binaries of libraries built by Samraksh. If you are interested in building an extension for a solution/platform/build type for which you dont find the necessary binaries, contact someone at Samraksh to get the binaries checkin into the repo.

Note to folks who update binaries: The idea is that the binaries would tract the master branch of Sarmaksh's repo and hence can change at anytime. Make sure you are updating only binaries of the solution/build type that you intend to. DONOT copy paste your entire BuildOuputs directory to this repo. That will overwrite stuff that you didnt intend.

# Build Instructions
- Install Visual Studio Community 2013. Install MF SDK 4.3. 
- You will need 'make'. So you need to install mingw or cygwin or WSL or somesuch thing on Windows. The Makefile for compilation is written and tested only under mingw32, but should work with very minor changes with other types of linux emulation.
	- If there is no 'make' in your mingw32/64 installation, get it from https://sourceforge.net/projects/ezwinports/files/make-4.2.1-without-guile-w32-bin.zip/download
- Create a C# Library project and write a interop
	- Use the decoreation "[MethodImplAttribute(MethodImplOptions.InternalCall)]" for methods that need to be implemented on the native side
	- Generate the native stubs: Go to project properties > .Net MicroFramework > And check the "Generate native stubs for internal methods" check box
	- Make sure everytime you regenerate the c# dll, you regenerate the stubs. This is important since a hash is generated and put in the native part that corresponds to the c# dll.
- Add you interop's call methods to the global interop table
	- Open the CLR_RT_InteropAssembliesTable.cpp that corresponds to your Compiler,Solution,Memory and Build type. For example for Compiler=GCC5.4,Solution=EmoteDotNow,Memory=FLASH, and Build=debug, the file will be at eMoteExt\Libs\THUMB2\GCC5.4\le\FLASH\debug\EmoteDotNow\obj\Solutions\EmoteDotNow\TinyCLR 
	- Add a extern declration to your CLR_RT_NativeAssemblyData member declaration(you will find this in your stubs directory's main .h file, it will be called something like "g_CLR_AssemblyNative_ExampleLib")
	- Add the the member to the *g_CLR_InteropAssembliesNativeData array, just before the NULL on the last line.
- Implement your native part by filling out the stubbed out C methods.

- Modify the Makefile under the Solutions/"solution_name". Makefile already has code to compile a  example extension called "ExampleLib", just replace/add ExampleLib with the name of the extension you are creating.
- compile by typing make.
- Create a test C# App that uses the C# dll.
- Load using jtag and test!!

# Common Build Errors:

- If you get a error like "../../Compilers/GCC5.4.1/bin/arm-none-eabi-ld.exe: unrecognized option '-\('", its probably because of older version of make. GNU Make versions less than 4.0 seems to throw this error. Use the link above to update your make to version 4.2
	
