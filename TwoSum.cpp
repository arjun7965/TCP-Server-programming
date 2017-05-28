#include<iostream>
#include<vector>

using namespace std;

class Solution {
public:
	vector<int> index;
	int compare(int x, int y, int target);
	vector<int> twoSum(vector<int>& nums, int target)
	{
		
		for (int i = 0; i<nums.size(); i++)
		{
			for (int j = i+1; j<nums.size(); j++)
			{
				int z = compare(nums.at(i), nums.at(j), target);
				if (z == 1)
				{
					index.push_back(i);
					index.push_back(j);
					//return index;
					//break;
				}
				else
				{
					continue;
				}

			}


		}
		for (int a = 0; a < index.size(); a++)
			cout << index.at(a) << "  ";
		return index;
	}
};

int Solution::compare(int x, int y, int target)
{
	if ((x + y) == target)
		return 1;
	else
		return 0;
}

void main()
{
	vector<int> number = {2,3,6,1,1,7};
	int x = 9;
	Solution s;
	s.twoSum(number, x);
	system("pause");

}