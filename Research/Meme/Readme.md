# Meme #

This is a gradient-based approach for solving for *distortion* on grid shaped images. The basic strategy of using traditional gradient descent is known to be unreliable, because there are clearly *local-minima* in the cost-function.

MEmE (Minima Escape Model Estimation) attempts to remedy this problem by training a cascading optimization machine learnt optimizing and using this in conjuction with a traditional gradient descent approach - with the hope that this system will be able to leap out of local minima. 

Additional some basic visualization techniques will be used to see if this can shed some light on the complexity of the error space, along with the minima that we are aiming to escape from. 
