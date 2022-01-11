# include <iostream>
# include <vector>
using namespace std;

int main()
{
	vector<float> nums;
	int howmany;
	float temp;
	float sum = 0.0;
	float avg;
	cout << "How many nums?" << endl;
	cin >> howmany;
	for (int i = 0; i<(howmany); i++)
	{
		cout << "Your num?"<< endl;
		cin >> temp;
		nums.push_back(temp);
		sum += temp;
	}
	avg = sum/howmany;
	cout << "The avg is " << avg << endl;
	for (int i=0; i<(howmany); i++)
	{
		if (nums[i] < avg)
		{
			cout << nums[i] << " ";
		}
	}
	return 0;
}