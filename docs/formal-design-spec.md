# Formal Design Specification

## Introduction

Our project will attempt to modernize the phonograph, by introducing subject tracking and voice-to-text translation controlled by the Raspberry PI.

The following are some of the use cases the project is attempting to address and cater for:

* Lecture theatre and tutorial recording and note taking in absence of Echo360 deployment
* Cost-effective portable recording solution for talks and presentations

## System Overview

System consists of the following components:

item | quantity | description
-----|----------|------------
Raspberry Pi | 2 | first RPi handles the computer vision (CV) and controlling the servo + motion sensor. The second is for processing sound and storage
RPi camera module | 1 | Using this for target tracking through CV
_USB camera_ | 1 | Used for actual session video recording
Servos | 2 | pan and tilt of the camera + microphone mount.
USB servo controller | 1 | External servo controller for movement accuracy
USB soundcard | 1 | As the RPi lacks an audio input, an external soundcard is required to facilitate microphone input
Shotgun microphone | 1 | A directional microphone with a relatively narrow cone of coverage would be ideal for recording to cut out the background noise
Motion sensor | 1 | To reduce overall power consumption, a motion sensor enables for the system to idle during bounds of inactivity
External battery pack | 1 | To take advantage of the lightweight nature of the components, an external battery pack enables portability and flexibility to the system
Wireless card | 1 | For external voice-to-text processing, an internet connection is required

## Design Considerations

The following are the initial design considerations that factor into the project:

* Power consumption for battery-powered operation
* Storage capability for prolonged usage
* Modularity of components

### Goals and Objectives

_TBD._

### Assumptions and Dependencies

_TBD._

### General Constraints

_TBD._

### Development Methodology

_TBD._

## Architecture

_TBD._

### System Design

_TBD._

### Data Design

_TBD._

### Program Design

_TBD._
