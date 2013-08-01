
Project summary
-------

##Overview##
A platform that tracks a specific individual within a room though computer vision, while keeping a directional microphone pointed in their direction.

##Project stages ##
The following are stages the project can be broken down into to mitigate the risk of running out of time.

###Stage 1###
* Develop CV target tracking functionality in a video stream
* Build initial physical base for the platform including horizontal panning capabilities

###Stage 2###
* Add vertical movement capability of the physical platform to enable birds-eye view mounting vantage point
* Add a directional microphone (stereo?) to the platform
* Implement efficient audio-processing/storage / batch processing

###Stage 3###
* Investigate external power-supply solutions
* Implement standby mode with a motion sensor
* Batch-process/offload stored data to external storage

###Stage 4###
* Investigate voice-to-text audio processing
* Identifying and clustering of voices (e.g. 2 conversations by the same person, additional meta-data such as gender, language etc.)
* Conversation content analysis (NLP) 

###Stage 5###
* NSA stickers

##Required equipment##
###Stage 1###
* Raspberry Pi (multiple for processing/analysis?)
* Webcam
* Servos
* Stand/base

###Stage 2###
* Microphone(s)

###Stage 3###
* Portable power source
* Motion sensor module
* Remote connectivity module (e.g. wifi, gsm etc.)

##Other platform applications##

* **Events analytics**: Use to 'checksum' people, thus tracking navigation between nodes on an anonymous basis.
* **Security**: Friend-or-foe style identification/smarter motion-sensor.
* **Targeted advertising**: Detect vehicle make/model, estimate value and use that to determine the display ad.