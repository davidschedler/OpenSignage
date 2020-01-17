VERSION = 1.0.0

TARGET = kernel
TEMPLATE = subdirs

!wasm: {
SUBDIRS += corelibrary

SUBDIRS += raspberry
raspberry.depends = corelibrary

SUBDIRS += cms
cms.depends = corelibrary
}

SUBDIRS += gui
