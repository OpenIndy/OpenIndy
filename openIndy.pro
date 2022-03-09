CONFIG += c++11

TEMPLATE = subdirs

SUBDIRS = \
    core \
    app \
    test

# project locations
app.subdir = build
core.subdir = lib/OpenIndy-Core
test.subdir = test

# project dependencies
app.depends = core
test.depends = app

QMAKE_EXTRA_TARGETS += run-test
run-test.commands = \
    cd $$test.subdir && $(MAKE) run-test
#
