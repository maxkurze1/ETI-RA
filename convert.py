#!/bin/env python3
import pandas as pd
import sys
import os

data = {}

# columns = set()
# indicies = set()

# for file in sys.argv[1:-1]:
#     filename = file.split('.')[0]
#     version = filename.split('-')[0]
#     size    = filename.split('-')[1]
#     indicies.add(size)
#     columns.add(version)

for file in sys.argv[1:-1]:
    filename = os.path.splitext(os.path.basename(file))[0]
    version = filename.split('-')[0]
    size    = filename.split('-')[1]
    df = pd.read_csv(file)['values']
    if version not in data:
        data[version] = {}
        data[version + '-error-minus'] = {}
        data[version + '-error-plus'] = {}
        data[version + '-min'] = {}
        data[version + '-max'] = {}
    data[version][size] = df.median()
    data[version + '-error-minus'][size] = df.median() - df.min()
    data[version + '-error-plus'][size] = df.max() - df.median()
    data[version + '-min'][size] = df.max()
    data[version + '-max'][size] = df.max()

df = pd.DataFrame(data)
df.index.names = ['size']

df.to_csv(sys.argv[-1],sep=' ')
