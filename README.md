webrtc_playground
=================

C++ WebRTC Playground

Just a simple playground to gain some knowledge about the native WebRTC interfaces.

Howto Build:

* Get the latest webrtc trunk 
* Put the content from libjingle_examples_extension.txt to libjingle_examples.gyp
* Copy the folder talk to webrtc/trunk/talk
* Build the source, gclient sync, gclient runhooks

Howto Use:

* Search and replace the hard coded path 'e:/signaling/'
* Start console application on pc1 type in create_offer
* Start webserver on pc1
* Open Chrome on pc2 with url of pc1
* Wait until all ice candidates available
* On pc1 type in handle_anwser


