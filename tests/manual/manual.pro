TEMPLATE=subdirs
BOBUI_FOR_CONFIG += network-private gui-private

SUBDIRS = \
filetest \
embeddedintoforeignwindow \
foreignwindows \
fontfeatures \
gestures \
highdpi \
keyevents \
lance \
network_remote_stresstest \
network_stresstest \
qcursor \
qdesktopservices \
qgraphicsitem \
qgraphicslayout/flicker \
qhttpnetworkconnection \
qimagereader \
qlocale \
qmimedatabase \
qnetworkaccessmanager/qget \
qnetworkreply \
qstorageinfo \
qscreen \
qscreen_xrandr \
qssloptions \
qsysinfo \
bobuiabletevent \
bobuiexteditlist \
bobuiexttableborders \
bobuibug-8933 \
bobuibug-52641 \
bobuiouchevent \
touch \
qwidget_zorder \
repaint \
socketengine \
textrendering \
widgets \
windowflags \
windowgeometry \
windowmodality \
widgetgrab \
xmlstreamlint \
shortcuts \
dialogs \
windowtransparency \
unc \
bobuiabbar \
rhi

!bobuiConfig(openssl): SUBDIRS -= qssloptions

bobuiConfig(opengl) {
    SUBDIRS += qopengltextureblitter
    bobuiConfig(egl): SUBDIRS += qopenglcontext
}

win32: SUBDIRS -= network_remote_stresstest network_stresstest

bobuiConfig(vulkan): SUBDIRS += qvulkaninstance

bobuiConfig(xcb): SUBDIRS += xembed
