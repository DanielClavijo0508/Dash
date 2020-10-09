#!/bin/bash

g++ main.cpp p2_client.cpp /data/Dash/demo/base/common_utils.cpp /data/Dash/demo/base/json_utils.cpp /data/Dash/demo/base/log.cpp /data/Dash/demo/base/md5.cpp /data/Dash/demo/net/channel.cpp /data/Dash/demo/net/poller.cpp  /data/Dash/demo/net/socket_utils.cpp /data/Dash/demo/json/json_reader.cpp /data/Dash/demo/json/json_value.cpp /data/Dash/demo/json/json_writer.cpp  -o jdemo
./jdemo
