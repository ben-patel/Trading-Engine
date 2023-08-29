# Trading Engine
- Only facilitates limit orders right now
- Matching engine slightly inspired by QUANT CUP1 Voyager

### Average time to match orders:<sub>(single orderbook)</sub>
N. orders | 100 | 1k | 10k | 100k | 1M
--- | --- | --- | --- |--- |---
Seconds | 0.000237 | 0.00182 | 0.0183 | 0.270 | 2.684

### Average time to match orders: <sub>(multiple orderbooks' w/o multithreading)</sub>
N. orders | 100 | 1k | 10k | 100k | 1M
--- | --- | --- | --- |--- |---
Seconds | 0.0000622 | 0.000254 | 0.00184 | 0.0210 | 0.421

#### Note, my laptop can only run 8 threads at once, so multithreading in this case is extremely slow :\(
#### In reality it would be way quicker, as we run a seperate thread for each orderbook.

### Average time to match orders: <sub>(multiple orderbooks' w multithreading)</sub>
N. orders | 100 | 1k | 10k | 100k | 1M
--- | --- | --- | --- |--- |---
Seconds | 0.00154 | 0.0120 | 0.0918 | 0.849 | 9.43