// Generated by gencpp from file probot_msgs/JogJoint.msg
// DO NOT EDIT!


#ifndef PROBOT_MSGS_MESSAGE_JOGJOINT_H
#define PROBOT_MSGS_MESSAGE_JOGJOINT_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace probot_msgs
{
template <class ContainerAllocator>
struct JogJoint_
{
  typedef JogJoint_<ContainerAllocator> Type;

  JogJoint_()
    : joint_index(0)
    , direction(0)
    , mode(0)
    , velocity_scale(0.0)  {
    }
  JogJoint_(const ContainerAllocator& _alloc)
    : joint_index(0)
    , direction(0)
    , mode(0)
    , velocity_scale(0.0)  {
  (void)_alloc;
    }



   typedef int8_t _joint_index_type;
  _joint_index_type joint_index;

   typedef int8_t _direction_type;
  _direction_type direction;

   typedef int8_t _mode_type;
  _mode_type mode;

   typedef float _velocity_scale_type;
  _velocity_scale_type velocity_scale;



  enum {
    DIR_PLUS = 0,
    DIR_MINUS = 1,
    MOD_CONTINUOUS = 0,
    MOD_INCREMENT_10_DEGREE = 1,
    MOD_INCREMENT_1_DEGREE = 2,
    MOD_INCREMENT_01_DEGREE = 3,
  };


  typedef boost::shared_ptr< ::probot_msgs::JogJoint_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::probot_msgs::JogJoint_<ContainerAllocator> const> ConstPtr;

}; // struct JogJoint_

typedef ::probot_msgs::JogJoint_<std::allocator<void> > JogJoint;

typedef boost::shared_ptr< ::probot_msgs::JogJoint > JogJointPtr;
typedef boost::shared_ptr< ::probot_msgs::JogJoint const> JogJointConstPtr;

// constants requiring out of line definition

   

   

   

   

   

   



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::probot_msgs::JogJoint_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::probot_msgs::JogJoint_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace probot_msgs

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': True, 'IsMessage': True, 'HasHeader': False}
// {'geometry_msgs': ['/opt/ros/kinetic/share/geometry_msgs/cmake/../msg'], 'probot_msgs': ['/home/cxw/robotic_ws/src/probot_anno/probot_msgs/msg'], 'std_msgs': ['/opt/ros/kinetic/share/std_msgs/cmake/../msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::probot_msgs::JogJoint_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::probot_msgs::JogJoint_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::probot_msgs::JogJoint_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::probot_msgs::JogJoint_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::probot_msgs::JogJoint_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::probot_msgs::JogJoint_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::probot_msgs::JogJoint_<ContainerAllocator> >
{
  static const char* value()
  {
    return "02f6d1212f80ad72751bc065b3e3523c";
  }

  static const char* value(const ::probot_msgs::JogJoint_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x02f6d1212f80ad72ULL;
  static const uint64_t static_value2 = 0x751bc065b3e3523cULL;
};

template<class ContainerAllocator>
struct DataType< ::probot_msgs::JogJoint_<ContainerAllocator> >
{
  static const char* value()
  {
    return "probot_msgs/JogJoint";
  }

  static const char* value(const ::probot_msgs::JogJoint_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::probot_msgs::JogJoint_<ContainerAllocator> >
{
  static const char* value()
  {
    return "# The Jogging joint message contains control information for jogging\n\
# a specific joint\n\
# Set velocity_scale to 0 to stop jogging\n\
\n\
# The jogged joint index\n\
# Negative joint index value means stop jogging\n\
int8 joint_index\n\
\n\
# The jogging direction\n\
int8 direction\n\
\n\
# The jogging mode\n\
int8 mode\n\
\n\
# The velocity scale for move_group interface\n\
float32 velocity_scale\n\
\n\
int8 DIR_PLUS = 0\n\
int8 DIR_MINUS = 1\n\
\n\
int8 MOD_CONTINUOUS            = 0\n\
int8 MOD_INCREMENT_10_DEGREE   = 1\n\
int8 MOD_INCREMENT_1_DEGREE    = 2\n\
int8 MOD_INCREMENT_01_DEGREE   = 3\n\
";
  }

  static const char* value(const ::probot_msgs::JogJoint_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::probot_msgs::JogJoint_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.joint_index);
      stream.next(m.direction);
      stream.next(m.mode);
      stream.next(m.velocity_scale);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct JogJoint_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::probot_msgs::JogJoint_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::probot_msgs::JogJoint_<ContainerAllocator>& v)
  {
    s << indent << "joint_index: ";
    Printer<int8_t>::stream(s, indent + "  ", v.joint_index);
    s << indent << "direction: ";
    Printer<int8_t>::stream(s, indent + "  ", v.direction);
    s << indent << "mode: ";
    Printer<int8_t>::stream(s, indent + "  ", v.mode);
    s << indent << "velocity_scale: ";
    Printer<float>::stream(s, indent + "  ", v.velocity_scale);
  }
};

} // namespace message_operations
} // namespace ros

#endif // PROBOT_MSGS_MESSAGE_JOGJOINT_H
