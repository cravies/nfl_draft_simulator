from matplotlib import pyplot as plt 
import seaborn as sns

# team filename
NUM_TEAMS=sum(1 for _ in open('./settings/teams.txt'))

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
        # number of picks, need modulus if we are doing multiple simulations
        # and thus have multiple of the same pick
        i = i % NUM_TEAMS
        sp = line.split(',')
        team = sp[0]
        # some teams have multiple picks so need to demarcate between them
        team = f'{team}_{i+1}'
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
        d = normalise_dict(picks[team])
        keys = list(d.keys())
        # get values in the same order as keys, and parse percentage values
        vals = [d[k] for k in keys]
        # word wrapping
        keys =  [key.replace(' ','\n') for key in keys]
        sns.barplot(x=keys, y=vals)
        plt.xticks(rotation=90)
        plt.tight_layout()
        plt.savefig(f'./output/{team}')
        plt.close()
    f.close()

if __name__=="__main__":
    plot_results("./output/draft.txt")