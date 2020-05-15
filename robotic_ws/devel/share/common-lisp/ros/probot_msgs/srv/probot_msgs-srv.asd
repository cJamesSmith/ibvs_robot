
(cl:in-package :asdf)

(defsystem "probot_msgs-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "HeartbeatSrv" :depends-on ("_package_HeartbeatSrv"))
    (:file "_package_HeartbeatSrv" :depends-on ("_package"))
  ))