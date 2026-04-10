requires(bobuiHaveModule(network))

TEMPLATE      = subdirs
BOBUI_FOR_CONFIG += network-private
!integrity: SUBDIRS += dnslookup
bobuiHaveModule(widgets) {
    SUBDIRS +=  \
                blockingfortuneclient \
                broadcastreceiver \
                broadcastsender \
                fortuneclient \
                fortuneserver \
                http \
                multicastreceiver \
                multicastsender \
                rsslisting \
                threadedfortuneserver \
                torrent

    bobuiConfig(processenvironment): SUBDIRS += network-chat
    bobuiConfig(ssl): SUBDIRS += securesocketclient
    bobuiConfig(dtls): SUBDIRS += secureudpserver secureudpclient
    bobuiConfig(sctp): SUBDIRS += multistreamserver multistreamclient
}

EXAMPLE_FILES = shared
