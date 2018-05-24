!exists(vendor/vendor.pri) {
    message("cd $$PWD")
    message("qpm install")
    message($$system("qpm install"))
}

exists(vendor/vendor.pri) {
    include(vendor/vendor.pri)
} else {
    error("vendor/vendor.pri is not found. To install required packages. Run `qpm install` in $$PWD");
}
