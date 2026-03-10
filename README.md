This repository holds the code for the agent based modelling of the effect of different types of social cognition on cooperative hunting behaviour. The model forms the core of the paper ~~DOI to be provided on publication~~

# Basic use

The code is written in plain C++ and should compile and run without any special libraries.
If run with no alterations, the software will run 10,000 simulations for each of the four different types of social cognition (see published paper for a description of these). It will also save the results to a text file.

# Main classes

## World
Class that describes the simulation world, with all the animals in it

## Animal
The Animal class includes both predators and prey. The common functions are to do with movement

## Predator
Predator is derived from class Animal

## Prey
Prey is derived from class Animal

## PredatorSocialCognition
Holds the affinity network that defines the relationships between individual predators

## PreyCornered
A class for deciding whether any of the prey agents are in a status of being cornered, or "caught"
To be cornered, the prey should have been surrounded by a certain number of predators for a certain number of turns

## Position
Simple class for manipulating 2d locations