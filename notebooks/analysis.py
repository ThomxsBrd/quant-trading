import pandas as pd
import numpy as np
import matplotlib.pyplot as plt


def calculate_profit(signals, prices):

    profit = pd.Series(index=prices.index, dtype = 'float64')
    profit.fillna(0, inplace=True)

    buys = signals[signals['orders'] == 1].index
    sells = signals[signals['orders'] == -1].index
    skip = 0
    for bi in buys:
        if skip > 0:
            skip -= 1
            continue
        sis = sells[sells > bi]
        if len(sis) > 0:
            si = sis[0]
            profit[si] = prices[si] - prices[bi]
            skip = len(buys[(buys > bi) & (buys < si)])
        else:
            profit[-1] = prices[-1] - prices[bi]
    cum_profit = profit.cumsum()

    return cum_profit

def plot_strategy(prices, signal, profit):

    fig, (ax1, ax2) = plt.subplots(2, 1, gridspec_kw={'height_ratios': (3, 1)},
                                   figsize=(24, 12))

    ax1.set_xlabel('Date')
    ax1.set_ylabel('Price in $')
    ax1.plot(prices.index, prices, color='g', lw=0.25)

    # Plot the Buy and Sell signals
    ax1.plot(signal.loc[signal.orders == 1.0].index,
             prices[signal.orders == 1.0],
             '^', color='blue', label='Buy')
    ax1.plot(signal.loc[signal.orders == -1.0].index,
             prices[signal.orders == -1.0],
             'v', color='red', label='Sell')

    ax2.plot(profit.index, profit, color='b')
    ax2.set_ylabel('Cumulative Profit (%)')
    ax2.set_xlabel('Date')

    return ax1, ax2