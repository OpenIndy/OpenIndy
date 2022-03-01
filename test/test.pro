TEMPLATE = subdirs

SUBDIRS = watchwindow stablepoint featuresorter measurebehavior

QMAKE_EXTRA_TARGETS += run-test
win32 {
run-test.commands = \
    if not exist "reports" (mkdir reports & echo create reports) else (echo reports exists & \
    cd $$shell_path($$OUT_PWD/watchwindow) && $(MAKE) run-test & \
    cd $$shell_path($$OUT_PWD/stablepoint) && $(MAKE) run-test & \
    cd $$shell_path($$OUT_PWD/featuresorter) && $(MAKE) run-test & \
    cd $$shell_path($$OUT_PWD/measurebehavior) && $(MAKE) run-test
} else:linux {
run-test.commands = \
    [ -e "reports" ] || mkdir reports ; \
    $(MAKE) -C watchwindow run-test ; \
    $(MAKE) -C stablepoint run-test ; \
    $(MAKE) -C featuresorter run-test ; \
    $(MAKE) -C measurebehavior run-test
}
