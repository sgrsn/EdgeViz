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
  EdgeViz::ConsoleViz viz(30, 30);
  viz.circle(/*origin*/{15, 15}, /*radius*/12, RED);
  viz.rectangle({10, 0}, {20, 15}, /*angle*/0.3, /*color*/BLUE);
  viz.line({50, 50}, {-10, -20}, WHITE);
  viz.imshow(true);
}
void loop(){}
```

Run the following command

```bash
picocom -b 9600 /dev/ttyACM0
```

![image](https://user-images.githubusercontent.com/23274028/190917819-48116a9a-d73c-481d-abad-fd5a4e44455e.png)

