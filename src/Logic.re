let () = Random.self_init()

let count_neighbours(matrix, w, h, y, x) =
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
  let count_neighbours = count_neighbours(matrix, h, w);
  let make_cell = (i, j) =>
    fun
    | true => MineCell.Bomb
    | false => MineCell.Hint(count_neighbours(i, j));
  let make_cell(i, j, v) = MineCell.{opened: false, cellType: make_cell(i, j, v)};
  let make_row = i => Array.mapi(make_cell(i));
  let matrix = Array.mapi(make_row, matrix);
  matrix;
};

let show_cell(board, j, i) = MineCell.({
  board[j][i] = {
    ...(board[j][i]), opened: true
  }
});

let rec propagate_open(board, j, i, h, w) =
  MineCell.(switch (board[j][i].cellType)
  {
    | Hint(n) => {
        if (!board[j][i].opened)
        {
          show_cell(board, j, i);
          if (n == 0)
          {
            if (j > 0) propagate_open(board, j - 1, i, h, w);
            if (j < h) propagate_open(board, j + 1, i, h, w);
            if (i > 0) propagate_open(board, j, i - 1, h, w);
            if (i < w) propagate_open(board, j, i + 1, h, w);
          }
        }
      }
   | Bomb => () // we will never be here because they are hint around
  });
