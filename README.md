# EdgeViz
**Console Visualization library for Arduino**

Library for drawing pictures with strings on serial consoles.

Supports straight lines, rectangles, and circles.

```example.cpp
#include <vector>
#include <Arduino.h>
#include "EdgeViz.h"

void setup()
{
  Serial.begin(9600);
  EdgeViz::ConsoleViz viz(100, 100);
  viz.circle(/*origin*/{50, 50}, /*radius*/30, RED);
  viz.rectangle({10, 30}, {80, 50}, /*angle*/0.3, /*color*/BLUE);
  viz.line({50, 50}, {-10, 100}, WHITE);
  viz.imshow();
}
void loop(){}
```

Run the following command

```bash
picocom -b 9600 /dev/ttyACM0
```

![image](https://user-images.githubusercontent.com/23274028/189058188-ece8e0bb-e1a3-4b88-847b-753e435b0243.png)
