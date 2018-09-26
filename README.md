# eMoteExt Repository (branch CMSIS)

# Overview
This branch is aimed at creating C# interops corresponging to [CMSIS Version 5](https://github.com/ARM-software/CMSIS_5) [DSP](http://www.keil.com/pack/doc/CMSIS/DSP/html/index.html) and [NN](http://www.keil.com/pack/doc/CMSIS/NN/html/index.html) functions. The target application is a C# implementation of [FastGRNN](https://adityakusupati.github.io/links/FastGRNN.html) for the Austere .NOW, that uses these native libraries for linear algebra and neural network operations.

For general instructions on how to build interops, kindly check the master [README](https://github.com/MukundanAtSamraksh/eMoteExt/blob/master/README.md).

# Roadmap
- [C# FastGRNN PC application](https://github.com/MukundanAtSamraksh/eMoteExt/tree/cmsis/Support/FastGRNN), written by [Pranshu Jain](https://github.com/pranshu93).
- [FastGRNN eMote app](https://github.com/MukundanAtSamraksh/eMoteExt/tree/cmsis/Apps/FastGRNN)
- [CMSIS interops](https://github.com/MukundanAtSamraksh/eMoteExt/tree/cmsis/Extensions/MathFuncs)
- Interops test apps:
	- [Core functions test] (https://github.com/MukundanAtSamraksh/eMoteExt/tree/cmsis/Extensions/MathFuncsTest)
	- [Operator overloading test] (https://github.com/MukundanAtSamraksh/eMoteExt/tree/cmsis/Extensions/MathOpsOverloadingTest)
- [CMSIS native](https://github.com/MukundanAtSamraksh/eMoteExt/tree/cmsis/CMSIS_5)
