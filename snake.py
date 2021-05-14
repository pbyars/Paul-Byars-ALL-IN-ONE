#Nick Ponder and Paul Byars
#CS 470
#Due: 12/01/2020
#Term Project: Snake AI
###################################################################################################################
import numpy as np
import tkinter as tk
import time
import math
import random as rand
###################################################################################################################
#constants
UP = (-1, 0)
DOWN = (1, 0)
LEFT = (0, -1)
RIGHT = (0, 1)
MOVE = [UP, DOWN, LEFT, RIGHT]
EMPTY = 0
FOOD = 99
##################################################################################################################
#game
class game:
    def __init__(self, theSize, numOfSnakes, thePlayer, gui = None, output = False, maximumTurns = 100):
        self.theSize = theSize
        self.numOfSnakes = numOfSnakes
        self.thePlayer = thePlayer
        self.gui = gui
        self.output = output
        self.maximumTurns = maximumTurns
        self.num_food = 4
        self.turn = 0
        self.snakeSize = 3

        self.snake = [[((j + 1) * self.theSize // (2 * self.numOfSnakes), self.theSize // 2 + i) 
                        for i in range(self.snakeSize)]
                       for j in range(self.numOfSnakes)]
        self.food = [(self.theSize // 4, self.theSize // 4), (3 * self.theSize // 4, self.theSize // 4), 
                        (self.theSize // 4, 3 * self.theSize // 4),
                     (3 * self.theSize // 4, 3 * self.theSize // 4)]
        self.partPlayer = [i for i in range(self.numOfSnakes)]
        self.theBoard = np.zeros([self.theSize, self.theSize])
        
        for i in self.partPlayer:
            for j in self.snake[i]:
                self.theBoard[j[0]][j[1]] = i + 1
                
        for j in self.food:
            self.theBoard[j[0]][j[1]] = FOOD

        self.primaryFood = 0
        self.foodPiece = ([(rand.randint(0, 9), rand.randint(0, 9)) for _ in range(200)])

#Name: move
#Incoming: self
#Outgoing: N/A
#Purpose: calculate the next move of the snake
#Return: the move that will be made
    def move(self):
        move = []
        
        for i in self.partPlayer:
            snake1 = self.snake[i]
            move1 = self.thePlayer[i].findNextMove(self.theBoard, snake1)
            move.append(move1)
            newPiece = (snake1[-1][0] + move1[0], snake1[-1][1] + move1[1])
            snake1.append(newPiece)
            
        for i in self.partPlayer:
            head1 = self.snake[i][-1]
            if head1 not in self.food:
                self.theBoard[self.snake[i][0][0]][self.snake[i][0][1]] = EMPTY
                self.snake[i].pop(0)
            else:
                self.food.remove(head1)
                
        for i in self.partPlayer:
            head1 = self.snake[i][-1]
            if head1[0] >= self.theSize or head1[1] >= self.theSize or head1[0] < 0 or head1[1] < 0:
                self.partPlayer.remove(i)
            else:
                self.theBoard[head1[0]][head1[1]] = i + 1
                
        for i in self.partPlayer:
            head1 = self.snake[i][-1]
            
            for j in range(self.numOfSnakes):
                if i == j:
                    if head1 in self.snake[i][:-1]:
                        self.partPlayer.remove(i)
                else:
                    if head1 in self.snake[j]:
                        self.partPlayer.remove(i)
                        
        while len(self.food) < self.num_food:
            X = self.foodPiece[self.primaryFood][0]
            Y = self.foodPiece[self.primaryFood][1]
            
            while self.theBoard[X][Y] != EMPTY:
                self.primaryFood += 1
                X = self.foodPiece[self.primaryFood][0]
                Y = self.foodPiece[self.primaryFood][1]
                
            self.food.append((X, Y))
            self.theBoard[X][Y] = FOOD
            self.primaryFood += 1
            
        return move

#Name: showMovement
#Incoming: self, output, end = False
#Outgoing: N/A
#Purpose: demonstrate the moevement of the snake through GUI
#Return: N/A
    def showMovement(self, output, end = False):
        while True:
            if end:
                for i in self.partPlayer:
                    if len(self.snake[0]) - self.turn / 20 <= 0:
                        self.partPlayer.remove(i)
                        
                        return -2
                    
            if len(self.partPlayer) == 0:
                return -1
            
            if self.turn >= self.maximumTurns:
                return 0
            
            move = self.move()
            self.turn += 1
            
            if output:
                for i in move:
                    if i == UP:
                        print("UP")
                        
                    elif i == RIGHT:
                        print("RIGHT")
                        
                    elif i == LEFT:
                        print("LEFT")
                        
                    else:
                        print("DOWN")
                        
            
                if self.gui is not None:
                    self.gui.update()
                    
                time.sleep(1)

class Gui:
    def __init__(self, theGame, theSize):
        self.theGame = theGame
        self.theGame.gui = self
        self.theSize = theSize
        self.gameSize = self.theSize / self.theGame.theSize
        self.visual = tk.Tk()
        self.display = tk.Canvas(self.visual, width = self.theSize, height = self.theSize)
        self.display.pack()

        for i in range(len(self.theGame.snake)):
            displayColor = '#' + '{0:03X}'.format((i + 1) * 500)
            snake = self.theGame.snake[i]
            self.display.create_rectangle(self.gameSize*(snake[-1][1]), self.gameSize*(snake[-1][0]), 
                                            self.gameSize*(snake[-1][1] + 1), self.gameSize*(snake[-1][0] + 1), 
                                            fill = displayColor)

            for j in range(len(snake) - 1):
                displayColor = '#' + '{0:03X}'.format((i + 1) * 123)
                self.display.create_rectangle(self.gameSize*(snake[j][1]), self.gameSize*(snake[j][0]), 
                                                self.gameSize*(snake[j][1] + 1), self.gameSize*(snake[j][0] + 1), 
                                                fill = displayColor)        
        for i in self.theGame.food:
            self.display.create_rectangle(self.gameSize*(i[1]), self.gameSize*(i[0]), self.gameSize*(i[1] + 1), 
                                            self.gameSize*(i[0] + 1), fill = '#000000000')

#Name: update
#Incoming: self
#Outgoing: N/A
#Purpose: display calculation
#Return: N/A
    def update(self):
        self.display.delete("all")
        
        for i in range(len(self.theGame.snake)):
            displayColor = '#' + '{0:03X}'.format((i + 1) * 500)
            snake = self.theGame.snake[i]
            self.display.create_rectangle(self.gameSize*(snake[-1][1]), self.gameSize*(snake[-1][0]), 
                                            self.gameSize*(snake[-1][1] + 1), self.gameSize*(snake[-1][0] + 1),
                                            fill = displayColor)

            for j in range(len(snake) - 1):
                displayColor = '#' + '{0:03X}'.format((i + 1) * 123)
                self.display.create_rectangle(self.gameSize * (snake[j][1]), self.gameSize * (snake[j][0]), 
                                                self.gameSize * (snake[j][1] + 1), self.gameSize * (snake[j][0] + 1),
                                                fill = displayColor)  

        for i in self.theGame.food:
            self.display.create_rectangle(self.gameSize * (i[1]), self.gameSize * (i[0]), self.gameSize * (i[1] + 1), 
                                            self.gameSize * (i[0] + 1), fill = '#000000000')
            
        self.display.pack()
        self.visual.update()
####################################################################################################################
#player
class RandomPlayer:
    def __init__(self, i):
        self.i = i

#Name: findNextMove
#Incoming: self, the Board, snake
#Outgoing: N/A
#Purpose: calculates the next move
#Return: MOVE[randomNum]
    def findNextMove(self, theBoard, snake):
        randomNum = rand.randint(0, 3)
        
        return MOVE[randomNum]

class GeneticPlayer:
    def __init__(self, popFromList, numberOfGenerations, numberOfTrials, view, hiddenLayer, gridSize, 
                    mutation = 0.1, mutationSize = 0.1):
        self.popFromList = popFromList
        self.numberOfGenerations = numberOfGenerations
        self.numberOfTrials = numberOfTrials
        self.view = view
        self.hiddenLayer = hiddenLayer
        self.gridSize = gridSize
        self.mutation = mutation
        self.mutationSize = mutationSize
        self.output = False
        self.engineInUse = None
        self.pop = [self.createEngine(self.view ** 2, self.hiddenLayer, len(MOVE)) for i in range(self.popFromList)]

#Name: createEngine
#Incoming: self, input_size, hiddenLayer, output_size
#Outgoing: N/A
#Purpose: builds the "engine"
#Return: hiddenLayer1, hiddenLayer2, displayLayer
    def createEngine(self, input_size, hiddenLayer, output_size):
        hiddenLayer1 = np.array([[rand.uniform(-1, 1) for i in range(input_size + 1)] for i in range(hiddenLayer)])
        hiddenLayer2 = np.array([[rand.uniform(-1, 1) for i in range(hiddenLayer + 1)] for i in range(hiddenLayer)])
        displayLayer = np.array([[rand.uniform(-1, 1) for i in range(hiddenLayer + 1)] for i in range(output_size)])
        
        return[hiddenLayer1, hiddenLayer2, displayLayer]

#Name: findNextMove
#Incoming: self, the Board, snake
#Outgoing: N/A
#Purpose: calculates the next move
#Return: MOVE[randomNum]
    def findNextMove(self, theBoard, snake):
        directionalVector = self.interpretFields(theBoard, snake[-1][0], snake[-1][1], snake[-2][0], snake[-2][1])
        
        hiddenLayer1 = self.engineInUse[0]
        hiddenLayer2 = self.engineInUse[1]
        displayLayer = self.engineInUse[2]

        hiddenLayer1Actual = np.array([math.tanh(np.dot(directionalVector, hiddenLayer1[i])) 
                                for i in range(hiddenLayer1.shape[0])] + [1])
        hiddenLayer2Actual = np.array([math.tanh(np.dot(hiddenLayer1Actual, hiddenLayer2[i])) 
                                for i in range(hiddenLayer2.shape[0])] + [1])
        outputActual = np.array([np.dot(hiddenLayer2Actual, displayLayer[i]) 
                                for i in range(displayLayer.shape[0])])
  
        maximumValue = np.argmax(outputActual)
        
        return MOVE[maximumValue]

#Name: interpretFields
#Incoming: self, theBoard, x1, y1, x2, y2
#Outgoing: N/A
#Purpose: explain the fields and determine if the snake can continue
#Return: np.array(directionalVector)
    def interpretFields(self, theBoard, x1, y1, x2, y2):
        directionalVector = [[0 for i in range(self.view)] for i in range(self.view)]

        for i in range(self.view):
            for j in range(self.view):
                a = x1 + i - self.view // 2
                b = y1 + j - self.view // 2

                if a < 0 or b < 0 or a >= self.gridSize or b >= self.gridSize:
                    directionalVector[i][j] = -1
                    
                elif theBoard[a][b] == FOOD:
                    directionalVector[i][j] = 1
                    
                elif theBoard[a][b] == EMPTY:
                    directionalVector[i][j] = 0
                    
                else:
                    directionalVector[i][j] = -1
            
        directionalVector = list(np.array(directionalVector).flatten()) + [1]
        
        return np.array(directionalVector)

#Name: reproduceCreatedValues
#Incoming: self, bestValues
#Outgoing: N/A
#Purpose: remake the created values to use
#Return: popFromNewValues
    def reproduceCreatedValues(self, bestValues):
        popFromNewValues = []
        
        for i in bestValues:
            popFromNewValues.append(i)
            
        for i in bestValues:
            laterEngine = self.mutate(i)
            popFromNewValues.append(laterEngine)
            
        for i in range(self.popFromList // 2):
            popFromNewValues.append(self.createEngine(self.view ** 2, self.hiddenLayer, len(MOVE)))
            
        return popFromNewValues

#Name: mutate
#Incoming: self, engine
#Outgoing: N/A
#Purpose: calculate the mutation
#Return: laterEngine
    def mutate(self, engine):
        laterEngine = []
        
        for layer in engine:
            createNewLayer = np.copy(layer)
            
            for i in range(createNewLayer.shape[0]):
                for j in range(createNewLayer.shape[1]):
                    if rand.uniform(0, 1) < self.mutation:
                        createNewLayer[i][j] += rand.uniform(-1, 1) * self.mutationSize
                        
            laterEngine.append(createNewLayer)
            
        return laterEngine

#Name: singleGeneration
#Incoming: self
#Outgoing: N/A
#Purpose: calculate a single generation of values
#Return: N/A
    def singleGeneration(self):
        values = [0 for i in range(self.popFromList)]
        maximumValues = 0
        
        for i in range(self.popFromList):
            for j in range(self.numberOfTrials):
                self.engineInUse = self.pop[i]
                theGame = game(self.gridSize, 1, [self])
                outcome = theGame.showMovement(False, end = True)
                num = len(theGame.snake[0])
                values[i] += num

        firstBestValues = list(np.argsort(values))[3 * (self.popFromList//4):self.popFromList]

        print("Length:", values[0])
        bestValues = [self.pop[i] for i in firstBestValues][::-1]
        self.pop = self.reproduceCreatedValues(bestValues)

#Name: valuePop
#Incoming: self
#Outgoing: N/A
#Purpose: output the generation number and maximum length
#Return: N/A
    def valuePop(self):
#for loop to calculate the generation number
        for i in range(self.numberOfGenerations):
            self.singleGeneration()
            print("Generation:", i)
            print("")

#for loop to calculate the maximum length
        for i in self.pop:
            self.output = True
            self.engineInUse = i
            theGame = game(self.gridSize, 1, [self], output = True)
            gui = Gui(theGame, 800)
            theGame.showMovement(True, end = True)
            print("Maximum length:", len(theGame.snake[0]))
            print("")
####################################################################################################################
#main
popFromList = 100
#####################
numberOfGenerations = 500
#####################
numberOfTrials = 1
view = 7
hiddenLayer = 15
gridSize = 10
computerPlayer = GeneticPlayer(popFromList, numberOfGenerations, numberOfTrials, view, hiddenLayer, gridSize, 
                    mutation = 0.1, mutationSize = 0.1)
computerPlayer.valuePop()
####################################################################################################################
#END OF PROGRAM
