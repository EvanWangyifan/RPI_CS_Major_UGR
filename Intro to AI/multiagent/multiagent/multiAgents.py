# multiAgents.py
# --------------
# Licensing Information:  You are free to use or extend these projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UC Berkeley, including a link to
# http://inst.eecs.berkeley.edu/~cs188/pacman/pacman.html
#
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and
# Pieter Abbeel (pabbeel@cs.berkeley.edu).


from util import manhattanDistance
from game import Directions
import random, util

from game import Agent


class ReflexAgent(Agent):
    """
      A reflex agent chooses an action at each choice point by examining
      its alternatives via a state evaluation function.

      The code below is provided as a guide.  You are welcome to change
      it in any way you see fit, so long as you don't touch our method
      headers.
    """

    def getAction(self, gameState):
        """
        You do not need to change this method, but you're welcome to.

        getAction chooses among the best options according to the evaluation function.

        Just like in the previous project, getAction takes a GameState and returns
        some Directions.X for some X in the set {North, South, West, East, Stop}
        """
        # Collect legal moves and successor states
        legalMoves = gameState.getLegalActions()

        # Choose one of the best actions
        scores = [self.evaluationFunction(gameState, action) for action in legalMoves]
        bestScore = max(scores)
        bestIndices = [index for index in range(len(scores)) if scores[index] == bestScore]
        chosenIndex = random.choice(bestIndices)  # Pick randomly among the best

        "Add more of your code here if you want to"

        return legalMoves[chosenIndex]

    def evaluationFunction(self, currentGameState, action):
        """
        Design a better evaluation function here.

        The evaluation function takes in the current and proposed successor
        GameStates (pacman.py) and returns a number, where higher numbers are better.

        The code below extracts some useful information from the state, like the
        remaining food (newFood) and Pacman position after moving (newPos).
        newScaredTimes holds the number of moves that each ghost will remain
        scared because of Pacman having eaten a power pellet.

        Print out these variables to see what you're getting, then combine them
        to create a masterful evaluation function.
        """
        # Useful information you can extract from a GameState (pacman.py)
        successorGameState = currentGameState.generatePacmanSuccessor(action)
        newPos = successorGameState.getPacmanPosition()
        newFood = successorGameState.getFood()
        newGhostStates = successorGameState.getGhostStates()
        newScaredTimes = [ghostState.scaredTimer for ghostState in newGhostStates]

        "*** YOUR CODE HERE ***"
        # print("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
        # print("successorGameState.getScore():", successorGameState.getScore())
        # print("newPos:", newPos)
        # print("newFood:", newFood.count())
        # print("getGhostPosition:", currentGameState.getGhostPositions())
        # print("newScaredTimes:", newScaredTimes)

        # add getScore
        master_score = successorGameState.getScore()
        # add parametrized nearest food distance
        foodDisList = []
        min_foodDis = 999999999
        for foodPos in newFood.asList():
            foodDisList.append(util.manhattanDistance(newPos, foodPos))
        if len(foodDisList) != 0:
            min_foodDis = min(foodDisList)
        if min_foodDis == 0:
            min_foodDis = 0.1
        master_score += 10 / min_foodDis
        # deduct parametrized food count
        master_score -= newFood.count() / 10
        # deduct parametrized nearest ghost distance
        ghostDisList = []
        min_ghostDis = 999999999
        for ghostPos in successorGameState.getGhostPositions():
            ghostDisList.append(util.manhattanDistance(newPos, ghostPos))
        if len(ghostDisList) != 0:
            min_ghostDis = min(ghostDisList)
        if min_ghostDis == 0:
            min_ghostDis = 0.1
        master_score -= 1 / (min_ghostDis * 4)
        # add all scaredTimes
        for t in newScaredTimes:
            master_score += t
        # return the masterful evaluation score
        return master_score


def scoreEvaluationFunction(currentGameState):
    """
      This default evaluation function just returns the score of the state.
      The score is the same one displayed in the Pacman GUI.

      This evaluation function is meant for use with adversarial search agents
      (not reflex agents).
    """
    return currentGameState.getScore()


class MultiAgentSearchAgent(Agent):
    """
      This class provides some common elements to all of your
      multi-agent searchers.  Any methods defined here will be available
      to the MinimaxPacmanAgent, AlphaBetaPacmanAgent & ExpectimaxPacmanAgent.

      You *do not* need to make any changes here, but you can if you want to
      add functionality to all your adversarial search agents.  Please do not
      remove anything, however.

      Note: this is an abstract class: one that should not be instantiated.  It's
      only partially specified, and designed to be extended.  Agent (game.py)
      is another abstract class.
    """

    def __init__(self, evalFn='scoreEvaluationFunction', depth='2'):
        self.index = 0  # Pacman is always agent index 0
        self.evaluationFunction = util.lookup(evalFn, globals())
        self.depth = int(depth)


class MinimaxAgent(MultiAgentSearchAgent):
    """
      Your minimax agent (question 2)
    """
    # Minimax helper function for getAction()
    def MM(self, gameState, agentIndex, currentDepth, layer):
        # leaf node -> base case
        if gameState.isWin() or gameState.isLose() or self.depth == currentDepth:
            return self.evaluationFunction(gameState)
        # pacman -> max node
        elif agentIndex == 0:
            # find max
            max_list = []
            # print(layer)
            for action in gameState.getLegalActions(agentIndex):
                max_list.append(self.MM(gameState.generateSuccessor(agentIndex, action), 1, currentDepth, layer+1))
            return max(max_list)
        # ghost -> min node
        else:
            # add depth if all ghost has been went through
            if agentIndex + 1 == gameState.getNumAgents():
                currentDepth += 1
            # find min
            min_list = []
            # print(layer)
            for action in gameState.getLegalActions(agentIndex):
                min_list.append(self.MM(gameState.generateSuccessor(agentIndex, action), (agentIndex + 1) % gameState.getNumAgents(), currentDepth, layer+1))
            return min(min_list)

    def getAction(self, gameState):
        """
          Returns the minimax action from the current gameState using self.depth
          and self.evaluationFunction.
          Here are some method calls that might be useful when implementing minimax.
          gameState.getLegalActions(agentIndex):
            Returns a list of legal actions for an agent
            agentIndex=0 means Pacman, ghosts are >= 1
          gameState.generateSuccessor(agentIndex, action):
            Returns the successor game state after an agent takes an action
          gameState.getNumAgents():
            Returns the total number of agents in the game
        """
        "*** YOUR CODE HERE ***"
        # return the next move with the max MM value
        l_MM_state = []
        for successorState in gameState.getLegalActions(0):
            l_MM_state.append((self.MM(gameState.generateSuccessor(0, successorState), 1, 0, 0), successorState))
        l_MM_state.sort(reverse=True)
        return l_MM_state[0][1]


class AlphaBetaAgent(MultiAgentSearchAgent):
    """
      Your minimax agent with alpha-beta pruning (question 3)
    """
    # Minimax helper function for getAction()
    def MM_alpha_beta(self, gameState, agentIndex, currentDepth, layer, alpha, beta):
        # leaf node -> base case
        if gameState.isWin() or gameState.isLose() or self.depth == currentDepth:
            return self.evaluationFunction(gameState)
        # pacman -> max node
        elif agentIndex == 0:
            # find max
            max_alpha = -99999999
            # print(layer)
            for action in gameState.getLegalActions(agentIndex):
                max_alpha = max(max_alpha, self.MM_alpha_beta(gameState.generateSuccessor(agentIndex, action), 1,
                                                                            currentDepth, layer + 1, alpha, beta))
                if max_alpha > alpha:  # update to maximum alpha
                    alpha = max_alpha
                if max_alpha > beta:  # cut here !!!!!!
                    return max_alpha
            return max_alpha
        # ghost -> min node
        else:
            # add depth if all ghost has been went through
            if agentIndex + 1 == gameState.getNumAgents():
                currentDepth += 1
            # find min
            min_beta = 999999999
            # print(layer)
            for action in gameState.getLegalActions(agentIndex):
                min_beta = min(min_beta, self.MM_alpha_beta(gameState.generateSuccessor(agentIndex, action),
                                    (agentIndex + 1) % gameState.getNumAgents(), currentDepth, layer + 1, alpha, beta))
                if min_beta < beta:  # update to minimum beta
                    beta = min_beta
                if alpha > min_beta:  # cut here !!!!!!
                    return min_beta
            return min_beta

    def getAction(self, gameState):
        """
          Returns the minimax action using self.depth and self.evaluationFunction
        """
        "*** YOUR CODE HERE ***"
        alpha = -9999999999
        alpha_move = []
        for action in gameState.getLegalActions(0):
            minimax_alpha_beta = self.MM_alpha_beta(gameState.generateSuccessor(0, action), 1, 0, 0, alpha, 9999999999)
            alpha_move.append((minimax_alpha_beta, action))
            if minimax_alpha_beta > alpha:
                alpha = minimax_alpha_beta
        alpha_move.sort(reverse=True)
        return alpha_move[0][1]


class ExpectimaxAgent(MultiAgentSearchAgent):
    """
      Your expectimax agent (question 4)
    """

    # Minimax helper function for getAction()
    def expectimax(self, gameState, agentIndex, currentDepth, layer):
        # leaf node -> base case
        if gameState.isWin() or gameState.isLose() or self.depth == currentDepth:
            return self.evaluationFunction(gameState)
        # pacman -> max node
        elif agentIndex == 0:
            # find max
            max_list = []
            # print(layer)
            for action in gameState.getLegalActions(agentIndex):
                max_list.append(self.expectimax(gameState.generateSuccessor(agentIndex, action), 1, currentDepth, layer + 1))
            return max(max_list)
        # ghost -> min node
        else:
            # add depth if all ghost has been went through
            if agentIndex + 1 == gameState.getNumAgents():
                currentDepth += 1
            # find min
            min_list = []
            # print(layer)
            for action in gameState.getLegalActions(agentIndex):
                min_list.append(self.expectimax(gameState.generateSuccessor(agentIndex, action),
                                        (agentIndex + 1) % gameState.getNumAgents(), currentDepth, layer + 1))
            return sum(min_list) / len(min_list)

    def getAction(self, gameState):
        """
          Returns the expectimax action using self.depth and self.evaluationFunction

          All ghosts should be modeled as choosing uniformly at random from their
          legal moves.
        """
        "*** YOUR CODE HERE ***"
        # return the next move with the max MM value
        l_MM_state = []
        for successorState in gameState.getLegalActions(0):
            l_MM_state.append((self.expectimax(gameState.generateSuccessor(0, successorState), 1, 0, 0), successorState))
        l_MM_state.sort(reverse=True)
        return l_MM_state[0][1]


def betterEvaluationFunction(currentGameState):
    """
      Your extreme ghost-hunting, pellet-nabbing, food-gobbling, unstoppable
      evaluation function (question 5).

      DESCRIPTION: <This is basically the same weighted linear combination I used as flex agent
                    except that I use currentGameState instead of successorGameState after taking actions.
                    The formula is:

                    currentScore + 10/minFoodMHTDis - remainingFoodCount/10 - 1/(minGhostMHTDis*4) + ScaredTimes

                    I adjusted the coefficients multiple times to achieve the 'optimal' outcomes
                    >
    """
    "*** YOUR CODE HERE ***"
    # Useful information you can extract from a GameState (pacman.py)
    newPos = currentGameState.getPacmanPosition()
    newFood = currentGameState.getFood()
    newGhostStates = currentGameState.getGhostStates()
    newScaredTimes = [ghostState.scaredTimer for ghostState in newGhostStates]

    # add getScore
    master_score = currentGameState.getScore()
    # add parametrized nearest food distance
    foodDisList = []
    min_foodDis = 999999999
    for foodPos in newFood.asList():
        foodDisList.append(util.manhattanDistance(newPos, foodPos))
    if len(foodDisList) != 0:
        min_foodDis = min(foodDisList)
    if min_foodDis == 0:
        min_foodDis = 0.1
    master_score += 10 / min_foodDis
    # deduct parametrized food count
    master_score -= newFood.count() / 10
    # deduct parametrized nearest ghost distance
    ghostDisList = []
    min_ghostDis = 999999999
    for ghostPos in currentGameState.getGhostPositions():
        ghostDisList.append(util.manhattanDistance(newPos, ghostPos))
    if len(ghostDisList) != 0:
        min_ghostDis = min(ghostDisList)
    if min_ghostDis == 0:
        min_ghostDis = 0.1
    master_score -= 1 / (min_ghostDis * 4)
    # add all scaredTimes
    for t in newScaredTimes:
        master_score += t
    # return the masterful evaluation score
    return master_score


# Abbreviation
better = betterEvaluationFunction


class ContestAgent(MultiAgentSearchAgent):
    """
      Your agent for the mini-contest
    """

    def getAction(self, gameState):
        """
          Returns an action.  You can use any method you want and search to any depth you want.
          Just remember that the mini-contest is timed, so you have to trade off speed and computation.

          Ghosts don't behave randomly anymore, but they aren't perfect either -- they'll usually
          just make a beeline straight towards Pacman (or away from him if they're scared!)
        """
        "*** YOUR CODE HERE ***"
        util.raiseNotDefined()

