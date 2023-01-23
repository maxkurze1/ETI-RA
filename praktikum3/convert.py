#!/bin/env python3
# from collections import defaultdict
import pandas as pd
import sys
import os
# import seaborn as sns
# import matplotlib.pyplot as plt
# import matplotlib as mpl

# sns.set_theme(style='white', palette='Set2')
# mpl.use("pgf")
# mpl.rcParams.update({
#     "pgf.texsystem": "pdflatex",
#     'font.family': 'serif',
#     'text.usetex': True,
#     'pgf.rcfonts': False,
# })
# plt.style.use('fivethirtyeight')

# def error_bar_plot(dataframe: pd.DataFrame, x: str, y: str):
#     print("data\n",dataframe)
#     gb = dataframe.groupby(x)[y]
#     values = gb.median().reset_index()
#     yerr = [(gb.median() - gb.min()).values, (gb.max() - gb.median()).values]
#     print("median\n", gb.median())
#     print("min\n",gb.min())
#     print("max\n",gb.max())


#     sns.barplot(x=x, y=y, data=values, yerr=yerr, capsize=10)
    

plot_data = pd.DataFrame(columns=['size','time', 'time-e-m', 'time-e-p','flops', 'flops-e-m', 'flops-e-p'])
for file in sys.argv[1:-1]:
    df = pd.read_csv(file)
    df['Metric Value'] = df['Metric Value'].str.replace(',','').astype(int)

    filename = os.path.splitext(os.path.basename(file))[0]
    # version = filename.split('-')[0]
    size = int(filename.split('-')[1])
    # oflag   = filename.split('-')[2]

    # data = pd.DataFrame()
    time = df[df['Metric Name'] == 'gpu__time_duration.sum']['Metric Value'].reset_index(drop=True) / 1_000_000_000

    flop = df[df['Metric Name'] == 'smsp__sass_thread_inst_executed_op_dfma_pred_on.sum']['Metric Value'].reset_index(drop=True) * 2 \
         + df[df['Metric Name'] == 'smsp__sass_thread_inst_executed_op_dadd_pred_on.sum']['Metric Value'].reset_index(drop=True) \
         + df[df['Metric Name'] == 'smsp__sass_thread_inst_executed_op_dmul_pred_on.sum']['Metric Value'].reset_index(drop=True)
    flops = flop / time

    # data = data.assign(size=size)

    plot_data = pd.concat([plot_data, pd.DataFrame({'size': size,
                      'time': time.median(),
                      'time-e-m': time.median() - time.min(),
                      'time-e-p': time.max() - time.median(),
                      'flop': flop.median(),
                      'flops': flops.median(),
                      'flops-e-m': flops.median() - flops.min(),
                      'flops-e-p': flops.max() - flops.min()
                      }, index=[0])],ignore_index=True);

    # plot_data = pd.concat([plot_data, data])

plot_data.to_csv(sys.argv[-1], index=False)

# flop_data = dict(sorted(flop_data.items()))

# error_bar_plot(plot_data, 'size', 'flops')
# plt.savefig('test')
