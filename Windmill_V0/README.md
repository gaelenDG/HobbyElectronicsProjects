---
title: Elia Faucher Windmill
author: Gaelen Guzman
date: 2026-01-19
---

## Scoping

For once I am going to try to list out the features of this little display thing *before* trying to make it.

### Features

1. ESP32-S3 controller board -- will be responsible for toggling the on/off state of the lights & motor, reading the rotary encoder dial for brightness/speed, and handling battery management(?)

2. A high-torque DC motor with in-built gearbox -- to spin the windmill

3. An H-bridge motor driver to make sure I don't cook the ESP32 by overdrawing current.

4. A set of 3 white edison filaments spinning around the axle of the DC motor

5. A rotary encoder to enable on/off toggling and brightness & speed control

6. Bare copper wire as the support/structure of the whole device, with current transmission through the structure (mostly connected to ground, obviously -- just a +3.3V lines).

7. An over-current protector -- detect and trigger power cutoff if there's a short.

8. A LiPo battery (?????) to power on the go? Is this wise? Maybe not...
