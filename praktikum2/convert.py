#!/bin/env python3
import pandas as pd
import sys
import os

data = {}

# columns = set()
# indicies = set()

# for file in sys.argv[1:-1]:
#     filename = file.split('.')[0]
#     size = filename.split('-')[0]
#     size    = filename.split('-')[1]
#     indicies.add(size)
#     columns.add(size)

for file in sys.argv[1:-1]:
    filename = os.path.splitext(os.path.basename(file))[0]
    size = filename.split('-')[1]
    proc = filename.split('-')[2]
    df = pd.read_csv(file)['values']
    if size not in data:
        data[size] = {}
        data[size + '-error-minus'] = {}
        data[size + '-error-plus'] = {}
        data[size + '-min'] = {}
        data[size + '-max'] = {}
    data[size][proc] = df.median()
    data[size + '-error-minus'][proc] = df.median() - df.min()
    data[size + '-error-plus'][proc] = df.max() - df.median()
    data[size + '-min'][proc] = df.max()
    data[size + '-max'][proc] = df.max()

df = pd.DataFrame(data)
df.index.names = ['proc']

df.to_csv(sys.argv[-1],sep=' ')
