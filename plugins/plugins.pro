VERSION = 1.0.0

TARGET = plugins
TEMPLATE = subdirs

!wasm: {
SUBDIRS += communicationserver
SUBDIRS += database
SUBDIRS += communicationclient
SUBDIRS += downloadclient
SUBDIRS += player
}

#SUBDIRS += downloadserver

