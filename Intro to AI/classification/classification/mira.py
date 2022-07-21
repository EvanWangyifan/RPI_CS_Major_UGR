# mira.py
# -------
# Licensing Information: Please do not distribute or publish solutions to this
# project. You are free to use and extend these projects for educational
# purposes. The Pacman AI projects were developed at UC Berkeley, primarily by
# John DeNero (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and Pieter 
# Abbeel in Spring 2013.
# For more info, see http://inst.eecs.berkeley.edu/~cs188/pacman/pacman.html

# Mira implementation
import util
PRINT = True

class MiraClassifier:
    """
    Mira classifier.

    Note that the variable 'datum' in this code refers to a counter of features
    (not to a raw samples.Datum).
    """
    def __init__( self, legalLabels, max_iterations):
        self.legalLabels = legalLabels
        self.type = "mira"
        self.automaticTuning = False
        self.C = 0.001
        self.legalLabels = legalLabels
        self.max_iterations = max_iterations
        self.initializeWeightsToZero()

    def initializeWeightsToZero(self):
        "Resets the weights of each label to zero vectors"
        self.weights = {}
        for label in self.legalLabels:
            self.weights[label] = util.Counter() # this is the data-structure you should use

    def train(self, trainingData, trainingLabels, validationData, validationLabels):
        "Outside shell to call your method. Do not modify this method."

        self.features = trainingData[0].keys() # this could be useful for your code later...

        if (self.automaticTuning):
            Cgrid = [0.002, 0.004, 0.008]
        else:
            Cgrid = [self.C]

        return self.trainAndTune(trainingData, trainingLabels, validationData, validationLabels, Cgrid)

    def trainAndTune(self, trainingData, trainingLabels, validationData, validationLabels, Cgrid):
        """
        This method sets self.weights using MIRA.  Train the classifier for each value of C in Cgrid,
        then store the weights that give the best accuracy on the validationData.

        Use the provided self.weights[label] data structure so that
        the classify method works correctly. Also, recall that a
        datum is a counter from features to values for those features
        representing a vector of values.
        """
        "*** YOUR CODE HERE ***"
        label_outcomes = util.Counter()
        c_weights = util.Counter()
        c_ranking = []
        
        for c in Cgrid:
            for iteration in range(self.max_iterations):
                for i in range(len(trainingData)):
                    for label in self.legalLabels:
                        label_outcomes[label] = trainingData[i].__mul__(self.weights[label])
                    pred = label_outcomes.argMax()
                    if trainingLabels[i] == pred:
                        continue
                    else:
                        t = ((self.weights[pred].__sub__(self.weights[trainingLabels[i]])).__mul__(trainingData[i]))+1.0
                        t /= (2.0*trainingData[i].__mul__(trainingData[i]))
                        print(trainingData[i], "!!!!!!!!!")
                        if c < t:
                            t = c
                        tf = trainingData[i].copy()
                        tf.update((feature, value*t) for feature, value in tf.items())
                        
                        self.weights[trainingLabels[i]] += tf
                        self.weights[pred] -= tf
                
                validation_label_outcomes = util.Counter()
                correct = 0
                for i in range(len(validationData)):
                    for label in self.legalLabels:
                        validation_label_outcomes[label] = validationData[i].__mul__(self.weights[label])
                    valid_pred = validation_label_outcomes.argMax()
                    if valid_pred == validationLabels[i]:
                        correct += 1
            c_weights[c] = self.weights
            c_ranking.append((correct, c))
        best_c = sorted(c_ranking)[-1][1]
        self.weights = c_weights[best_c]
        
        
        
    def classify(self, data):
        """
        Classifies each datum as the label that most closely matches the prototype vector
        for that label.  See the project description for details.

        Recall that a datum is a util.counter...
        """
        guesses = []
        for datum in data:
            vectors = util.Counter()
            for l in self.legalLabels:
                vectors[l] = self.weights[l] * datum
            guesses.append(vectors.argMax())
        return guesses


    def findHighOddsFeatures(self, label1, label2):
        """
        Returns a list of the 100 features with the greatest difference in feature values
                         w_label1 - w_label2

        """
        featuresOdds = []
        
        "*** YOUR CODE HERE ***"

        return featuresOdds
