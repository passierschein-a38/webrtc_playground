webrtc_playground
=================

C++ WebRTC Playground

Just a simple playground to gain some knowledge about the native WebRTC interfaces.

7/7/2014 - Working with latest Chrome, FireFox and Opera

Howto Build:

* Get the latest webrtc trunk 
* Put the content from libjingle_examples_extension.txt to libjingle_examples.gyp
* Copy the folder talk to webrtc/trunk/talk
* Build the source, gclient sync, gclient runhooks

Howto Use:

* Search and replace the hard coded path 'e:/signaling/'
* Start console application on pc1 type in o ( to create an offer )
* Start webserver on pc1
* Open Chrome on pc2 with url of pc1
* Wait until all ice candidates available
* On pc1 type in rd ( to setup remote description )


