TEMPLATE      = subdirs

SUBDIRS = openglwindow \
          hellogles3

bobuiHaveModule(widgets) {
    SUBDIRS += 2dpainting \
               hellogl2 \
               cube \
               textures \
               stereoqopenglwidget
}
