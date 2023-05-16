QT       += core gui
QT       += network
QT       +=  printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17
LIBS += -lpthread libwsock32 libws2_32
QT    += core gui charts
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QWT_DLL QWT_POLAR_DLL
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    add_criteria.cpp \
    aes.c \
    aiming_screen.cpp \
    calibration.cpp \
    calibration_password.cpp \
    chart.cpp \
    circle_property.cpp \
    common_modbus_clientlib.c \
    complex_number.cpp \
    configuration.cpp \
    curve_grap_show.cpp \
    fft.cpp \
    formdoc.cpp \
    function_key.cpp \
    histogram.cpp \
    identify_area_attributes.cpp \
    main.cpp \
    mainwindow.cpp \
    measurement_task.cpp \
    modbus_clientlib.c \
    modbusclient.c \
    mouse_release_event.cpp \
    msxe371x_modbus_clientlib.c \
    network.c \
    new_profile.cpp \
    polar_plot_show.cpp \
    property_of_curve.cpp \
    qcustomplot.cpp \
    result_area_attribute.cpp \
    start_measuring_the_thread.cpp \
    statistical_report.cpp \
    time.c

HEADERS += \
    add_criteria.h \
    addidata_network.h \
    aes.h \
    aiming_screen.h \
    calibration.h \
    calibration_password.h \
    chart.h \
    circle_property.h \
    common_modbus_clientlib.h \
    complex_number.h \
    configuration.h \
    curve_grap_show.h \
    fft.h \
    formdoc.h \
    function_key.h \
    get_data_for_msx3711.h \
    get_polar_data.h \
    histogram.h \
    identify_area_attributes.h \
    mainwindow.h \
    measurement_task.h \
    modbus_clientlib.h \
    modbusclient.h \
    mouse_release_event.h \
    msxe371x_modbus_clientlib.h \
    new_profile.h \
    polar_plot_show.h \
    profile.h \
    property_of_curve.h \
    qcustomplot.h \
    result_area_attribute.h \
    start_measuring_the_thread.h \
    statistical_report.h \
    stdint_compat.h

FORMS += \
    add_criteria.ui \
    aiming_screen.ui \
    calibration.ui \
    calibration_password.ui \
    chart.ui \
    circle_property_.ui \
    configuration.ui \
    curve_grap_show.ui \
    formdoc.ui \
    function_key.ui \
    histogram.ui \
    identify_area_attributes.ui \
    mainwindow.ui \
    measurement_task.ui \
    new_profile.ui \
    polar_plot_show.ui \
    property_of_curve.ui \
    result_area_attribute.ui \
    statistical_report.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc



LIBS += -L"C:\Qt\Qt5.12.6\5.12.6\mingw73_64\lib" -lqwtd -lqwtpolard
LIBS += -L"C:\Qt\Qt5.12.6\5.12.6\mingw73_64\lib" -lqwt
INCLUDEPATH += C:\Qt\Qt5.12.6\5.12.6\mingw73_64\include\Qwt
INCLUDEPATH += C:\Qt\Qt5.12.6\5.12.6\mingw73_64\include\QwtPolar


INCLUDEPATH+=C:\opencv_3.4.3\build\include
             C:\opencv_3.4.3\build\include\opencv
             C:opencv_3.4.3\build\include\opencv2
             C:\opencv_3.4.3\include

LIBS += -L C:\opencv_3.4.3\opencv-build\lib\libopencv_*.a

INCLUDEPATH += C:\opencv_3.4.3\build\include
CONFIG(debug, debug|release): {
LIBS += -LC:\opencv_3.4.3\build\x64\vc14\bin \
-lopencv_world343
-lopencv_world343d
} else:CONFIG(release, debug|release): {
LIBS += -LC:\opencv_3.4.3\build\x64\vc14\bin \
-lopencv_world343
-lopencv_world343d
}




