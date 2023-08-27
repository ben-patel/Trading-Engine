# Trading Engine
- Only facilitates limit orders right now
- Matching engine lightly inspired by QUANT CUP1 Voyager

### Average time to match orders: <font size='1'>(single orderbook)<font>
N. orders | 100 | 1k | 10k | 100k | 1M
--- | --- | --- | --- |--- |---
Seconds | 0.000237 | 0.00182 | 0.0183 | 0.270 | 2.684

### Average time to match orders: <font size='1'>(multiple orderbooks' w/o multithreading)<font>
N. orders | 100 | 1k | 10k | 100k | 1M
--- | --- | --- | --- |--- |---
Seconds | 0.000122 | 0.000974 | 0.00994 | 0.114 | 1.472