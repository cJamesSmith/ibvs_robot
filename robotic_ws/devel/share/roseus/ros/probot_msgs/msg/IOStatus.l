;; Auto-generated. Do not edit!


(when (boundp 'probot_msgs::IOStatus)
  (if (not (find-package "PROBOT_MSGS"))
    (make-package "PROBOT_MSGS"))
  (shadow 'IOStatus (find-package "PROBOT_MSGS")))
(unless (find-package "PROBOT_MSGS::IOSTATUS")
  (make-package "PROBOT_MSGS::IOSTATUS"))

(in-package "ROS")
;;//! \htmlinclude IOStatus.msg.html


(intern "*IO_LOW*" (find-package "PROBOT_MSGS::IOSTATUS"))
(shadow '*IO_LOW* (find-package "PROBOT_MSGS::IOSTATUS"))
(defconstant probot_msgs::IOStatus::*IO_LOW* 0)
(intern "*IO_HIGH*" (find-package "PROBOT_MSGS::IOSTATUS"))
(shadow '*IO_HIGH* (find-package "PROBOT_MSGS::IOSTATUS"))
(defconstant probot_msgs::IOStatus::*IO_HIGH* 1)
(defclass probot_msgs::IOStatus
  :super ros::object
  :slots (_inputIOs _outputIOs ))

(defmethod probot_msgs::IOStatus
  (:init
   (&key
    ((:inputIOs __inputIOs) (make-array 0 :initial-element 0 :element-type :integer))
    ((:outputIOs __outputIOs) (make-array 0 :initial-element 0 :element-type :integer))
    )
   (send-super :init)
   (setq _inputIOs __inputIOs)
   (setq _outputIOs __outputIOs)
   self)
  (:inputIOs
   (&optional __inputIOs)
   (if __inputIOs (setq _inputIOs __inputIOs)) _inputIOs)
  (:outputIOs
   (&optional __outputIOs)
   (if __outputIOs (setq _outputIOs __outputIOs)) _outputIOs)
  (:serialization-length
   ()
   (+
    ;; int8[] _inputIOs
    (* 1    (length _inputIOs)) 4
    ;; int8[] _outputIOs
    (* 1    (length _outputIOs)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int8[] _inputIOs
     (write-long (length _inputIOs) s)
     (dotimes (i (length _inputIOs))
       (write-byte (elt _inputIOs i) s)
       )
     ;; int8[] _outputIOs
     (write-long (length _outputIOs) s)
     (dotimes (i (length _outputIOs))
       (write-byte (elt _outputIOs i) s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int8[] _inputIOs
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _inputIOs (instantiate integer-vector n))
     (dotimes (i n)
     (setf (elt _inputIOs i) (sys::peek buf ptr- :char)) (incf ptr- 1)
     (if (> (elt _inputIOs i) 127) (setf (elt _inputIOs i) (- (elt _inputIOs i) 256)))
     ))
   ;; int8[] _outputIOs
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _outputIOs (instantiate integer-vector n))
     (dotimes (i n)
     (setf (elt _outputIOs i) (sys::peek buf ptr- :char)) (incf ptr- 1)
     (if (> (elt _outputIOs i) 127) (setf (elt _outputIOs i) (- (elt _outputIOs i) 256)))
     ))
   ;;
   self)
  )

(setf (get probot_msgs::IOStatus :md5sum-) "b01f2a264a951d41a72a20bf487359e0")
(setf (get probot_msgs::IOStatus :datatype-) "probot_msgs/IOStatus")
(setf (get probot_msgs::IOStatus :definition-)
      "int8[] inputIOs
int8[] outputIOs

int8 IO_LOW = 0
int8 IO_HIGH = 1
")



(provide :probot_msgs/IOStatus "b01f2a264a951d41a72a20bf487359e0")


