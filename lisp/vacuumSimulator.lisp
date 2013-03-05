(defvar *map* '())
(defvar *agent-loc* '(0 0))
(defvar *agent-heading* '(0 1))
(defvar *home-loc* '(0 0))
(defvar *touch-sensor* 0)
(defvar *moves-remaining* 1000)
(defvar *score* 0)
(defvar *running* t)

(defun generate-2x2-map ()
  (setf *map*
	#(#(clean clean)
	  #(clean clean))))

(defun list-to-vector (list)
  (make-array (length list) :initial-contents list))

(defun uniform-random-dirt (prob)
  (setf *map* (list-to-vector
	       (loop for row being the elements of *map*
		     collect (list-to-vector (loop for square being the elements of row
						   collect (if (and (not (eq square 'furniture))
								    (< (random 1.0) prob))
							       'dirty
							     'clean)))))))

(defmacro with-xy (pair &body body)
  `(let ((x (car ,pair))
	 (y (cadr ,pair)))
     ,@body))

(defmacro defaction (name args &body body)
  `(defun ,name ,args
     (decf *score*)
     ,@body))

(defun elt-or-wall (vector loc)
  (if (or (< loc 0)
	  (not (typep vector 'vector))
	  (>= loc (length vector)))
      'wall
    (elt vector loc)))

(defun square-at (loc)
  (elt-or-wall (elt-or-wall *map* (cadr loc)) (car loc)))

(defun update-square-at (loc value)
  (setf (elt (elt *map* (cadr loc)) (car loc)) value))

(defsetf square-at update-square-at)

(defun percept ()
  (list
   *touch-sensor*
   (dirt-sensor)
   (home-sensor)))

(defun dirt-sensor ()
  (if (eq (square-at *agent-loc*) 'dirt)
      1
    0))
(defun home-sensor ()
  (if (equal *agent-loc* *home-loc*)
      1
    0))

(defaction turn-right ()
  (with-xy *agent-heading*
	   (setf *agent-heading* (list y (* x -1)))))
(defaction turn-left ()
  (with-xy *agent-heading*
	   (setf *agent-heading* (list (* -1 y) x))))
(defaction forward ()
  (let* ((next-loc (list (+ (car *agent-loc*) (car *agent-heading*))
			 (+ (cadr *agent-loc*) (cadr *agent-heading*))))
	 (next-square (square-at next-loc)))
    (if (or (eq next-square 'clean)
	    (eq next-square 'dirty))
	(progn
	  (setf *agent-loc* next-loc)
	  (setf *touch-sensor* 0))
      (setf *touch-sensor* 1))))
(defaction vacuum ()
  (if (eq (square-at *agent-loc*) 'dirty)
      (progn
	(setf (square-at *agent-loc*) 'clean)
	(incf *score* 100))
    nil))
(defaction turn-off ()
  (setf *running* nil)
  (if (not (eq *agent-loc* *home-loc*))
      (decf *score* 1000)
    nil))

(defun reset-simulation ()
  (setf *moves-remaining* 1000)
  (setf *score* 0)
  (setf *agent-loc* '(0 0))
  (setf *agent-heading* '(0 1))
  (setf *touch-sensor* 0)
  (generate-2x2-map)
  (uniform-random-dirt 0.5))

(defun simulate-agent (decision-function)
  (funcall decision-function (percept))
  (decf *moves-remaining*)
  (if *running*
      (simulate-agent decision-function)
    *score*))
