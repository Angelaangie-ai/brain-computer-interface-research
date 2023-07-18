# Brain Computer Interface Research

Applications of brain-computer interface (BCI) systems have grown in importance for assisting individuals with sever motor disabilities to navigate 
our increasingly technologically dependent society. With applications including electric wheelchairs and advanced prosthetics in mind, the goal of this 
research is to develop a system that enables the use of electroencephalography (EEG) and electromyography (EMG) signals to control the movement of a
robot. 

An EEG cap was used to obtain occipital alpha power density, frontal muscular artifacts, and sensorimotor mu rhythms to send back to a PC via Bluetooth for further pro-
cessing. Signal-processing algorithms were developed and implemented in order to determine the user’s mental activity. While the implementation with a physical robot has yet to be
completed, simulation data demonstrates promising results for future real-time asynchronous control of physical devices.

The cap has the following layout:

![Screenshot 2023-07-17 7 14 41 PM](https://github.com/Angelaangie-ai/brain-computer-interface-research/assets/64707998/cd7a3dc9-da6b-421f-83af-a5a98328a6c0)

In the image, the scatter plot on the left shows the original signal, and the scatter plot on the right shows the signal after CSP has been applied. The CSP algorithm has effectively separated the signal into two components, one that is more representative of left hand movement and one that is more representative of right hand movement. This makes it easier to classify the signal, as the classifier only needs to distinguish between two different classes instead of a large number of different classes.

![Screenshot 2023-07-17 7 34 22 PM](https://github.com/Angelaangie-ai/brain-computer-interface-research/assets/64707998/a627ff6f-3654-4814-bfd1-08e7d8f6611d)

Due to the noise present in the data, the difference is not the most visible. However, the CSP's impact can be seen in the following places:

|       | Original Signal | Signal after CSP |
|-------|------------|------------|
| Number of points | More points | Less points|
| Distribution of points | Uniform | Clustered |
| Variance | Larger Variance | Smaller Variance |


After we apply CSP to the data, we analyze it using Random Forest. The result are presented in the following graph:

![Screenshot 2023-07-17 7 26 47 PM](https://github.com/Angelaangie-ai/brain-computer-interface-research/assets/64707998/375aca54-d9ec-4e4f-aa33-6a79b8cb47a5)

The purple data represents right motor imagery, the green data represents left motor imagery. The random forest model is able to distinguish between right and left motor imagery by using different decision trees.

The accuracy results from the analyzed trials are the following:

|-------|------------|------------|
| 89.1556% % | 91.3414 % | 88.3556 % |
|-------|------------|------------|


