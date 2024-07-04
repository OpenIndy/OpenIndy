TEMPLATE = subdirs

SUBDIRS = watchwindow featuresorter measurebehavior dialogs

QMAKE_EXTRA_TARGETS += run-test
win32-msvc* {
run-test.commands = \
    if not exist reports mkdir reports & if not exist reports exit 1 $$escape_expand(\n\t)\
    cd $$shell_quote($$OUT_PWD/watchwindow) && $(MAKE) run-test & \
    cd $$shell_quote($$OUT_PWD/featuresorter) && $(MAKE) run-test

contains( DEFINES, ENABLE_MULTITHREAD_TEST ) {
run-test.commangs += & \
    cd $$shell_quote($$OUT_PWD/measurebehavior) && $(MAKE) run-test
}

contains( DEFINES, ENABLE_GUI_TEST ) {
run-test.commangs += & \
    cd $$shell_quote($$OUT_PWD/dialogs) && $(MAKE) run-test
}

} else:win32-g++ {
run-test.commands = \
    [ -e "reports" ] || mkdir reports ; \
    $(MAKE) -C $$shell_quote($$OUT_PWD/watchwindow) run-test & \
    $(MAKE) -C $$shell_quote($$OUT_PWD/featuresorter) run-test

contains( DEFINES, ENABLE_GUI_TEST ) {
    run-test.commands += & \
    $(MAKE) -C $$shell_quote($$OUT_PWD/dialogs) run-test
}

contains( DEFINES, ENABLE_MULTITHREAD_TEST ) {
run-test.commangs += & \
    $(MAKE) -C $$shell_quote($$OUT_PWD/measurebehavior) run-test
}

} else:linux {
run-test.commands = \
    [ -e "reports" ] || mkdir reports ; \
    $(MAKE) -C watchwindow run-test ; \
    $(MAKE) -C featuresorter run-test

contains( DEFINES, ENABLE_GUI_TEST ) {
    run-test.commands = ; \
    $(MAKE) -C dialogs run-test
}

contains( DEFINES, ENABLE_MULTITHREAD_TEST ) {
    run-test.commands = ; \
    $(MAKE) -C measurebehavior run-test
}

}
