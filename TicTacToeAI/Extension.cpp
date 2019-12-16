#include "Extension.hpp"

std::pair<unsigned, unsigned> Extension::random_move(const TicTacToe& board)
{
  const auto board_copy = board.get_board();
  const unsigned width = board_copy.size();
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> range(0, width - 1);

  // return the pair of unsigned integers
  // each represents x and y coordinates, respectively.
  unsigned x;
  unsigned y;

  do {
    x = range(gen);
    y = range(gen);
  } while (board_copy[y][x] != ' ' && !board.is_full());

  return {x, y};
}

std::pair<unsigned, unsigned> Extension::winning_move(const TicTacToe& board, char mark)
{
  const auto board_copy(board.get_board());
  const unsigned width(board.get_board().size());

  std::vector<std::vector<unsigned>> lines(2);
  std::vector<unsigned> crosses(2);

  /*
   * -----
   * -----
   * -----
   */

  for (unsigned row = 0; row < width; row++) {
    unsigned n = 0;
    for (unsigned col = 0; col < width; col++) {
      if (board_copy[row][col] != mark && board_copy[row][col] != ' ') {
        n = -1;
        break;
      }
      if (board_copy[row][col] == mark)
        n++;
    }
    lines[0].push_back(n);
  }

  /*
   * | | |
   * | | |
   * | | |
   */

  for (unsigned col = 0; col < width; col++) {
    unsigned n = 0;
    for (unsigned row = 0; row < width; row++) {
      if (board_copy[row][col] != mark && board_copy[row][col] != ' ') {
        n = -1;
        break;
      }
      if (board_copy[row][col] == mark)
        n++;
    }
    lines[1].push_back(n);
  }

  /*
   * *
   *   *
   *     *
   */

  unsigned n = 0;
  for (unsigned row = 0, col = 0; row < width; row++, col++) {
    if (board_copy[row][col] != mark && board_copy[row][col] != ' ') {
      n = -1;
      break;
    }
    if (board_copy[row][col] == mark)
      n++;
  }
  crosses[0] = n;

  /*
   *     *
   *   *
   * *
   */

  n = 0;
  for (unsigned row = 0, col = width - 1; row < width; row++, col--) {
    if (board_copy[row][col] != mark && board_copy[row][col] != ' ') {
      n = -1;
      break;
    }
    if (board_copy[row][col] == mark)
      n++;
  }
  crosses[1] = n;

  std::vector<unsigned> patterns(lines[0]);
  patterns.insert(patterns.end(), lines[1].begin(), lines[1].end());
  patterns.insert(patterns.end(), crosses.begin(), crosses.end());

  unsigned greatest(0);
  for (const auto& pattern : patterns)
    if (greatest < pattern && pattern != static_cast<unsigned>(-1))
      greatest = pattern;

  std::vector<unsigned> greatest_options;
  
  for (unsigned i = 0; i < patterns.size(); i++)
    if (greatest == patterns[i])
      greatest_options.push_back(i);

  std::random_device rd;
  std::mt19937 gen(rd());

  if (greatest_options.size() == 0) {
    std::vector<std::pair<unsigned, unsigned>> dummy_pos;

    for (unsigned row = 0; row < width; row++) {
      for (unsigned col = 0; col < width; col++) {
        if (board_copy[row][col] == ' ')
          dummy_pos.emplace_back(col, row);
      }
    }

    std::uniform_int_distribution<> dummy_range(0, dummy_pos.size() - 1);

    return dummy_pos[dummy_range(gen)];
  } else {
    std::uniform_int_distribution<> pattern_range(0, greatest_options.size() - 1);
    unsigned p_no(greatest_options[pattern_range(gen)]);

    std::vector<unsigned> pos;

    if (p_no < width) {
      for (unsigned i = 0; i < width; i++)
        if (board_copy[p_no][i] == ' ')
          pos.push_back(i);

      std::uniform_int_distribution<> pos_range(0, pos.size() - 1);

      return {pos[pos_range(gen)], p_no};
    } else if (p_no < width * 2) {
      for (unsigned i = 0; i < width; i++)
        if (board_copy[i][p_no - width] == ' ')
          pos.push_back(i);

      std::uniform_int_distribution<> pos_range(0, pos.size() - 1);
      
      return {p_no - width, pos[pos_range(gen)]};
    } else {
      if (p_no - width * 2 == 0) {
        for (unsigned row = 0, col = 0; row < width; row++, col++)
          if (board_copy[row][col] == ' ')
            pos.push_back(row);

        std::uniform_int_distribution<> pos_range(0, pos.size() - 1);
        unsigned xy(pos[pos_range(gen)]);

        return {xy, xy};
      } else {
        std::vector<std::pair<unsigned, unsigned>> cross_pos;

        for (unsigned row = 0, col = width - 1; row < width; row++, col--)
          if (board_copy[row][col] == ' ')
            cross_pos.emplace_back(col, row);

        std::uniform_int_distribution<> pos_range(0, cross_pos.size() - 1);

        return cross_pos[pos_range(gen)];
      }
    }
  }
}

std::pair<unsigned, unsigned>
Extension::minimax_move(const TicTacToe& ttt, char mark)
{
  auto board(ttt.get_board());
  const unsigned width(board.size());

  const auto moves(get_legal_moves(board, mark));
  std::vector<unsigned> scores;
  int best_score(std::numeric_limits<int>::min());
  std::pair<unsigned, unsigned> best_move;

  for (const auto& move : moves) {
    auto new_board(board);
    make_move(new_board, move, mark);
    char opponent = mark == 'X' ? 'O' : 'X';
    int new_score(minimax_score(new_board, opponent, mark));
    
    if (best_score < new_score) {
      best_score = new_score;
      best_move = move;
    }
  }

  return best_move;
}

int Extension::minimax_score(
  std::vector<std::vector<char>>& board, char mark, char whoami
)
{
  const char opponent = mark == 'X' ? 'O' : 'X';

  if (has_won(board, whoami))
    return 10;
  else if (has_won(board, opponent))
    return -10;
  else if (is_full(board))
    return 0;

  const auto& legal_moves = get_legal_moves(board, mark);
  std::vector<int> scores;

  for (const auto& move : legal_moves) {
    auto new_board(board);
    make_move(new_board, move, mark);
    int score = minimax_score(new_board, opponent, whoami);
    scores.push_back(score);
  }

  if (mark == whoami)
    return *std::max_element(scores.begin(), scores.end());
  else
    return *std::min_element(scores.begin(), scores.end());
}

std::vector<std::pair<unsigned, unsigned>>
Extension::get_legal_moves(const std::vector<std::vector<char>>& board, char mark)
{
  const unsigned width(board.size());
  std::vector<std::pair<unsigned, unsigned>> legal_moves;

  for (unsigned row = 0; row < width; row++)
    for (unsigned col = 0; col < width; col++)
      if (board[row][col] == ' ')
        legal_moves.emplace_back(col, row);
  
  return legal_moves;
}

void Extension::make_move(
  std::vector<std::vector<char>>& board,
  std::pair<unsigned, unsigned> xy,
  char mark
)
{
  board[xy.second][xy.first] = mark;
}

bool Extension::has_won(const std::vector<std::vector<char>>& board, char mark)
{
  for (unsigned row = 0; row < board.size(); row++) {
    bool win = true;
    for (unsigned col = 0; col < board.size(); col++) {
      if (board[row][col] != mark) {
        win = false;
        break;
      }
    }
    if (win) return true;
  }

  for (unsigned col = 0; col < board.size(); col++) {
    bool win = true;
    for (unsigned row = 0; row < board.size(); row++) {
      if (board[row][col] != mark) {
        win = false;
        break;
      }
    }
    if (win) return true;
  }

  bool win = true;

  for (unsigned row = 0, col = 0; row < board.size(); row++, col++) {
    if (board[row][col] != mark) {
      win = false;
      break;
    }
  }

  if (win) return true;

  win = true;

  for (unsigned row = 0, col = board.size() - 1;
       row < board.size(); row++, col--) {
    if (board[row][col] != mark) {
      win = false;
      break;
    }
  }
  
  return win ? true : false;
}

bool Extension::is_full(const std::vector<std::vector<char>>& board)
{
  // check whether board is full (draw)
  for (const auto& row : board)
    for (const auto& col : row)
      if (col == ' ')
        return false;

  return true;
}