#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <numeric>

std::vector<std::uint8_t> convert(const std::string& input) {
	std::vector<std::uint8_t> array(input.size());
	std::string::size_type i = 0;
	std::generate(array.begin(), array.end(), [i, &input] () mutable { return input[i++] - '0';});
	return array;
}

unsigned sum(const std::string& input) {
	auto nums = convert(input);
	decltype(nums) filtered;
	decltype(nums)::size_type i = 0;
	std::copy_if(nums.begin(), nums.end(), std::back_inserter(filtered), 
	[i, &nums] (const auto& t) mutable 
	{
		const auto length = nums.size();
		auto idx = (i + length / 2) % length;
		i++;
		return t == nums[idx];
	});
	return std::accumulate(filtered.begin(), filtered.end(), 0U, [](const auto& e1, const auto& e2){ return e1 + e2;});
	
}


int main() {
	static const std::string input = "1212";
	auto s = sum(input);
	std::cout << s;
	return 0;
}
