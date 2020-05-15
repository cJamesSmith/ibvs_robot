// Auto-generated. Do not edit!

// (in-package probot_msgs.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class SetOutputIO {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.ioNumber = null;
      this.status = null;
    }
    else {
      if (initObj.hasOwnProperty('ioNumber')) {
        this.ioNumber = initObj.ioNumber
      }
      else {
        this.ioNumber = 0;
      }
      if (initObj.hasOwnProperty('status')) {
        this.status = initObj.status
      }
      else {
        this.status = 0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type SetOutputIO
    // Serialize message field [ioNumber]
    bufferOffset = _serializer.int8(obj.ioNumber, buffer, bufferOffset);
    // Serialize message field [status]
    bufferOffset = _serializer.int8(obj.status, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type SetOutputIO
    let len;
    let data = new SetOutputIO(null);
    // Deserialize message field [ioNumber]
    data.ioNumber = _deserializer.int8(buffer, bufferOffset);
    // Deserialize message field [status]
    data.status = _deserializer.int8(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 2;
  }

  static datatype() {
    // Returns string type for a message object
    return 'probot_msgs/SetOutputIO';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '17d3f288ef8d0147911617f2a2126f98';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    int8 ioNumber
    int8 status
    
    int8 IO_LOW = 0
    int8 IO_HIGH = 1
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new SetOutputIO(null);
    if (msg.ioNumber !== undefined) {
      resolved.ioNumber = msg.ioNumber;
    }
    else {
      resolved.ioNumber = 0
    }

    if (msg.status !== undefined) {
      resolved.status = msg.status;
    }
    else {
      resolved.status = 0
    }

    return resolved;
    }
};

// Constants for message
SetOutputIO.Constants = {
  IO_LOW: 0,
  IO_HIGH: 1,
}

module.exports = SetOutputIO;
