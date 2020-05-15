; Auto-generated. Do not edit!


(cl:in-package probot_msgs-msg)


;//! \htmlinclude SetOutputIO.msg.html

(cl:defclass <SetOutputIO> (roslisp-msg-protocol:ros-message)
  ((ioNumber
    :reader ioNumber
    :initarg :ioNumber
    :type cl:fixnum
    :initform 0)
   (status
    :reader status
    :initarg :status
    :type cl:fixnum
    :initform 0))
)

(cl:defclass SetOutputIO (<SetOutputIO>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <SetOutputIO>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'SetOutputIO)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name probot_msgs-msg:<SetOutputIO> is deprecated: use probot_msgs-msg:SetOutputIO instead.")))

(cl:ensure-generic-function 'ioNumber-val :lambda-list '(m))
(cl:defmethod ioNumber-val ((m <SetOutputIO>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader probot_msgs-msg:ioNumber-val is deprecated.  Use probot_msgs-msg:ioNumber instead.")
  (ioNumber m))

(cl:ensure-generic-function 'status-val :lambda-list '(m))
(cl:defmethod status-val ((m <SetOutputIO>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader probot_msgs-msg:status-val is deprecated.  Use probot_msgs-msg:status instead.")
  (status m))
(cl:defmethod roslisp-msg-protocol:symbol-codes ((msg-type (cl:eql '<SetOutputIO>)))
    "Constants for message type '<SetOutputIO>"
  '((:IO_LOW . 0)
    (:IO_HIGH . 1))
)
(cl:defmethod roslisp-msg-protocol:symbol-codes ((msg-type (cl:eql 'SetOutputIO)))
    "Constants for message type 'SetOutputIO"
  '((:IO_LOW . 0)
    (:IO_HIGH . 1))
)
(cl:defmethod roslisp-msg-protocol:serialize ((msg <SetOutputIO>) ostream)
  "Serializes a message object of type '<SetOutputIO>"
  (cl:let* ((signed (cl:slot-value msg 'ioNumber)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 256) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'status)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 256) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <SetOutputIO>) istream)
  "Deserializes a message object of type '<SetOutputIO>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'ioNumber) (cl:if (cl:< unsigned 128) unsigned (cl:- unsigned 256))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'status) (cl:if (cl:< unsigned 128) unsigned (cl:- unsigned 256))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<SetOutputIO>)))
  "Returns string type for a message object of type '<SetOutputIO>"
  "probot_msgs/SetOutputIO")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'SetOutputIO)))
  "Returns string type for a message object of type 'SetOutputIO"
  "probot_msgs/SetOutputIO")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<SetOutputIO>)))
  "Returns md5sum for a message object of type '<SetOutputIO>"
  "17d3f288ef8d0147911617f2a2126f98")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'SetOutputIO)))
  "Returns md5sum for a message object of type 'SetOutputIO"
  "17d3f288ef8d0147911617f2a2126f98")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<SetOutputIO>)))
  "Returns full string definition for message of type '<SetOutputIO>"
  (cl:format cl:nil "int8 ioNumber~%int8 status~%~%int8 IO_LOW = 0~%int8 IO_HIGH = 1~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'SetOutputIO)))
  "Returns full string definition for message of type 'SetOutputIO"
  (cl:format cl:nil "int8 ioNumber~%int8 status~%~%int8 IO_LOW = 0~%int8 IO_HIGH = 1~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <SetOutputIO>))
  (cl:+ 0
     1
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <SetOutputIO>))
  "Converts a ROS message object to a list"
  (cl:list 'SetOutputIO
    (cl:cons ':ioNumber (ioNumber msg))
    (cl:cons ':status (status msg))
))
