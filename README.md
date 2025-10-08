# COMP345_Project_N20



# Running the Program

## Compilation

Compile the code using the following command:

### For VSCode:
```
g++ -o MainDriver.exe MainDriver.cpp Cards/Cards.cpp Orders/Orders.cpp Player/Player.cpp Map/Map.cpp Game_Engine/GameEngine.cpp
```
### For Visual Studio
```
cl -o MainDriver.exe MainDriver.cpp Cards/Cards.cpp Orders/Orders.cpp Player/Player.cpp Map/Map.cpp Game_Engine/GameEngine.cpp
```

## Execution

Run the compiled executable:

```
./MainDriver.exe
```

## Game Engine Test

The first test that runs is the game engine test. You'll need to enter commands to transition between game states.

### State Transitions

The game starts in the **Start** state. Follow these commands in order:

**1. Load a map** (transitions to Map Loaded state):

```
loadmap Map/Asia.map
```

Replace `Asia.map` with your desired map file from the `Map/` directory.

**2. Validate the map**:

```
validatemap
```

**3. Add players**:

```
addplayer Player1
addplayer Player2
```

Add as many players as needed.

**4. Assign countries**:

```
assigncountries
```

> **Note:** This command doesn't perform much functionality yet, as it wasn't part of the current assignment.

**5. Issue orders**:

```
issueorder
```

**6. End issuing orders**:

```
endissueorders
```

**7. Execute orders**:

```
execorder
```

**8. Win condition**:

```
win
```

**9. End game**:

```
end
```