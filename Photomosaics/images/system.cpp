#include <cstdlib>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

int main()
{
	system(("rm " + fs::current_path().string() + "/test/*").c_str());
}
