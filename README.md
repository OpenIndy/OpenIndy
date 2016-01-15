OpenIndy
========

OpenIndy is a metrology software solution that can be extended by [plugins](https://github.com/OpenIndy/OiPluginTemplate). The project started in 2013 as a student project in the Department of Geoinformatics and Surveying ([HS Mainz](https://www.hs-mainz.de/)).

IDE
----

OpenIndy is developed with the Qt framework (Qt libs + Qt Creator IDE). You can download the framework [here](http://qt-project.org/downloads).

Dependencies
------------

- [OpenIndy-Core](https://github.com/OpenIndy/OpenIndy-Core)
- [Qt](http://qt-project.org)
- [sqlite](https://sqlite.org)

Build
-----

The easiest way to build OpenIndy is to use the [Qt Creator](http://www.qt.io/download/). You can build OpenIndy in `debug` or `release` mode.
We recommend the msvc 2013 compiler (to get it simply install visual studio express).
In the project settings in Qt-Creator add an additional make step -> make install.

After the build is finished copy the database (/db/oisystemdb.sqlite) to the executable (/bin/debug or /bin/release)
