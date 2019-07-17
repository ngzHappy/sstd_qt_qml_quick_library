
QT += gui
QT += qml
QT += svg
QT += core
QT += quick
QT += widgets
QT += network
QT += concurrent
QT += quickwidgets
QT += quickcontrols2

DEFINES *= SSTD_HAS_GLEW_APPLICATION_INITIALIZE

#glm ...
DEFINES *= GLM_FORCE_CXX17
DEFINES *= GLM_FORCE_PURE
DEFINES *= _CRT_SECURE_NO_WARNINGS
