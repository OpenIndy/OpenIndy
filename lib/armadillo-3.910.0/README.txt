=== Contents ===

1: Introduction

2: Citation Details

3: Installation
   3.0: Preliminaries
   3.1: Installation on Linux and Mac OS X
   3.2: Manual Installation / Installation on Windows

4: Compiling Programs and Linking
   4.0: Examples
   4.1: Compiling & Linking on Linux and Mac OS X
   4.2: Compiling & Linking on Windows

5: Support for high-speed BLAS & LAPACK replacements
   5.0: Support for OpenBLAS, Intel MKL and AMD ACML
   5.1: Support for ATLAS

6: Documentation / API Reference Manual

7: FAQs and Bug Reports

8: Developers and Contributors

9: License




=== 1: Introduction ===

Armadillo is a C++ linear algebra library (matrix maths)
aiming towards a good balance between speed and ease of use.
The syntax is deliberately similar to Matlab.

Integer, floating point and complex numbers are supported,
as well as a subset of trigonometric and statistics functions.
Various matrix decompositions are provided through optional
integration with LAPACK or high-performance LAPACK-compatible
libraries (such as Intel MKL or AMD ACML).

A delayed evaluation approach is employed (during compile time)
to combine several operations into one and reduce (or eliminate)
the need for temporaries. This is accomplished through recursive
templates and template meta-programming.

This library is useful for conversion of research code into
production environments, or if C++ has been decided as the
language of choice, due to speed and/or integration capabilities.

The library is open-source software, and is distributed under a
license that is useful in both open-source and commercial/proprietary
contexts. 

Armadillo is primarily developed at NICTA (Australia),
with contributions from around the world.  More information
about NICTA can be obtained from http://nicta.com.au



=== 2: Citation Details ===

Please cite the following tech report if you use Armadillo in your
research and/or software. Citations are useful for the continued
development and maintenance of the library.

  Conrad Sanderson.
  Armadillo: An Open Source C++ Linear Algebra Library for
  Fast Prototyping and Computationally Intensive Experiments.
  Technical Report, NICTA, 2010.



=== 3.0: Installation: Preliminaries ===

Armadillo makes extensive use of template meta-programming,
recursive templates and template based function overloading.
As such, C++ compilers which do not fully implement the C++
standard may not work correctly.

The functionality of Armadillo is partly dependent on other
libraries: mainly LAPACK and BLAS. Armadillo can work without
LAPACK or BLAS, but its functionality will be reduced.
In particular, basic functionality will be available
(eg. matrix addition and multiplication), but things like
eigen decomposition or matrix inversion will not be.
Matrix multiplication (mainly for big matrices) may not be as fast.

* For automatic installation on Linux and Mac OS X systems,
  see section 3.1. This installation is also likely to work on
  other Unix-like systems, such as FreeBSD, NetBSD, OpenBSD,
  Solaris, CygWin, etc.
  
* For manual installation and/or installation on Windows,
  see section 3.2.
  
* If you have a previous version of Armadillo already installed,
  we recommend removing it before installing a newer version.



=== 3.1: Installation on Linux and Mac OS X ===

You can use the manual installation process as described in
section 3.2, or the following CMake based automatic installation.

* Step 1:
  If CMake is not already be present on your system, download
  it from http://www.cmake.org
  
  On major Linux systems (such as Fedora, Ubuntu, Debian, etc),
  cmake is available as a pre-built package, though it may need
  to be explicitly installed (using a tool such as PackageKit,
  yum, rpm, apt, aptitude).
  
* Step 2:
  If you have BLAS and/or LAPACK, install them before installing
  Armadillo. Under Mac OS X this is not necessary.
  
  On Linux systems it is recommended that the following libraries
  are present: LAPACK, BLAS, ATLAS and Boost. LAPACK and BLAS are
  the most important. If you have ATLAS and Boost, it's also necessary
  to have the corresponding header files installed.
  
  For best performance, we recommend using the multi-threaded OpenBLAS
  library instead of standard BLAS.  See http://xianyi.github.com/OpenBLAS/
  
* Step 3:
  Open a shell (command line), change into the directory that was
  created by unpacking the armadillo archive, and type the following
  commands:
  
  cmake .
  make 
  
  The full stop separated from "cmake" by a space is important.
  CMake will figure out what other libraries are currently installed
  and will modify Armadillo's configuration correspondingly.
  CMake will also generate a run-time armadillo library, which is a 
  combined alias for all the relevant libraries present on your system
  (eg. BLAS, LAPACK and ATLAS).
  
  If you need to re-run cmake, it's a good idea to first delete the
  "CMakeCache.txt" file (not "CMakeLists.txt").
  
* Step 4:
  If you have access to root/administrator/superuser privileges,
  first enable the privileges (eg. through "su" or "sudo")
  and then type the following command:
  
  make install
  
  If you don't have root/administrator/superuser privileges, 
  type the following command:
  
  make install DESTDIR=my_usr_dir
  
  where "my_usr_dir" is for storing C++ headers and library files.
  Make sure your C++ compiler is configured to use the sub-directories
  present within this directory.



=== 3.2: Manual Installation / Installation on Windows ===

The manual installation is comprised of 3 steps:

* Step 1:
  Copy the entire "include" folder to a convenient location
  and tell your compiler to use that location for header files
  (in addition to the locations it uses already).
  Alternatively, you can use the "include" folder directly.
  
* Step 2:
  Modify "include/armadillo_bits/config.hpp" to indicate 
  which libraries are currently available on your system.
  For example, if you have LAPACK and BLAS (or OpenBLAS) present, 
  uncomment the following lines:
  
  #define ARMA_USE_LAPACK
  #define ARMA_USE_BLAS
  
* Step 3:
  If you have LAPACK and/or BLAS present, configure your 
  compiler to link with these libraries. 
  
  If using Linux, link using -llapack -lblas
  If using Mac OS X, link using -framework Accelerate
  If using Windows, see section 4.2.
  
  You can also link with high-speed replacements for LAPACK and BLAS,
  such as OpenBLAS, or Intel MKL, or AMD ACML. See section 5 for more info.



=== 4.0: Compiling Programs and Linking: Examples ===

The "examples" directory contains several quick example programs
that use the Armadillo library. If Armadillo was installed manually
(ie. according to section 3.2), you will also need to explicitly
link your programs with the libraries that were specified in
"include/armadillo_bits/config.hpp".

"example1.cpp" may require the BLAS library or its equivalent.
"example2.cpp" requires the LAPACK library or its equivalent
(eg. the Accelerate framework on Mac OS X).

You may get errors at compile or run time if BLAS and/or LAPACK
functions are not available.

NOTE: As Armadillo is a template library, we recommended that
      optimisation is enabled during compilation. For example,
      for the GCC compiler use -O1 or -O2



=== 4.1: Compiling & Linking on Linux and Mac OS X ===

Please see "examples/Makefile", which may may need to be configured
for your system. If Armadillo header files were installed in a
non-standard location, you will need to modify "examples/Makefile"
to tell the compiler where they are.

In general, programs which use Armadillo are compiled along these lines:
  g++ example1.cpp -o example1 -O2 -larmadillo

(you may also need to specify the include directory via the -I switch)

If you get linking errors, or if Armadillo was installed manually
and you specified that LAPACK and BLAS are available, you will
need to explicitly link with LAPACK and BLAS (or their equivalents),
for example:
  g++ example1.cpp -o example1 -O2 -llapack -lblas

(you may also need to specify the library directory via the -L switch)

Notes:

  * under most Linux systems, using "-llapack -lblas" should be enough;
    however, on Ubuntu and Debian you may need to add "-lgfortran"
    
  * under Mac OS X, try "-framework Accelerate" or "-llapack -lblas"
    (the Accelerate option is usually the fastest)
    
  * under the Sun Studio compiler, try "-library=sunperf"



=== 4.2: Compiling & Linking on Windows ===

As a courtesy, we've provided pre-compiled 32 bit versions of
standard LAPACK and BLAS for Windows, as well as MSVC project
files to compile example1.cpp and example2.cpp.
The project files are stored in the following folders:
  examples/example1_win32
  examples/example2_win32

The standard 32 bit versions of the LAPACK and BLAS libraries
are stored in:
  examples/lib_win32

If you're getting messages such as "use of LAPACK needs to be enabled",
you will need to manually modify "include/armadillo_bits/config.hpp"
to enable the use of LAPACK. See section 3.2 for more information.

Note that on 64 bit systems (such as Windows 7), dedicated
64 bit versions of BLAS and LAPACK are considerably faster.
If you don't have a 64 bit BLAS library, it's better to
explicitly disable the use of BLAS by defining ARMA_DONT_USE_BLAS
before including the armadillo header:

#define ARMA_DONT_USE_BLAS
#include <armadillo>

The MSCV project files were tested on 32 bit Windows XP with
Visual C++ 2008 (Express Edition). You may need to make adaptations
for 64 bit systems, later versions of Windows and/or the compiler.
For example, you may have to enable or disable the ARMA_BLAS_LONG
and ARMA_BLAS_UNDERSCORE macros in "armadillo_bits/config.hpp".

The pre-compiled versions of LAPACK and BLAS were downloaded from:
  http://www.fi.muni.cz/~xsvobod2/misc/lapack/

If the provided libraries don't work for you, or you want more speed,
try these versions:
  http://software.intel.com/en-us/intel-mkl/
  http://developer.amd.com/tools-and-sdks/cpu-development/amd-core-math-library-acml/
  http://xianyi.github.com/OpenBLAS/
  http://www.stanford.edu/~vkl/code/libs.html
  http://icl.cs.utk.edu/lapack-for-windows/lapack/

The OpenBLAS, MKL and ACML libraries are generally the fastest.
See section 5 for more info on making Armadillo use these libraries.


You can find the original sources for standard BLAS and LAPACK at:
  http://www.netlib.org/blas/
  http://www.netlib.org/lapack/


We recommend the following high-quality compilers:

  * GCC (part MinGW)
    http://www.mingw.org/

  * GCC (part of CygWin)
    http://www.cygwin.com/

  * Intel C++ compiler
    http://software.intel.com/en-us/intel-compilers/

For the GCC compiler, use version 4.0 or later.
For the Intel compiler, use version 10.0 or later.

For best results we also recommend using an operating system
that's more reliable and more suitable for heavy duty work,
such as Mac OS X or various Linux-based systems:
Ubuntu:           http://www.ubuntu.com/
Debian:           http://www.debian.org/
Scientific Linux: http://www.scientificlinux.org/



=== 5.0: Support for OpenBLAS, Intel MKL and AMD ACML ===

Armadillo can use OpenBLAS, or Intel Math Kernel Library (MKL), or the
AMD Core Math Library (ACML) as high-speed replacements for BLAS and LAPACK.
Generally this just involves linking with the replacement libraries
instead of BLAS and LAPACK.

You may need to make minor modifications to "include/armadillo_bits/config.hpp"
in order to make sure Armadillo uses the same style of function names
as used by MKL or ACML. For example, the function names might be in capitals.

On Linux systems, MKL and ACML might be installed in a non-standard
location, such as /opt, which can cause problems during linking.
Before installing Armadillo, the system should know where the MKL or ACML
libraries are located. For example, "/opt/intel/mkl/lib/intel64/".
This can be achieved by setting the LD_LIBRARY_PATH environment variable,
or for a more permanent solution, adding the directory locations
to "/etc/ld.so.conf". It may also be possible to store a text file 
with the locations in the "/etc/ld.so.conf.d" directory.
For example, "/etc/ld.so.conf.d/mkl.conf".
If you modify "/etc/ld.so.conf" or create "/etc/ld.so.conf.d/mkl.conf",
you will need to run "/sbin/ldconfig" afterwards.

Example of the contents of "/etc/ld.so.conf.d/mkl.conf" on a RHEL 6 system,
where Intel MKL version 11.0.3 is installed in "/opt/intel":

/opt/intel/lib/intel64
/opt/intel/mkl/lib/intel64

The default installations of ACML 4.4.0 and MKL 10.2.2.025 are known 
to have issues with SELinux, which is turned on by default in Fedora
(and possibly RHEL). The problem may manifest itself during run-time,
where the run-time linker reports permission problems.
It is possible to work around the problem by applying an appropriate
SELinux type to all ACML and MKL libraries.

If you have ACML or MKL installed and they are persistently giving
you problems during linking, you can disable the support for them
by editing the "CMakeLists.txt" file, deleting "CMakeCache.txt" and
re-running the CMake based installation. Specifically, comment out
the lines containing:
  INCLUDE(ARMA_FindMKL)
  INCLUDE(ARMA_FindACMLMP)
  INCLUDE(ARMA_FindACML)



=== 5.1: Support for ATLAS ===

Armadillo can use the ATLAS library for faster versions of
certain LAPACK and BLAS functions. Not all ATLAS functions are
currently used, and as such LAPACK should still be installed.

The minimum recommended version of ATLAS is 3.8.
Old versions (eg. 3.6) can produce incorrect results
as well as corrupting memory, leading to random crashes.

Users of Ubuntu and Debian based systems should explicitly
check that version 3.6 is not installed.  It's better to
remove the old version and use the standard LAPACK library.



=== 6: Documentation / API Reference Manual ===

A reference manual (documentation of APIs) is available at

  http://arma.sourceforge.net/docs.html

and in the "docs.html" file in this archive,
which can be viewed with a web browser.

The documentation explains how to use Armadillo's
classes and functions, with snippets of example code.



=== 7: FAQs and Bug Reports ===

Answers to Frequently Asked Questions (FAQs) can be found at:

  http://arma.sourceforge.net/faq.html

This library has gone through extensive testing and
has been successfully used in production environments.
However, as with almost all software, it's impossible
to guarantee 100% correct functionality.

If you find a bug in the library (or the documentation),
we are interested in hearing about it. Please make a
_small_ _self-contained_ program which exposes the bug
and send the program source (as well as the bug description)
to the developers.  The developers' contact details are at:

  http://arma.sourceforge.net/contact.html



=== 8: Developers and Contributors ===

Main sponsoring organisation:
- NICTA
  http://nicta.com.au

Main developers:
- Conrad Sanderson - http://conradsanderson.id.au
- Ryan Curtin
- Ian Cullinan
- Dimitrios Bouzas
- Stanislav Funiak

Contributors:
- Matthew Amidon
- Eric R. Anderson
- Benoît Bayol
- Salim Bcoin
- Justin Bedo
- Evan Bollig
- Darius Braziunas
- Filip Bruman
- Ted Campbell
- James Cline
- Chris Cooper
- Clement Creusot
- Chris Davey
- Dirk Eddelbuettel
- Romain Francois
- Michael McNeil Forbes
- Piotr Gawron
- Charles Gretton
- Benjamin Herzog
- Edmund Highcock
- Szabolcs Horvat
- Friedrich Hust
- Kshitij Kulshreshtha
- Oka Kurniawan
- Simen Kvaal
- David Lawrence
- Jussi Lehtola
- Jeremy Mason
- Nikolay Mayorov
- Carlos Mendes
- Sergey Nenakhov
- Artem Novikov
- Martin Orlob
- Ken Panici
- Adam Piątyszek
- Jayden Platell
- Vikas Reddy
- Ola Rinta-Koski
- Boris Sabanin
- James Sanders
- Alexander Scherbatey
- Gerhard Schreiber
- Ruslan Shestopalyuk
- Shane Stainsby
- Petter Strandmark
- Eric Jon Sundstrom
- Paul Torfs
- Martin Uhrin
- Simon Urbanek
- Juergen Wiest
- Arnold Wiliem
- Yong Kang Wong
- Buote Xu
- Sean Young



=== 9: License ===

Unless specified otherwise, the Mozilla Public License v2.0 is used.
See the "LICENSE.txt" file for license details.

The file "include/armadillo_bits/fft_engine.hpp" is licensed under
both the Mozilla Public License v2.0 and a 3-clause BSD license.
See "include/armadillo_bits/fft_engine.hpp" for license details.


