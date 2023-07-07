from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import gamelib

import abc
import tensorflow as tf
import numpy as np

import json

from tf_agents.environments import py_environment
from tf_agents.environments import tf_environment
from tf_agents.environments import tf_py_environment
from tf_agents.environments import utils
from tf_agents.specs import array_spec
from tf_agents.environments import wrappers
from tf_agents.environments import suite_gym
from tf_agents.trajectories import time_step as ts

def game_state_to_observation(turn_state : str, game_state: gamelib.GameState):

  arr = json.loads(turn_state)
  
  # 0 -> 839
  #pairwise by 2
  #(type, health)
  input = []
  map = enumerate(game_state.game_map)
  for e in map:
      #e is (index, [x, y])
      unit : gamelib.GameUnit = game_state.contains_stationary_unit(e[1])
      if not unit:
        input.append(0)
        input.append(0)
        continue 

      upgradeModifier = 3 if unit.upgraded else 0

      type = unit.unit_type
      if type == 'FF':
        input.append(1 + upgradeModifier)
      elif type == 'EF':
        input.append(2 + upgradeModifier)
      elif type == 'DF':
        input.append( 3 + upgradeModifier)

      input.append(int(unit.health))


  #840 -> 842
  assert(len(input)==840)
  for i in range(arr["p1Stats"]-1):
    input.append(int(arr["p1Stats"][i]))
  
  for i in range(arr["p2Stats"]-1):
    input.append(int(arr["p2Stats"][i]))

  #846 -> 901
  assert(len(input)==846)
  for i in range(56):
    input.append(0)

  we_score = 0
  they_score = 0
  for e in turn_state['events']['breach']:

    #[[x, y], ...]
    id = e[0][0]

    if e[4] ==1:
      id += 28
      we_score += 1
    else:
      they_score += 1
    # if player number is 2 it breached on our side. if it's 1 we breached them 
  
    input[846 + id] += 1

  reward = we_score-they_score

  input_np = np.array(input, dtype=np.int32)

  if turn_state['turnInfo'][0] == 2:
    winner = turn_state["endStats"]["winner"]
    win_bonus = 10 if winner == 1 else -10
    return ts.termination(input_np, reward=reward+win_bonus)
  else:
    return ts.transition(input_np, reward=reward, discount=0.9)
    

class GameEnv(py_environment.PyEnvironment):

  def __init__(self):
    self._action_spec = array_spec.BoundedArraySpec(
        shape=(), dtype=np.int32, minimum=0, maximum=1, name='action')
    
    self._observation_spec = array_spec.BoundedArraySpec(
        shape=(1,), dtype=np.int32, minimum=0, name='observation')
    
    self._episode_ended = False
  

  def action_spec(self):
    return self._action_spec

  def observation_spec(self):
    return self._observation_spec

  def _reset(self):
    #run match

    self._episode_ended = False

    #return observation
    return ts.restart(np.array([self._state], dtype=np.int32))

  def _step(self, action):

    #wait for function to be called from algo_strategy to on_turn

    game_state_to_observation()#stuff that you got from the algo_strategy)
    #updates action variable

    if self._episode_ended:
      # The last action ended the episode. Ignore the current action and start
      # a new episode.
      return self.reset()

    # Make sure episodes don't go on forever.
    # if action == 1:
    #   self._episode_ended = True
    # elif action == 0:
    #   new_card = np.random.randint(1, 11)
    #   self._state += new_card
    # else:
    #   raise ValueError('`action` should be 0 or 1.')

    if self._episode_ended or self._state >= 21:
      reward = self._state - 21 if self._state <= 21 else -21
      return ts.termination(np.array([self._state], dtype=np.int32), reward)
    else:
      return ts.transition(
          np.array([self._state], dtype=np.int32), reward=0.0, discount=1.0)

# py_env = GameEnv()
# tf_env = tf_py_environment.TFPyEnvironment(py_env)

# utils.validate_py_environment(py_env, episodes=5)

# tensorflow code that creates environment and runs it
