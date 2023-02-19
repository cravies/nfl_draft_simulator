from matplotlib import pyplot as plt 
import numpy as np

def normalise_dict(d):
    # normalise a "count dict" to have a probability distribution 
    # over its entries
    # also sort by probability value
    total=0
    for key in d.keys():
        total += d[key]
    for key in d.keys():
        d[key] /= total
    return dict(sorted(d.items(), key=lambda item: item[1], reverse=True))

def plot_results(fname):
    # grab results from file
    f = open(fname, "r")
    lines = f.readlines()
    picks = {}
    for i,line in enumerate(lines):
        # 63 picks in first two rounds
        i = i % 63
        sp = line.split(',')
        team = sp[0]
        # some teams have multiple picks so need to demarcate between them
        team = f'{team}_{i}'
        player = sp[1].replace('\n','')
        #first time, add to dict
        if team not in picks:
            picks[team]={}
        #first time player picked by team
        if player not in picks[team]:
            picks[team][player] = 1
        else:
            picks[team][player] += 1
    for team in picks:
        print(team,": ",normalise_dict(picks[team]))
    f.close()

if __name__=="__main__":
    plot_results("draft.txt")