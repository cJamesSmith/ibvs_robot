;; Auto-generated. Do not edit!


(when (boundp 'probot_msgs::SetOutputIO)
  (if (not (find-package "PROBOT_MSGS"))
    (make-package "PROBOT_MSGS"))
  (shadow 'SetOutputIO (find-package "PROBOT_MSGS")))
(unless (find-package "PROBOT_MSGS::SETOUTPUTIO")
  (make-package "PROBOT_MSGS::SETOUTPUTIO"))

(in-package "ROS")
;;//! \htmlinclude SetOutputIO.msg.html


(intern "*IO_LOW*" (find-package "PROBOT_MSGS::SETOUTPUTIO"))
(shadow '*IO_LOW* (find-package "PROBOT_MSGS::SETOUTPUTIO"))
(defconstant probot_msgs::SetOutputIO::*IO_LOW* 0)
(intern "*IO_HIGH*" (find-package "PROBOT_MSGS::SETOUTPUTIO"))
(shadow '*IO_HIGH* (find-package "PROBOT_MSGS::SETOUTPUTIO"))
(defconstant probot_msgs::SetOutputIO::*IO_HIGH* 1)
(defclass probot_msgs::SetOutputIO
  :super ros::object
  :slots (_ioNumber _status ))

(defmethod probot_msgs::SetOutputIO
  (:init
   (&key
    ((:ioNumber __ioNumber) 0)
    ((:status __status) 0)
    )
   (send-super :init)
   (setq _ioNumber (round __ioNumber))
   (setq _status (round __status))
   self)
  (:ioNumber
   (&optional __ioNumber)
   (if __ioNumber (setq _ioNumber __ioNumber)) _ioNumber)
  (:status
   (&optional __status)
   (if __status (setq _status __status)) _status)
  (:serialization-length
   ()
   (+
    ;; int8 _ioNumber
    1
    ;; int8 _status
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int8 _ioNumber
       (write-byte _ioNumber s)
     ;; int8 _status
       (write-byte _status s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int8 _ioNumber
     (setq _ioNumber (sys::peek buf ptr- :char)) (incf ptr- 1)
     (if (> _ioNumber 127) (setq _ioNumber (- _ioNumber 256)))
   ;; int8 _status
     (setq _status (sys::peek buf ptr- :char)) (incf ptr- 1)
     (if (> _status 127) (setq _status (- _status 256)))
   ;;
   self)
  )

(setf (get probot_msgs::SetOutputIO :md5sum-) "17d3f288ef8d0147911617f2a2126f98")
(setf (get probot_msgs::SetOutputIO :datatype-) "probot_msgs/SetOutputIO")
(setf (get probot_msgs::SetOutputIO :definition-)
      "int8 ioNumber
int8 status

int8 IO_LOW = 0
int8 IO_HIGH = 1

")



(provide :probot_msgs/SetOutputIO "17d3f288ef8d0147911617f2a2126f98")


