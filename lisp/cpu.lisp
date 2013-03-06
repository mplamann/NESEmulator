(defpackage :com.infreefall.nes
  (:use :common-lisp))
(in-package :com.infreefall.nes)

(defclass cpu () (
		  (x :initform 0 :accessor x)
		  (y :initform 0 :accessor y)
		  (a :initform 0 :accessor a)
		  (s :initform 0 :accessor s)
		  (pc :initform 0 :accessor pc)
		  (n :initform nil :accessor n)
		  (z :initform nil :accessor z)
		  (c :initform nil :accessor c)
		  (i :initform nil :accessor i)
		  (cli-delay :initform -1 :accessor cli-delay)
		  (d :initform nil :accessor d)
		  (v :initform nil :accessor v)
		  (b :initform nil :accessor b)))

(defvar *cpu* (make-instance 'cpu))

(defmacro with-cpu (cpu &body body)
  `(with-accessors ((x x)
		    (y y)
		    (a a)
		    (s s)
		    (pc pc)
		    (n n)
		    (z z)
		    (c c)
		    (i i)
		    (cli-delay cli-delay)
		    (d d)
		    (v v)
		    (b b))
		   ,cpu
     ,@body))

(defmacro defopcode (name &body body)
  `(defun ,name (cpu arg)
     (with-cpu cpu ,@body)))
(defmacro defnzopcode (name &body body)
  `(defopcode ,name
     (let ((result (progn ,@body)))
       (setf n (/= 0 (boole boole-and #x80 result)))
       (setf z (= 0 result)))))

(defmacro with-acc-mem (varname &body body)
  `(let* ((,varname (if (= arg -1)
		       a
		     (mem-read arg)))
	  (result (progn ,@body)))
     (if (= arg -1)
	 (setf a result)
       (mem-write arg result))))
(defun andFF (value)
  (boole boole-and value #xFF))

(defun mem-read (addr) addr)
(defun mem-write (addr value) (+ addr  value))

(defun run-instruction ()
  (with-cpu
   *cpu*
   (when (> cli-delay 0)
     (when (= (decf cli-delay) 0)
       (setf i nil)))
   (run (decode (fetch-instruction)) (fetch-arguments))))

(defun fetch-instruction ()
  (with-cpu
   *cpu*
   (mem-read pc)))

(defun fetch-arguments ()
  (with-cpu
   *cpu*
   (list
    (mem-read (+ pc 1))
    (mem-read (+ pc 2)))))

(defun decode (opcode)
  (case opcode
    ((#x69 #x65 #x75 #x6D #x7D #x79 #x61 #x71) 'adc)
    ((#x29 #x25 #x35 #x2D #x3 #x39 #x21 #x31) '6502-and)
    ((#x0A #x06 #x16 #x0E #x1E) 'asl)
    ((#x24 #x2C) '6502-bit)
    (#x10 'bpl)
    (#x30 'bmi)
    (#x50 'bvc)
    (#x70 'bvs)
    (#x90 'bcc)
    (#xB0 'bcs)
    (#xD0 'bne)
    (#xF0 'beq)
    (#x00 'brk)
    ((#xC9 #xC5 #xD5 #xCD #xDD #xD9 #xC1 #xD1) 'cmp)
    ((#xE0 #xE4 #xEC) 'cpx)
    ((#xC0 #xC4 #xCC) 'cpy)
    ((#xC6 #xD6 #xCE #xDE) 'dec)
    ((#x49 #x45 #x55 #x4D #x5D #x59 #x41 #x51) 'eor)
    (#x18 'clc)
    (#x38 'sec)
    (#x58 'cli)
    (#x78 'sei)
    (#xB8 'clv)
    (#xD8 'cld)
    (#xF8 'sed)
    ((#xE6 #xF6 #xEE #xFE) 'inc)
    ((#x4C #x6C) 'jmp)
    (#x20 'jsr)
    ((#xA9 #xA5 #xB5 #xAD #xBD #xB9 #xA1 #xB1) 'lda)
    ((#xA2 #xA6 #xB6 #xAE #xBE) 'ldx)
    ((#xA0 #xA4 #xB4 #xAC #xBC) 'ldy)
    ((#x4A #x46 #x56 #x4E #x5E) 'lsr)
    ((#x09 #x05 #x15 #x0D #x1D #x19 #x01 #x11) 'ora)
    (#xAA 'tax)
    (#x8A 'txa)
    (#xCA 'dex)
    (#xE8 'inx)
    (#xA8 'tay)
    (#x98 'tya)
    (#x88 'dey)
    (#xC8 'iny)
    ((#x2A #x26 #x36 #x2E #x3E) 'rol)
    ((#x6A #x66 #x76 #x6E #x7E) 'ror)
    (#x40 'rti)
    (#x60 'rts)
    ((#xE9 #xE5 #xF5 #xED #xFD #xF9 #xE1 #xF1) 'sbc)
    ((#x85 #x95 #x8D #x9D #x99 #x81 #x91) 'sta)
    (#x9A 'txs)
    (#xBA 'tsx)
    (#x48 'pha)
    (#x68 'pla)
    (#x08 'php)
    (#x28 'plp)
    ((#x86 #x96 #x8E) 'stx)
    ((#x84 #x94 #x8C) 'sty)
    (t 'nop)))

(defun run (instr arguments)
  (funcall (symbol-function instr) *cpu* arguments))

;; Load / Store Operations
(defnzopcode tax
  (setf x a))
(defnzopcode tay
  (setf y a))
(defnzopcode tsx
  (setf x s))
(defopcode txs
  (setf s x))
(defnzopcode txa
  (setf a x))
(defnzopcode tya
  (setf a y))
(defnzopcode lax
  (setf x arg)
  (setf a arg))
(defnzopcode lda
  (setf a arg))
(defnzopcode ldx
  (setf x arg))
(defnzopcode ldy
  (setf y arg))
(defopcode sta
  (mem-write arg a))
(defopcode stx
  (mem-write arg x))
(defopcode sty
  (mem-write arg y))

;; Flag operations
(defopcode clc
  (setf c nil))
(defopcode sec
  (setf c t))
(defopcode cli
  (setf cli-delay 1))
(defopcode sei
  (setf i t))
(defopcode clv
  (setf v nil))
(defopcode cld
  (setf d nil))
(defopcode sed
  (setf d t))

;; Boolean Operations
(defnzopcode 6502-and
  (setf a (boole boole-and a arg)))
(defnzopcode eor
  (setf a (boole boole-xor a arg)))
(defnzopcode ora
  (setf a (boole boole-ior a arg)))
(defnzopcode asl
  (with-acc-mem value
		(setf value (andFF (* value 2)))
		(setf c (/= 0 (boole boole-and #x100 value)))
		value))
(defopcode 6502-bit
  (setf z (= 0 (boole boole-and a arg)))
  (setf v (/= 0 (boole boole-and #x40 arg)))
  (setf n (/= 0 (boole boole-and #x80 arg))))

(defopcode nop
  '())


