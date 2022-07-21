# naiveBayes.py
# -------------
# Licensing Information: Please do not distribute or publish solutions to this
# project. You are free to use and extend these projects for educational
# purposes. The Pacman AI projects were developed at UC Berkeley, primarily by
# John DeNero (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and Pieter 
# Abbeel in Spring 2013.
# For more info, see http://inst.eecs.berkeley.edu/~cs188/pacman/pacman.html

import util
import classificationMethod
import math


class NaiveBayesClassifier(classificationMethod.ClassificationMethod):
	"""
	See the project description for the specifications of the Naive Bayes classifier.

	Note that the variable 'datum' in this code refers to a counter of features
	(not to a raw samples.Datum).
	"""
	
	def __init__(self, legalLabels):
		self.legalLabels = legalLabels
		self.type = "naivebayes"
		self.k = 1  # this is the smoothing parameter, ** use it in your train method **
		self.automaticTuning = False  # Look at this flag to decide whether to choose k automatically ** use this in your train method **
	
	# self.copy_label_count = None
	# self.P_cond = None
	
	def setSmoothing(self, k):
		"""
		This is used by the main method to change the smoothing parameter before training.
		Do not modify this method.
		"""
		self.k = k
	
	def train(self, trainingData, trainingLabels, validationData, validationLabels):
		"""
		Outside shell to call your method. Do not modify this method.
		"""
		
		# might be useful in your code later...
		# this is a list of all features in the training set.
		self.features = list(set([f for datum in trainingData for f in datum.keys()]));
		
		if (self.automaticTuning):
			kgrid = [0.001, 0.01, 0.05, 0.1, 0.5, 1, 2, 5, 10, 20, 50]
		else:
			kgrid = [self.k]
		
		self.trainAndTune(trainingData, trainingLabels, validationData, validationLabels, kgrid)
	
	def trainAndTune(self, trainingData, trainingLabels, validationData, validationLabels, kgrid):
		"""
		Trains the classifier by collecting counts over the training data, and
		stores the Laplace smoothed estimates so that they can be used to classify.
		Evaluate each value of k in kgrid to choose the smoothing parameter
		that gives the best accuracy on the held-out validationData.

		trainingData and validationData are lists of feature Counters.  The corresponding
		label lists contain the correct label for each datum.

		To get the list of all possible features or labels, use self.features and
		self.legalLabels.
		"""
		
		"*** YOUR CODE HERE ***"
		label_count = util.Counter()
		feature_label_count = util.Counter()
		feature_is1_label_count = util.Counter()
		# count everything needed
		for i in range(len(trainingData)):
			label_count[trainingLabels[i]] += 1  # count labels
			for datum in trainingData[i].items():
				feature_label_count[(datum[0], trainingLabels[i])] += 1  # count (feature, label)
				if datum[1] == 1:  # if pixel value is 1
					feature_is1_label_count[(datum[0], trainingLabels[i])] += 1  # count (feature, label==1)
		# evaluate k in kgrid
		k_ranking = []
		for k in kgrid:
			P_cond = feature_is1_label_count.copy()
			copy_label_count = label_count.copy()
			copy_feature_label_count = feature_label_count.copy()
			# do smoothing
			for label in self.legalLabels:
				for feature in self.features:
					copy_feature_label_count[(feature, label)] += 2 * k
					P_cond[(feature, label)] += k
			# calculate conditional Probabilities
			for datum, count in P_cond.items():
				P_cond[datum] = float(count) / float(copy_feature_label_count[datum])
			# do prediction
			self.copy_label_count = copy_label_count
			self.P_cond = P_cond
			pred = self.classify(validationData)
			# count correct prediction
			correct = 0
			for i in range(len(validationLabels)):
				if validationLabels[i] == pred[i]:
					correct += 1
			k_ranking.append((correct, (k, P_cond, copy_label_count)))
		best_k = sorted(k_ranking)[-1][1]
		self.k = best_k[0]
		self.P_cond = best_k[1]
		self.label_count = best_k[2]
	
	def classify(self, testData):
		"""
		Classify the data based on the posterior distribution over labels.

		You shouldn't modify this method.
		"""
		guesses = []
		self.posteriors = []  # Log posteriors are stored for later data analysis (autograder).
		for datum in testData:
			posterior = self.calculateLogJointProbabilities(datum)
			guesses.append(posterior.argMax())
			self.posteriors.append(posterior)
		return guesses
	
	def calculateLogJointProbabilities(self, datum):
		"""
		Returns the log-joint distribution over legal labels and the datum.
		Each log-probability should be stored in the log-joint counter, e.g.
		logJoint[3] = <Estimate of log( P(Label = 3, datum) )>

		To get the list of all possible features or labels, use self.features and
		self.legalLabels.
		"""
		logJoint = util.Counter()
		
		"*** YOUR CODE HERE ***"
		self.copy_label_count.normalize()
		for label in self.legalLabels:
			logJoint[label] = math.log(self.copy_label_count[label])
			for feature, value in datum.items():
				if value == 0:
					logJoint[label] += math.log(1 - self.P_cond[feature, label])
				else:
					logJoint[label] += math.log(self.P_cond[feature, label])
		return logJoint
	
	def findHighOddsFeatures(self, label1, label2):
		"""
		Returns the 100 best features for the odds ratio:
				P(feature=1 | label1)/P(feature=1 | label2)

		Note: you may find 'self.features' a useful way to loop through all possible features
		"""
		featuresOdds = []
		
		"*** YOUR CODE HERE ***"
		feature_ranking = []
		for feature in self.features:
			feature_ranking += (self.P_cond[feature, label1] / self.P_cond[feature, label2], feature)
		top100 = sorted(feature_ranking, reverse=True)[:100]
		for p_cond, feature in top100:
			featuresOdds.append(feature)  # just append the features not the value
		
		return featuresOdds
