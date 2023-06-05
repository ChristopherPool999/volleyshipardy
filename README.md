Final for Data Structures Class @ Clovis Community College.
A 2player Game that is a combination of Volleyball, Jeopardy, and Battleship.
Uses simple networking to connect a player on a client and a player on a server.

Screenshots
----------------------------------------------------------------------------------------------------------------------------------------
https://imgur.com/a/9miJnAr

Noteworthy Features
----------------------------------------------------------------------------------------------------------------------------------------
- Some fairly complex networking logic such as showing the enemy players screen but only allowing one player input
- It will even track the enemy's cursor location in real time as they are moving around the map, while still not allowing player input until their turn

- Volleyball is basically a trivia round where each player must answer a question in lower and lower time, the networking will figure out who's turn it is and have the appropriate player attack and other watch, swapping between the two and sending the other player the time needed to answer in. 
- WASD movement using ncurses to clear the screen and reprint on every input, updating the map which is a 2d array

Libraries
-----------------------------------------------------------------------------------------------------------------------------------------
- Boost Asio - networking
- Ncurses - UI
