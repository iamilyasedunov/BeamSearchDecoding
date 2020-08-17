import numpy as np
import argparse
import concurrent.futures
from src import beam_search_test as BeamSearch
from argparse import Namespace
from datetime import datetime
import sys, platform
import ctypes, ctypes.util

class BaseReader():
    """
        Base functional for reading data from file
    """
    def read(self):
        with open(self.path, encoding="utf-8") as f:
            self.data = f.read()
    def get(self):
        self.read()
        return self.data

class Alphabet(BaseReader):
    def __init__(self, path):
        self.path = path
        self.data = ""

class Probabilities(BaseReader):
    def __init__(self, path):
        self.path = path
        self.data = []
    def read(self):
        with open(self.path) as f:
            self.data = f.read().splitlines()
            self.data = [list(d.split(',')) for d in self.data]

class Decoder():
    def __init__(self, args):
        self.alphabet = args.alphabet
        self.probs = np.array(args.probs, dtype=float)
    def solve(self):
        start = datetime.now()
        output = BeamSearch.ctcBeamSearch(self.probs, self.alphabet, None)
        time = datetime.now() - start
        print('BEAM SEARCH  : "{}"'.format(output))
        print('TIME EXEC    : {} sec.'.format(time.total_seconds()))

def read_params(args):
    args.alphabet = Alphabet(args.alphabet_path).get()
    args.probs = Probabilities(args.probs_path).get()

if __name__ == '__main__':
    args = Namespace(
        alphabet = '', # chars
        probs = [],    # input matrix
        t_start = 0,
        t_end = 0,
    )
    parser = argparse.ArgumentParser(description='Optional app description')
    parser.add_argument('-a','--alphabet_path', type=str, help='Path to alphabet.txt')
    parser.add_argument('-p','--probs_path', type=str, help='Path to probs.csv')
    args = parser.parse_args()

    read_params(args)
    Decoder(args).solve()
