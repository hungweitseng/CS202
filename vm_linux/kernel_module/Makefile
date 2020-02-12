#
# By default, the build is done against the running linux kernel source.
# To build against a different kernel source tree, set SYSSRC:
#
#    make SYSSRC=/path/to/kernel/source

CONFIG_MODULE_SIG=n

ifdef SYSSRC
 KERNEL_SOURCES	 = $(SYSSRC)
else
 KERNEL_UNAME	:= $(shell uname -r)
 KERNEL_SOURCES	 = /lib/modules/$(KERNEL_UNAME)/build
endif


default: modules
.PHONY: default
install: modules_install
	cp mymodule-blacklist.conf /etc/modprobe.d
	cp 80-mymodule.rules /etc/udev/rules.d
	mkdir -p /usr/local/include/mymodule
	cp include/*.h /usr/local/include/mymodule/
	update-initramfs -u

.PHONY: install


.PHONY:


%::
	$(MAKE) -C $(KERNEL_SOURCES) \
        M=$$PWD $@
