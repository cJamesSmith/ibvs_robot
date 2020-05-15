; Auto-generated. Do not edit!


(cl:in-package probot_msgs-msg)


;//! \htmlinclude IOStatus.msg.html

(cl:defclass <IOStatus> (roslisp-msg-protocol:ros-message)
  ((inputIOs
    :reader inputIOs
    :initarg :inputIOs
    :type (cl:vector cl:fixnum)
   :initform (cl:make-array 0 :element-type 'cl:fixnum :initial-element 0))
   (outputIOs
    :reader outputIOs
    :initarg :outputIOs
    :type (cl:vector cl:fixnum)
   :initform (cl:make-array 0 :element-type 'cl:fixnum :initial-element 0)))
)

(cl:defclass IOStatus (<IOStatus>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <IOStatus>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'IOStatus)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name probot_msgs-msg:<IOStatus> is deprecated: use probot_msgs-msg:IOStatus instead.")))

(cl:ensure-generic-function 'inputIOs-val :lambda-list '(m))
(cl:defmethod inputIOs-val ((m <IOStatus>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader probot_msgs-msg:inputIOs-val is deprecated.  Use probot_msgs-msg:inputIOs instead.")
  (inputIOs m))

(cl:ensure-generic-function 'outputIOs-val :lambda-list '(m))
(cl:defmethod outputIOs-val ((m <IOStatus>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader probot_msgs-msg:outputIOs-val is deprecated.  Use probot_msgs-msg:outputIOs instead.")
  (outputIOs m))
(cl:defmethod roslisp-msg-protocol:symbol-codes ((msg-type (cl:eql '<IOStatus>)))
    "Constants for message type '<IOStatus>"
  '((:IO_LOW . 0)
    (:IO_HIGH . 1))
)
(cl:defmethod roslisp-msg-protocol:symbol-codes ((msg-type (cl:eql 'IOStatus)))
    "Constants for message type 'IOStatus"
  '((:IO_LOW . 0)
    (:IO_HIGH . 1))
)
(cl:defmethod roslisp-msg-protocol:serialize ((msg <IOStatus>) ostream)
  "Serializes a message object of type '<IOStatus>"
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'inputIOs))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let* ((signed ele) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 256) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    ))
   (cl:slot-value msg 'inputIOs))
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'outputIOs))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let* ((signed ele) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 256) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    ))
   (cl:slot-value msg 'outputIOs))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <IOStatus>) istream)
  "Deserializes a message object of type '<IOStatus>"
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'inputIOs) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'inputIOs)))
    (cl:dotimes (i __ros_arr_len)
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:aref vals i) (cl:if (cl:< unsigned 128) unsigned (cl:- unsigned 256)))))))
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'outputIOs) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'outputIOs)))
    (cl:dotimes (i __ros_arr_len)
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:aref vals i) (cl:if (cl:< unsigned 128) unsigned (cl:- unsigned 256)))))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<IOStatus>)))
  "Returns string type for a message object of type '<IOStatus>"
  "probot_msgs/IOStatus")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'IOStatus)))
  "Returns string type for a message object of type 'IOStatus"
  "probot_msgs/IOStatus")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<IOStatus>)))
  "Returns md5sum for a message object of type '<IOStatus>"
  "b01f2a264a951d41a72a20bf487359e0")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'IOStatus)))
  "Returns md5sum for a message object of type 'IOStatus"
  "b01f2a264a951d41a72a20bf487359e0")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<IOStatus>)))
  "Returns full string definition for message of type '<IOStatus>"
  (cl:format cl:nil "int8[] inputIOs~%int8[] outputIOs~%~%int8 IO_LOW = 0~%int8 IO_HIGH = 1~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'IOStatus)))
  "Returns full string definition for message of type 'IOStatus"
  (cl:format cl:nil "int8[] inputIOs~%int8[] outputIOs~%~%int8 IO_LOW = 0~%int8 IO_HIGH = 1~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <IOStatus>))
  (cl:+ 0
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'inputIOs) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 1)))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'outputIOs) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 1)))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <IOStatus>))
  "Converts a ROS message object to a list"
  (cl:list 'IOStatus
    (cl:cons ':inputIOs (inputIOs msg))
    (cl:cons ':outputIOs (outputIOs msg))
))
