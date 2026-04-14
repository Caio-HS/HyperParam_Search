# Neural Network Behavior Lab (FANN + C)

## Overview

This project is an experimental environment designed to study how neural networks behave in a **systematic, measurable, and reproducible** way.

Instead of building an application, the goal here is to understand what actually happens inside the training process when we change things like:

- the architecture of the network  
- the number of neurons and layers  
- activation functions  
- training algorithms  
- and the random initialization  

In other words, this is about understanding **why a model works (or not)**.

---

## Purpose

The project is meant to answer questions such as:

- Does adding more layers really improve accuracy?
- At what point does increasing the number of neurons stop being worth the cost?
- How do different activation functions modify the behavior of the training and execution?
- Is an improvement consistent, or just the result of a lucky random seed?
- When does a network start getting more expensive without actually getting better?

The focus is on the relationship between:

- network structure  
- data structure  
- training behavior  
- computational cost  
- and final performance  

---

## System Architecture

The project is divided into two main components.

### 1) Worker (written in C)

The worker is responsible for executing a single experiment.

It does the heavy work:

1. Receives the network configuration through command-line arguments  
2. Builds the neural network using FANN  
3. Initializes the weights using a controlled random seed  
4. Trains the network  
5. Measures CPU time  
6. Measures peak memory usage  
7. Tests the trained network  
8. Measures time and memory again  
9. Outputs the results  

The worker does **not** make decisions.  
It simply executes the experiment and returns the data.

---

### 2) Orchestrator (written in python)

The orchestrator is responsible for:

- generating combinations of hyperparameters  
- calling the worker repeatedly  
- storing the results    

In short: the worker runs experiments, the orchestrator analyzes them.

---

## What a Single Experiment Produces

Each execution of the worker returns measurable data such as:

- number of weights in the network  
- training time  
- memory used during training  
- testing time  
- memory used during testing
- volume of trained data
- training MSE  
- testing MSE  
- classification error (bit error)  
- random seed used for initialization  

This makes it possible to compare different architectures in a consistent and objective way.

---

## Why C + FANN?

The goal of this project is not fast development, but **full control over the experiment**.

Using C makes it possible to:

- measure CPU time precisely  
- measure real memory usage  
- avoid unnecessary runtime overhead  
- guarantee reproducibility  
- run experiments on very limited hardware  

FANN was chosen because it is:

- lightweight  
- fast  
- simple to control  
- CPU-Driven

---

## Motivation

A lot of neural network work today is mostly empirical: try parameters until something works.

This project takes a different approach, the question which is intended to be answered is:

> **“Why does this configuration work better than the others?”**
