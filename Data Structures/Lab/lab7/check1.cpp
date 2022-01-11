#include <iostream>
#include <cmath>

int count_path(int x, int y){
	int count=0;
	if (x==0 && y==0)
		return 1;
	
	if ((std::abs(x+1)+std::abs(y))<=(std::abs(x)+std::abs(y)))
		count+=count_path(x+1,y);
	if ((std::abs(x-1)+std::abs(y))<=(std::abs(x)+std::abs(y)))
		count+=count_path(x-1,y);
	if ((std::abs(x)+std::abs(y-1))<=(std::abs(x)+std::abs(y)))
		count+=count_path(x,y-1);
	if ((std::abs(x)+std::abs(y+1))<=(std::abs(x)+std::abs(y)))
		count+=count_path(x,y+1);
	return count;
}

int main(){
	std::cout << count_path(3,3) << std::endl;
}