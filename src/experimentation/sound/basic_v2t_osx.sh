#!/usr/bin/sh
# A simple shell script to test Google's Voice to Text API
# requires Sox (brew install sox) which is the OSX equiv of arecord
# and ffmpeg for conversion of WAV to FLAC (as the API only takes FLAC)

sox -d test.wav
cat test.wav | ffmpeg -y -i - -ar 16000 -acodec flac test.flac
wget -q -U "Mozilla/5.0" --post-file test.flac --header "Content-Type: audio/x-flac; rate=16000" -O - "http://www.google.com/speech-api/v1/recognize?lang=en-us&client=chromium"
