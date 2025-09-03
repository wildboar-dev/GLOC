# GLOC #

## What is it? ##

This is a *research project* into the notion of a *single-shot* optical camera calibration. This follows the spirit of [TSAI calibration](http://vigir.missouri.edu/~gdesouza/Research/Calibration/Calibration%20-%20%20Tsai.pdf) which requires a **single** calibration image. This is in constrast to what many consider to be the SOTA, [ZHANG calibration](https://ieeexplore.ieee.org/abstract/document/888718?casa_token=sZcv9w9haoMAAAAA:iW2IPB7BPfhBPFxHGic8RJlhk4xJeGw2okuME888kcJmd_0-EiICOikBpWfERygzMIInkMkQdQ), which typically uses about 12 images.

The main novelity of this work is:

* Distortion estimation prior to estimation of linear components.

## Utilities ##

The following are utilities that have been developed for the research project.

* **BCamera:** This is a *Pylon* based utility to capture images with *Basler* cameras.
* **CalScene:** Appears to load up a multi-scene data file and output points and board images. It does not appear to deal with distortion though.
* **CalScore:** Appears to calculate the errors associated with an *approximated* calibration.
* **MetaGen:** Generates a random scene with calibration parameters (current it does not appear to generate distortion).
* **PointConvert:** Appears to write the grid points of the meta file.
* **PointGen:** Appears to generate the points for a particular meta, but only one grid.


