ODee
====

Open Source measurement of bacterial OD600 with an arduino.

This code is written to work with the **Arduino Uno** and the **Arduino wifi-shield**.

It uses the webservice Cosm, now **Xively** to publish optical density measurements of a bacterial solution in an 15mL Falcon Tube.

The range of the measurement is extended beyond 1 Absorbtion Unit by initially using pulse width modulation to dim the illuminating LED.

Once the Abs is sufficiently high the code should switch to a higher intensity PWM.

The setup is extremely sensitive to the position of the LED and the photodiode at either sides of the tube, and additionally ambient light needs to be blocked out.

A hacked together 3D printed holder was used in my expierments however it was little more than a block with a hole for the falcon tube and two holes on a perpendicular axis for the LED and the photodiode.

The device does not perform massively well as I had little time to refine it, however I'm happy to collaborate by offering advice.
