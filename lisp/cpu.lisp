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
  `(defun ,name (cpu args)
     (with-cpu cpu ,@body)))
(defmacro defnzopcode (name &body body)
  `(defopcode ,name
     (let ((result (progn ,@body)))
       (setf n (/= 0 (boole boole-and #x80 result)))
       (setf z (= 0 result)))))

(defun mem-read (addr) addr)
(defun mem-write (addr value) value)

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
    ((#x0A #x06 #x16 #x0E #x1E) 'asl)
    ((#x24 #x2C) 'bit)
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
  (setf x (car args))
  (setf a (car args)))
(defnzopcode lda
  (setf a (car args)))
(defnzopcode ldx
  (setf x (car args)))
(defnzopcode ldy
  (setf y (car args)))
(defopcode sta
  (mem-write (car args) a))
(defopcode stx
  (mem-write (car args) x))
(defopcode sty
  (mem-write (car args) y))

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
  (setf a (boole boole-and a (car args))))
(defnzopcode eor
  (setf a (boole boole-xor a (car args))))
(defnzopcode ora
  (setf a (boole boole-ior a (car args))))
(defnzopcode asl
  (let ((value (lsh ())))
   (if (= (car args) -1)
       (setf a value)
     (mem-write (car args) value))))
(defopcode bit
  (setf z (= 0 (boole boole-and a (car args))))
  (setf v (/= 0 (boole boole-and #x40 (car args))))
  (setf n (/= 0 (boole boole-and #x80 (car args)))))

(defopcode nop
  '())


