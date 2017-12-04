#include <iostream>
#include <cmath>

int main() {
	const int num = 312051;
	const auto sq = static_cast<int>(std::ceil(std::sqrt(num)));
	const auto squared = (sq) * (sq);
	const auto how_many_between = (squared - num) / sq;
	const auto how_far_in_line = how_many_between * sq;
	
	const auto max_in_line = (squared - sq * how_many_between);
	const auto min_in_line = max_in_line - sq + 1;
	std::cout << min_in_line << std::endl;
	const auto med_in_line = std::ceil((max_in_line + min_in_line)/2);
	const auto hor_offset = std::fabs(med_in_line - num);
	std::cout << med_in_line << std::endl;
	const auto vert_offset = sq/2;
	std::cout << vert_offset + hor_offset;

	
}
