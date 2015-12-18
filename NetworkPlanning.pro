TEMPLATE = subdirs

EXTRAS += \
    NetworkPlanning \
    NetworkCalc

for(dir, EXTRAS) {
    exists($$dir) {
        SUBDIRS += $$dir
    }
}

