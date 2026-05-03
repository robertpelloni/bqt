TEMPLATE      = subdirs
SUBDIRS       = \
              chip \
              elasticnodes \
              collidingmice \
              basicgraphicslayouts \
              diagramscene \
              dragdroprobot \
              simpleanchorlayout

contains(DEFINES, BOBUI_NO_CURSOR)|!bobuiConfig(draganddrop): SUBDIRS -= dragdroprobot
