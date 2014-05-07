OiPluginTemplate
================

This repository includes a template for an [OpenIndy](https://github.com/OpenIndy/OpenIndy) plugin. The instructions for creating your own plugins is described in the [OiPluginTemplate wiki](https://github.com/OpenIndy/OiPluginTemplate/wiki).

IDE
----
OpenIndy is developed with the Qt framework (Qt libs + Qt Creator IDE). You can download the framework [here](http://qt-project.org/downloads).


build
-----
The easiest way to build OpenIndy plugins is to use the Qt Creator. You can build OpenIndy plugins in `debug` or `release` mode.  

First build the openIndyLib. It includes functionalities such as linear algebra algorithmen.

- Qt Creator ----> `lib/openIndyLib/openIndyLib.pro`

Then develop and build your OpenIndy plugin. 

- Qt Creator ----> `OiTemplatePlugin.pro`
- If the build was successful, you will find your plugin under `bin/debug` or `bin/release`

Now you can use your Plugin in [OpenIndy](https://github.com/OpenIndy/OpenIndy).

fork and upstream
------------------
If you want to develop your plugin on github, then simply [fork](https://github.com/OpenIndy/OiPluginTemplate/wiki/Get-Started) this repo!

If you work with a private remote git repo or a repo of the [OpenIndy Organisation](https://github.com/OpenIndy) then you have to add an upstream.

`git remote add upstream https://github.com/OpenIndy/OiPluginTemplate.git`

Use following commands for updating your repo:

`git fetch upstream`

`git merge upstream/master`


