# Formal Design Specification

## Content
1. [Introduction](#introduction)
2. [System overview](#system-overview)
3. [Design Considerations](#design-considerations)
4. [Goals and Objectives](#goals-and-objectives)
5. [Assumptions and Dependencies](#assumptions-and-dependencies)
6. [General Constraints](#general-constraints)
7. [Development Methodology](#development-methodology)
8. [Architecture](#Architecture)
9. [System Design](#system-design)
10. [Data Design](#data-design)
11. [Program Design](#program-design)

## <a id="introduction"></a>Introduction

Our project attempts to modernize the phonograph, by introducing subject tracking and voice-to-text translation controlled by the Raspberry PI. 

The following are some of the use cases the project is attempting to address and cater for:

* Lecture theatre and tutorial recording and note taking in absence of Echo360 deployment
* Cost-effective portable recording solution for talks and presentations

## <a id="system-overview"></a>System Overview

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

## <a id="design-considerations"></a>Design Considerations

The following are the initial design considerations that factor into the project:

* Power consumption for battery-powered operation
* Storage capability for prolonged usage
* Modularity of components

### <a id="goals-and-objectives"></a>Goals and Objectives

This project aims to provide an easy way to audio record and annotate lectures, talks and other presentations. The platform is to be relatively portable, require minimal setup and start-up time.
_TBD._

### <a id="assumptions-and-dependencies"></a>Assumptions and Dependencies

_TBD._

### <a id="general-constraints"></a>General Constraints

_TBD._

### <a id="development-methodology"></a>Development Methodology

_TBD._

## <a id="architecture"></a>Architecture

_TBD._

### <a id="system-design"></a>System Design

_TBD._

### <a id="data-design"></a>Data Design

_TBD._

### <a id="program-design"></a>Program Design

_TBD._
