let () = Random.self_init()

let countNeighbours(matrix, w, h, y, x) =
    (if (x > 0)
        ((if (y > 0 && matrix[y - 1][x - 1]) 1 else 0)
        + (if (matrix[y][x - 1]) 1 else 0)
        + (if (y < h && matrix[y + 1][x - 1]) 1 else 0))
    else
        0)
    + (if (y > 0 && matrix[y - 1][x]) 1 else 0)
    + (if (y < h && matrix[y + 1][x]) 1 else 0)
    + (if (x < w)
        ((if (y > 0 && matrix[y - 1][x + 1]) 1 else 0)
        + (if (matrix[y][x + 1]) 1 else 0)
        + (if (y < h && matrix[y + 1][x + 1]) 1 else 0))
    else
        0);

let minesweeper = (w, h, percent) => {
  let percent = 101 - percent;
  let matrix = Array.make_matrix(w, h, false);
  let (w, h) = (w - 1, h - 1);
  let matrix = Array.map(Array.map(_ => Random.int(100) >= percent), matrix); /* fill bomb */
  let countNeighbours = countNeighbours(matrix, h, w);
  let numberOfFlag = ref(0);
  let makeCell = (i, j) =>
    fun
    | true => {
        numberOfFlag := numberOfFlag^ + 1;
        MineCell.Bomb
      }
    | false => MineCell.Hint(countNeighbours(i, j));
  let makeCell(i, j, v) = MineCell.{opened: false, cellType: makeCell(i, j, v)};
  let makeRow = i => Array.mapi(makeCell(i));
  let matrix = Array.mapi(makeRow, matrix);
  (numberOfFlag^, matrix);
};

let showCell(board, j, i, rmnCell) = {
  rmnCell := rmnCell^ - 1;
  MineCell.({
  board[j][i] = {
    ...(board[j][i]), opened: true
  }
})};

let toogleFlag(board, j, i, nbrFlag) = {
  open MineCell;
  let cellType = switch (board[j][i].cellType)
  {
    | Flag(v) => {
      nbrFlag := nbrFlag^ + 1;
      v
    }
    | v => {
      nbrFlag := nbrFlag^ - 1;
      Flag(v)
    }
  };
  {
    board[j][i] = {
      ...(board[j][i]),
      cellType,
    }
  }
};

let rec propagateOpen(board, j, i, h, w, rmnCell) =
  MineCell.(switch (board[j][i].cellType)
  {
    | Hint(n) => {
        if (!board[j][i].opened)
        {
          showCell(board, j, i, rmnCell);
          if (n == 0)
          {
            if (j > 0) propagateOpen(board, j - 1, i, h, w, rmnCell);
            if (j < h) propagateOpen(board, j + 1, i, h, w, rmnCell);
            if (i > 0) propagateOpen(board, j, i - 1, h, w, rmnCell);
            if (i < w) propagateOpen(board, j, i + 1, h, w, rmnCell);
          }
        }
      }
    | Flag(_) // we need to cancel it before automatic open
    | Bomb => () // we will never be here because they are hint around
  });
