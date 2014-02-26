OpenIndy
========

OpenIndy is a metrology software solution that can be extended by [plugins](https://github.com/OpenIndy/OiPluginTemplate). The project started in 2013 as a student project in the Department of Geoinformatics and Surveying ([FH Mainz](https://www.fh-mainz.de/technology/geoinformatics-and-surveying/index.html)). Our primary goal is to attract students and to jointly develop and learn. For more information take a look at the [OpenIndy Wiki](https://github.com/OpenIndy/OpenIndy/wiki).

IDE
----
OpenIndy is developed with the Qt framework (Qt libs + Qt Creator IDE). You can download the framework [here](http://qt-project.org/downloads).


build
-----
The easiest way to build OpenIndy is to use the Qt Creator. You can build OpenIndy in `debug` or `release` mode.  

First build the openIndyLib. It includes functionalities such as linear algebra algorithmen.

- Qt Creator ----> `lib/openIndyLib/openIndyLib.pro`

Then build OpenIndy and copy the dependencies. 

- Qt Creator ----> `openindy.pro`
- Copy the openIndyLib (`lib/openIndyLib/bin/debug` || `lib/openIndyLib/bin/release`) and the oisystemdb.sqlite (`db/`) to `bin/debug` || `bin/release`. On Mac OS, you can find the binaries under `openindy/contents/MacOs`
- Include LAPACK/BLAS


BLAS and LAPACK
----------------

If using Linux:

  - Use the Terminal (command line): `sudo apt-get install liblapack-dev`

If using Windows:

  - Copy all dll's from `lib/armadillo-3.910.0/examples/lib_win64` to `bin/debug` || `bin/release`

If using Mac OS:

  - Copy `lib/armadillo-3.910.0/examples/framework_mac/Accelerate.framework` to the Mac OS Library `/Library/Frameworks`
  

Default Plugin
---------------
You can find the default plugin under `plugins/OiDefaultPlugin`. After you've built the plugin you can embed it via the GUI (`Plugin -> load plugins`). Also you can find a template for a plugin in the folder, which you can copy and start developing your own plugin.




