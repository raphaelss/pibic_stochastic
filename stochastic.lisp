(defpackage #:stochastic
  (:use #:cl))

(in-package  #:stochastic)

(defun transpose (series n)
  (mapcar #'(lambda (x) (mod (+ x n) 12)) series))

(defun retrograde (series)
  (reverse series))

(defun invert (series)
  (mapcar #'(lambda (x) (mod (- 12 x) 12)) series))

(defun next-series (series)
  (transpose
   (case (random 4)
     (0 series)
     (1 (retrograde series))
     (2 (invert series))
     (3 (retrograde (invert series))))
   (random 12)))

(defstruct series-handler state series)

(defun init-series-handler (series)
  (let ((s (next-series series)))
    (make-series-handler :state s :series s)))

(defun get-pitches (sh &key (min 1) (max 6))
  (let ((n (+ min (random (- (1+ max) min))))
	(pitches)
	(series (series-handler-state sh)))
    (dotimes (x n)
      (unless series
	(setf series (next-series (series-handler-series sh))))
      (push (pop series) pitches))
    (setf (series-handler-state sh) series)
    pitches))

(defun exp-distr (mean)
  (/ (log (- 1 (random 1.0)))
     (- (/ mean))))

(defun make-dur-list (target-dur mean)
  (do ((list nil (cons dur list))
       (time 0 (+ time dur))
       (dur (exp-distr mean) (exp-distr mean)))
      ((> time target-dur) list)))

(defun run (total-dur mean-sec-dur mean-sec-dens series)
  (let ((sections (make-dur-list total-dur mean-sec-dur))
	(sh (init-series-handler series))
	(n 1))
    (format t "Duração: ~a~%" (reduce #'+ sections))
    (dolist (s sections)
      (let ((events (make-dur-list s (/ s mean-sec-dens))))
	(format t "SEÇÃO ~a - duração: ~a~%" n (reduce #'+ events))
	(dolist (ev events)
	  (let ((chord (get-pitches sh)))
	    (format t "- Duração: ~a; acorde: ~a~%" ev chord)))
      (format t "~%---~%~%")
      (incf n)))))
