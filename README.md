Final for Data Structures Class @ Clovis Community College
A 2player Game that is a combination of Volleyball, Jeopardy, and Battleship.
Uses simple networking to connect a player on a client and a player on a server

Noteworthy features
----------------------------------------------------------------------------------------------------------------------------------------
- Some fairly complex networking logic such as showing the enemy players screen but only allowing one player input
- It will even track the enemy's cursor location in real time as they are moving around the map, while still not allowing player input until their turn

- Volleyball is basically a trivia round where each player must answer a question in lower and lower time, the networking will figure out who's turn it is and have the approiate player attack and other watch, swapping between the two and sending the other player the time needed to answer in. 

libraries
-----------------------------------------------------------------------------------------------------------------------------------------
- uses boost asio for networking
- nice UI using ncurses
