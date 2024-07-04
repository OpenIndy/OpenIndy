CONFIG += c++11

TEMPLATE = subdirs

SUBDIRS = \
    core \
    app \
    appui \
    test

# project locations
app.subdir = build
appui.subdir = appui
core.subdir = lib/OpenIndy-Core
test.subdir = test

# project dependencies
appui.depends = core
app.depends = appui
test.depends = app

QMAKE_EXTRA_TARGETS += run-test
run-test.commands = \
    cd $$test.subdir && $(MAKE) run-test
#
