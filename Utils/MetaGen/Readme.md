# MetaGen #

The purpose of this utility is to generate the meta data for new *random* scenes for us to experiment against. This meta data can then theoretically be converted into points and images using the CalScene utility.

## Execution ##

To execute the utility, simply build it and run it. The ArgReader allows one to specify the name of the dataset. Multiple files are generated with the date and time as the associated filename.

## Output ##

An example of the output file is as follows:

```xml
<opencv_storage>
    <focal>7.8720000000000005e+02</focal>
    <imageSize> 640 480</imageSize>
    <rvec_1> 0. 9.1430184639269541e-01 6.5407747472708211e+00</rvec_1>
    <tvec_1> -49. -7. 394.</tvec_1>
    <rvec_2> 4.9193172030137777e-01 1.0931816006697284e-01 2.6509653816240912e+00</rvec_2>
    <tvec_2> 2. -14. 256.</tvec_2>
    <blockSize>5</blockSize>
    <gridSize>10</gridSize>
    <distortion> -3.3000000000000002e-01 -2.8999999999999998e-01 1.4000000000000001e-01 -8.1000000000000005e-01</distortion>
</opencv_storage>
```