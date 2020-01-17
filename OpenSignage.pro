VERSION = 1.0.0

TARGET = OpenSignage
TEMPLATE = subdirs

!wasm: {
SUBDIRS += plugins
}
SUBDIRS += kernel


