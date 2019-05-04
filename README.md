## KatieBot: A cryptocurrency trading bot using Binance API

#### Features:
* Notifies user on his phone through Pushbullet each time a trade is made
* Qt Interface
* Test a strategy with the last 800 time periods (~ 1 month for 1h intervals)
* chart updated at the end of each period

#### Will be added
* Possibility to edit a config file to choose the strategy without having to change the code
* Separate simulation and real trades

#### Strategy:
This bot get its buy / trades signals from an MACD-like strategy with Hull Moving Averages instead of EMAs. (currently a 9-period and a 36-period HMA on time intervals of 1 hour).

#### Sources
Thanks to 11mariom for the [pushbullet API library](https://github.com/11mariom/pushbullet)
Thanks to tensaix2j for the [binance API library](https://github.com/tensaix2j/binacpp)

