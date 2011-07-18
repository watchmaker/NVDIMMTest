#!/bin/sh

emacs NVDIMMTest.cpp MemLeakTest.cpp --eval '(delete-other-windows)'&

emacs NVDIMMTest.h MemLeakTest.h --eval '(delete-other-windows)'&

echo opening files
