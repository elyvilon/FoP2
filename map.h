// types of objects
enum ObjectType {
  BLOCK,
  NONBLOCK,
  NONE,
  BIRCH,
  SPRUCE,
  BOULDER,
  MUSHROOM,
  HOLE,
  FIRE,
  GOAL
};

// empty matrix of objects
ObjectType objectMap[20][4] = {NONBLOCK};

// position variables
struct Position {
  int col;
  int row;
};

bool visited[20][4];


// function to randomly draw boulders, fire and holes to tiles marked for blocking objects
void randomizeObstacles(int currentLevel) {
  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 20; col++) {
      if (objectMap[col][row] == BLOCK) {
        int rand = random(3);
        switch (rand) {
          case 0:
            objectMap[col][row] = BOULDER;
            break;
          case 1:
            objectMap[col][row] = HOLE;
            break;
          case 2:
            objectMap[col][row] = FIRE;
            break;
        }
      } else if (objectMap[col][row] == NONBLOCK) { // nonblocking objects and objects to be collected
          // random number for probabilities
          int rand = random(100)+1;

          // probabilities if easy difficulty
          if (currentLevel <= 10) {
            if (rand <= 40) objectMap[col][row] = NONE;
            else if (rand >= 41 && rand <= 60) objectMap[col][row] = MUSHROOM;
            else if (rand >= 61 && rand <= 80) objectMap[col][row] = BIRCH;
            else if (rand >= 81 && rand <= 100) objectMap[col][row] = SPRUCE;
          }
          // probabilities if medium difficulty
          else if (currentLevel > 10 && currentLevel <= 20) {
            if (rand <= 30) objectMap[col][row] = NONE;
            else if (rand >= 31 && rand <= 50) objectMap[col][row] = MUSHROOM;
            else if (rand >= 51 && rand <= 70) objectMap[col][row] = BIRCH;
            else if (rand >= 71 && rand <= 100) objectMap[col][row] = SPRUCE;
          }
          // probabilities if hard difficulty
          else if (currentLevel > 20) {
            if (rand <= 20) objectMap[col][row] = NONE;
            else if (rand >= 21 && rand <= 30) objectMap[col][row] = MUSHROOM;
            else if (rand >= 30 && rand <= 65) objectMap[col][row] = BIRCH;
            else if (rand >= 66 && rand <= 100) objectMap[col][row] = SPRUCE;
          }
                   
      }
    }
  }
}


// mark all tiles on the map nonblocking
void clearMap() {
  for (int col = 0; col < 20; col++) {
    for (int row = 0; row < 4; row++) {
      objectMap[col][row] = NONBLOCK;
      visited[col][row] = false;
    }
  }
}


// DFS algorithm to create a path between start and goal that will not have blocking objects
bool createPath() {
  Position stack[80]; // stack for a 20x4 grid
  int stackSize = 0;
  
  // starting point col0, row0
  stack[stackSize++] = {0, 0};
  visited[0][0] = true;
  objectMap[0][0] = NONE;
  
  while (stackSize > 0) {
    Position current = stack[--stackSize];
    
    // if in last col and object in tile is goal flag, goal reached
    if (current.col == 19) {
      objectMap[current.col][current.row] = GOAL;
      return true;
    }
    
    objectMap[current.col][current.row] = NONE; // mark tile as empty and thus part of the path

    // possible directions (right, down, up)
    Position directions[3] = {{current.col + 1, current.row}, {current.col, current.row + 1}, {current.col, current.row - 1}};
    
    // suffle the directions randomly
    for (int i = 0; i < 3; i++) {
      int r = random(i, 3);
      Position temp = directions[i];
      directions[i] = directions[r];
      directions[r] = temp;
    }
    
    // add applicaple tiles to stack
    for (int i = 0; i < 3; i++) {
      int newCol = directions[i].col;
      int newRow = directions[i].row;
      
      // check if the tile is within level bounds and if it has not already been visited
      if (newCol >= 0 && newCol < 20 && newRow >= 0 && newRow < 4 && !visited[newCol][newRow]) {
        visited[newCol][newRow] = true; // mark as visited
        stack[stackSize++] = {newCol, newRow};  // add to stack
      }
    }
  }
  
  return false;  // return error if stack got empty before reaching the last column
}

// generate a new random map for level
void generateMap() {
  clearMap();

  // create the path of nonblocking objects
  if (!createPath()) {
    // in the rare case a path was unable to be generated, try again
    generateMap();
    return;
  }

  // randomly place blocking objects in tiles outside the path
  for (int col = 0; col < 20; col++) {
    for (int row = 0; row < 4; row++) {
      if (objectMap[col][row] == NONBLOCK && random(100) < 35) {
        objectMap[col][row] = BLOCK;
      }
    }
  }
}
