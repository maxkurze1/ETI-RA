#!/bin/env python3
from collections import defaultdict
import pandas as pd
import sys
import os

data = defaultdict(dict)

for file in sys.argv[1:-1]:
    filename = os.path.splitext(os.path.basename(file))[0]
    # version = filename.split('-')[0]
    # size    = filename.split('-')[1]
    # oflag   = filename.split('-')[2]
    df = pd.read_csv(file, index_col='Metric Name')['Metric Value'].str.replace(',','').astype(int)
    time = df['gpu__time_duration.sum']

    fma = list(df['smsp__sass_thread_inst_executed_op_dfma_pred_on.sum'])
    add = list(df['smsp__sass_thread_inst_executed_op_dadd_pred_on.sum'])
    mul = list(df['smsp__sass_thread_inst_executed_op_dmul_pred_on.sum'])

    flops = pd.Series([(fma * 2 + add + mul)*(1_000_000_000/time) for (fma, add, mul, time) in zip(fma, add, mul, list(time))])

    data['time-min'][filename]          = time.min()
    data['time-max'][filename]          = time.max()
    data['time-median'][filename]       = time.median()
    data['time-error-minus'][filename]  = time.median() - time.min()
    data['time-error-plus'][filename]   = time.median() - time.max()
    data['flops-min'][filename]         = flops.min()
    data['flops-max'][filename]         = flops.max()
    data['flops-media'][filename]       = flops.median()
    data['flops-error-minus'][filename] = flops.median() - time.min()
    data['flops-error-plus'][filename]  = flops.median() - time.max()

df = pd.DataFrame(data)

df.to_csv(sys.argv[-1],sep=' ')
