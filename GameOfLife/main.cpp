#include <thread>     // sleep_for
#include <chrono>     // milliseconds

#include "GameOfLife.hpp"
#include "LangtonsAnt.hpp"

int main(int argc, char* argv[])
{
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " WIDTH" << " HEIGHT\n";
    exit(EXIT_FAILURE);
  }

  // 236 - maximum width of the terminal with border when maximized
  // 238 - maximum width of the terminal without border when maximized
  // 100 - maximum width of the terminal when minimized
  const size_t WIDTH = atoi(argv[1]);

  // 55 - maximum height of the terminal with border when maximized
  // 57 - maximum height of the terminal without border when maximized
  // 25 - maximum height of the terminal when minimized
  const size_t HEIGHT = atoi(argv[2]);

  // GameOfLife GOL(WIDTH, HEIGHT);
  GameOfLife GOL(WIDTH, HEIGHT,
    {"GameOfLifeExt/StillLifes", "GameOfLifeExt/Oscillators"});

  GOL.render();

  // Game of Life (Without patterns)
  // while (true) {
  //   GOL.render();
  //   GOL.next_state();
  //   std::this_thread::sleep_for(std::chrono::milliseconds(30));
  // }
  
  // Game of Life (With patterns)
  // for (const auto& pattern : GOL.getExt().getRefPatterns()) {
  //   std::cout << pattern.first << ":\n";
  //   for (int w = 0; w < pattern.second.second; w++) {
  //     for (int h = 0; h < pattern.second.second; h++) {
  //       std::cout << pattern.second.first[w][h];
  //     }
  //     std::cout << "\n";
  //   }
  // }

  // Langton's Ant
  // LAnt lant(WIDTH, HEIGHT);
  // while (true) {
  //   render(arr, WIDTH, HEIGHT);
  //   lant.ant_move(arr);
  //   std::this_thread::sleep_for(std::chrono::milliseconds(30));
  // }
}