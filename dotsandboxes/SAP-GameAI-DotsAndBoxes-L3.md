# SAP-GameAI-DotsAndBoxes-L3 (SAPGAIDABL3)
This is an AI algorithm as a type of L3 (predictive) AI game algorithm.
This algorithm uses rules and a map of balances to make predictions on where to next.

Since obviously the aim of the game is to get the most points, the bot needs to make the most boxes.

## Algorithm
The image below is an example of a beginning move, before the player made a move, each of those grey
bars would be equal to 0.5, which would give them all an equal chance of being picked.

![Example A](<images/Dots And Boxes (SAP-GameAI-DotsAndBoxes-L3) 1.png>)

Now, before the bot even thinks about doing calculations and moving based on predictions, it first checks
the whole board for any easy boxes it could get, for instance if it finds a square where there are 3 sides,
it can add the fourth and get the point.

In the example below, you can see this rule.

![Example B](<images/Dots And Boxes (SAP-GameAI-DotsAndBoxes-L3) 2.png>)