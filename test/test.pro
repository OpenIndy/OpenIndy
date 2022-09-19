TEMPLATE = subdirs

SUBDIRS = watchwindow stablepoint featuresorter measurebehavior dialogs

QMAKE_EXTRA_TARGETS += run-test
win32-msvc* {
run-test.commands = \
    if not exist reports mkdir reports & if not exist reports exit 1 $$escape_expand(\n\t)\
    cd $$shell_path($$OUT_PWD/watchwindow) && $(MAKE) run-test & \
    cd $$shell_path($$OUT_PWD/stablepoint) && $(MAKE) run-test & \
    cd $$shell_path($$OUT_PWD/featuresorter) && $(MAKE) run-test & \
    cd $$shell_path($$OUT_PWD/measurebehavior) && rem $(MAKE) run-test & \
    cd $$shell_path($$OUT_PWD/dialogs) && $(MAKE) run-test
} else:win32-g++ {
run-test.commands = \
    [ -e "reports" ] || mkdir reports ; \
    $(MAKE) -C $$shell_path($$OUT_PWD/watchwindow) run-test & \
    $(MAKE) -C $$shell_path($$OUT_PWD/featuresorter) run-test & \
    $(MAKE) -C $$shell_path($$OUT_PWD/stablepoint) run-test & \
    $(MAKE) -C $$shell_path($$OUT_PWD/measurebehavior) run-test & \
    $(MAKE) -C $$shell_path($$OUT_PWD/dialogs) run-test
} else:linux {
run-test.commands = \
    [ -e "reports" ] || mkdir reports ; \
    $(MAKE) -C watchwindow run-test ; \
    $(MAKE) -C stablepoint run-test ; \
    $(MAKE) -C featuresorter run-test ; \
    $(MAKE) -C measurebehavior run-test ; \
    $(MAKE) -C dialogs run-test
}
