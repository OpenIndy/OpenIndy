OiDefaultPlugin
================

This repository includes a default plugin for [OpenIndy](https://github.com/OpenIndy/OpenIndy). The instructions for creating plugins is described in the [OiPluginTemplate wiki](https://github.com/OpenIndy/OiPluginTemplate/wiki).

IDE
----
OpenIndy is developed with the Qt framework (Qt libs + Qt Creator IDE). You can download the framework [here](http://qt-project.org/downloads).


build
-----
The easiest way to build OpenIndy plugins is to use the Qt Creator. You can build OpenIndy plugins in `debug` or `release` mode.  

First build the openIndyLib. It includes functionalities such as linear algebra algorithmen.

- Qt Creator ----> `OpenIndy\lib\openIndyLib`

Then develop and build the default plugin. 

- Qt Creator ----> `OpenIndy\plugins\OiDefaultPlugin\oiDefaultPlugin.pro`
- If the build was successful, you will find your plugin under `OpenIndy\plugins\OiDefaultPlugin\bin/debug` or `OpenIndy\plugins\OiDefaultPlugin\bin/release`

Now you can use your Plugin in [OpenIndy](https://github.com/OpenIndy/OpenIndy).

plugin debugger
----
you can use the /pluginDebugger/PluginDebugger.pro project to debug your plugin classes. Just add your classes and write a unit test. 
Here you can find an example:

- [OiTest testFit.h](https://github.com/OpenIndy/OiPluginTemplate/blob/master/pluginDebugger/oitestTestFit.h)
- [OiTest testFit.cpp](https://github.com/OpenIndy/OiPluginTemplate/blob/master/pluginDebugger/oitestTestFit.cpp)




