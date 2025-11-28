<h1 style="display: inline;">
  Silkfish
  <img src="images/logo64.gif" alt="Silkfish Logo" style="vertical-align: middle;"/>
</h1>

<div align="center">

![Lichess Bot Status](https://img.shields.io/badge/Lichess%20Bot-Online-brightgreen?style=flat-square&logo=lichess&logoColor=white)  
![Lichess Bot Rating](https://img.shields.io/badge/Rapid_Rating-1708-blue?style=flat-square&logo=chess)
![Lichess Bot Rating](https://img.shields.io/badge/Classical_Rating-1668-blue?style=flat-square&logo=chess)

</div>

---

* **2024.9.7:** silkfish is now a Lichess Bot &#129302; on lichess.org! View its [profile](https://lichess.org/@/silkfish).
* **2025.11.28:** Rewriting silkfish to bring it back online!
    
## 1. Compile the Engine
***silkfish*** engine is written in c++, so you do need a ```g++``` compiler to compile it. 

To compile, run the following command under the current directory,

    make

This should give you an executable named ```silkfish``` under the current directory, being the engine itself &#x270C;

## 2. UCI Commands
***silkfish*** is by default running under UCI mode, meaning all the interactions are done through standard I/O. Here are some useful UCI commands:

1. `uci`: Get basic information of the engine.
2. `isready`: Check if engine is ready to accept the next command.
3. `setoption name <option name> value <v>`: Set an option. For example, `setoption name Threads value 8`.
4. `ucinewgame`: Start a new game.
5. `position startops`: Set the board to start position, or with given moves. For example, `position startops moves e2e4 e7e5 g1f3`.
6. `position fen <fenstring>`: Set the board to the given FEN position, or with given moves. For example, `position fen <fenstring> moves e2e4 e7e5 g1f3`.
7. `go`: Tell the engine to start searching. Can specify limitations like time control. For example, `go wtime 60000 btime 60000 winc 1000 binc 1000`.
8. `stop`: Stop searching. The engine will output `bestmove <move>`.
9. `quit`: Quit the engine.