TEMPLATE = subdirs

SUBDIRS = watchwindow stablepoint featuresorter measurebehavior

QMAKE_EXTRA_TARGETS += run-test
win32 {
run-test.commands = \
    if not exist "reports" mkdir reports & \
    cd watchwindow && $(MAKE) run-test & \
    cd stablepoint && $(MAKE) run-test & \
    cd featuresorter && $(MAKE) run-test & \
    cd measurebehavior && $(MAKE) run-test
} else:linux {
run-test.commands = \
    [ -e "reports" ] || mkdir reports ; \
    cd watchwindow && $(MAKE) run-test ; \
    cd stablepoint && $(MAKE) run-test ; \
    cd featuresorter && $(MAKE) run-test ; \
    cd measurebehavior && $(MAKE) run-test
}
