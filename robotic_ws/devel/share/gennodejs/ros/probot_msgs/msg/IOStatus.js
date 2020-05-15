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

class IOStatus {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.inputIOs = null;
      this.outputIOs = null;
    }
    else {
      if (initObj.hasOwnProperty('inputIOs')) {
        this.inputIOs = initObj.inputIOs
      }
      else {
        this.inputIOs = [];
      }
      if (initObj.hasOwnProperty('outputIOs')) {
        this.outputIOs = initObj.outputIOs
      }
      else {
        this.outputIOs = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type IOStatus
    // Serialize message field [inputIOs]
    bufferOffset = _arraySerializer.int8(obj.inputIOs, buffer, bufferOffset, null);
    // Serialize message field [outputIOs]
    bufferOffset = _arraySerializer.int8(obj.outputIOs, buffer, bufferOffset, null);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type IOStatus
    let len;
    let data = new IOStatus(null);
    // Deserialize message field [inputIOs]
    data.inputIOs = _arrayDeserializer.int8(buffer, bufferOffset, null)
    // Deserialize message field [outputIOs]
    data.outputIOs = _arrayDeserializer.int8(buffer, bufferOffset, null)
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += object.inputIOs.length;
    length += object.outputIOs.length;
    return length + 8;
  }

  static datatype() {
    // Returns string type for a message object
    return 'probot_msgs/IOStatus';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'b01f2a264a951d41a72a20bf487359e0';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    int8[] inputIOs
    int8[] outputIOs
    
    int8 IO_LOW = 0
    int8 IO_HIGH = 1
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new IOStatus(null);
    if (msg.inputIOs !== undefined) {
      resolved.inputIOs = msg.inputIOs;
    }
    else {
      resolved.inputIOs = []
    }

    if (msg.outputIOs !== undefined) {
      resolved.outputIOs = msg.outputIOs;
    }
    else {
      resolved.outputIOs = []
    }

    return resolved;
    }
};

// Constants for message
IOStatus.Constants = {
  IO_LOW: 0,
  IO_HIGH: 1,
}

module.exports = IOStatus;
