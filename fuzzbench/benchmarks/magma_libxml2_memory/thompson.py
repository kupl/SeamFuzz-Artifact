from collections import defaultdict
from typing import List
from uuid import uuid4
import sys
import random

import numpy as np

from abc import (
    ABC,
    abstractmethod,
)

class Bandit:
    def __init__(self, m):
        """
        Simulates bandit.
        Args:
            m (float): True mean.
        """

        self.m = m
        self.id = uuid4()

    def pull(self):
        """
        Simulate pulling the arm of the bandit.
        Normal distribution with mu = self.m and sigma = 1.
        """
        reward = np.random.randn() + self.m

        return reward

class BernoulliBandit:
    def __init__(self, p):
        """
        Simulates bandit.
        Args:
            p: Probability of success.
        """
        (a, b) = p
        self.success = a
        self.trials = b
        self.id = uuid4()

    def pull(self):
        """
        Simulate pulling the arm of the bandit.
        """
        return np.random.binomial(1, self.p, size=1)[0]
        
class BanditRewardsLog:
    def __init__(self):
        self.selected = []

    '''
    def record_action(self, bandit, actions, reward, selected_params, score):
        self.selected_params = selected_params
        self.total_actions += actions
        self.total_rewards += reward
        self.all_rewards.append(score)

        self.record[bandit.id]['actions'] += actions
        self.record[bandit.id]['reward'] += reward
    '''
    def record_action(self, selected_param):
        self.selected = selected_param


class Agent(ABC):

    def __init__(self):
        self.rewards_log = BanditRewardsLog()
        self._bandits = None

    @property
    def bandits(self) -> List[Bandit]:
        return self._bandits

    @bandits.setter
    def bandits(self, val: List[Bandit]):
        self._bandits = val

    @abstractmethod
    def take_action(self):
        ...

    def take_actions(self, n: int):
        for _ in range(n):
            self.take_action()


class BayesianAgent(Agent):
    def __init__(self, reward_distr='bernoulli'):
        if reward_distr not in ('bernoulli'):
            raise ValueError('reward_distr must be "bernoulli".')

        self.reward_distr = reward_distr
        super().__init__()

    def _sample_bandit_mean(self, bandit):
        '''
        print(bandit.success)
        print(bandit.trials)
        '''

        successes = float(bandit.success)
        failures = float(bandit.trials)

        if failures == 0:
            failures = 1

        if successes == 0:
            successes = 1

        if self.reward_distr == 'bernoulli':
            # + 1 for a Beta(1, 1) prior
            return np.random.beta(a=successes, b=failures, size=1)[0]
        else:
            raise NotImplementedError()

    def take_action(self):
        samples = [self._sample_bandit_mean(bandit) for bandit in self.bandits]

        self.rewards_log.record_action(samples)
        return

    def __repr__(self):
        return 'BayesianAgent(reward_distr="{}")'.format(self.reward_distr)        

def read_probs(path):
    op_result = []
    sp_result = []
    time_to_sp = 0
    prob_file = open(path, 'r')

    while True:
        line = prob_file.readline()
        if not line: break

        bandit_info = line.split()

        if '-' in line:
            time_to_sp = 1
            continue

        if time_to_sp == 1:
            sp_result.append(tuple(bandit_info))
        else:
            op_result.append(tuple(bandit_info)) 

    return (op_result, sp_result)


def write_selected_mut(path, op, sp):
    param_file = open(path, 'w')

    for i in range(len(op)):
        param_file.write(str(int(op[i] * 1000)) + '\n')
    
    param_file.write("-\n")

    param_file.close()

    return

def get_params(
    op_agent,
    sp_agent,
    op_bandits: List[Bandit],
    sp_bandits: List[Bandit],
    path
):
    op_agent.bandits = op_bandits
    sp_agent.bandits = sp_bandits
    op_list = []
    sp_list = []

    op_agent.take_actions(1)

    write_selected_mut(path + "_next", op_agent.rewards_log.selected, sp_agent.rewards_log.selected)
    
    return

if __name__ == '__main__':

    (param_probs, dummy) = read_probs(sys.argv[1])

    bernoulli_bandits_op = [BernoulliBandit (p) for p in param_probs]
    dummy = []

    get_params(BayesianAgent('bernoulli'), BayesianAgent('bernoulli'), bernoulli_bandits_op, dummy, sys.argv[1])


