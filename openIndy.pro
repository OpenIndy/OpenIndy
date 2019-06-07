CONFIG += c++11

TEMPLATE = subdirs

SUBDIRS = \
    core \
    app

# project locations
app.subdir = $$PWD/build
core.subdir = $$PWD/lib/OpenIndy-Core

# project dependencies
app.depends = core
