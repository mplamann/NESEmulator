(in-package :com.infreefall.nes)

(defvar *RAM* (make-array #x2000 :initial-element 0))

(defun mem-read (addr) (elt *RAM* addr))
(defun mem-write (addr value) (setf (elt *RAM* addr) value))
