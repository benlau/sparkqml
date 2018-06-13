
!isEmpty(INSTALL_ROOT) {
    target.path = $${INSTALL_ROOT}
    INSTALLS += target
    export(INSTALLS)

    mac {
        SPARKQML_BIN = "$${INSTALL_ROOT}/SparkQML.app/Contents/MacOS/SparkQML"
    } else {
        SPARKQML_BIN = "$${INSTALL_ROOT}/SparkQML"
    }

    win32 {
        QTCREATOR_XML_PATH = "$$(APPDATA)/QtProject/qtcreator/externaltools"
    } else {
        QTCREATOR_XML_PATH = "$$(HOME)/.config/QtProject/qtcreator/externaltools"
    }

    QMAKE_SUBSTITUTES += deployment/qtcreator_sparkqml.xml.in

    install_qtcreator_xml.target = install_qtcreator_xml
    install_qtcreator_xml.commands = cp "$${OUT_PWD}/deployment/qtcreator_sparkqml.xml" "$${QTCREATOR_XML_PATH}/qtcreator_sparkqml.xml"

    QMAKE_EXTRA_TARGETS += install_qtcreator_xml
}

DISTFILES += \
    $$PWD/deployment/qtcreator_sparkqml.xml.in

