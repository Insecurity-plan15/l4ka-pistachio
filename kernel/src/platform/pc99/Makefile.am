
noinst_LIBRARIES = libplatform.a

libplatform_a_SOURCES = startup.S

if CONFIG_IOAPIC
libplatform_a_SOURCES += ../generic/intctrl-apic.cc
else
libplatform_a_SOURCES += ../generic/intctrl-pic.cc
endif
